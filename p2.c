#include"vision_utilities.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
Image** im;

double* getNormal(int x, int y, int b, int cx, int cy, int r){
  // sphere satisfies equation:
  // (cx-x)^2+(cy-y)^2+z^2 = r^2
  // we return (-dz/dx, -dz/dy, 1)
  // z^2 = (r^2 - ((cx-x)^2+(cy-y)^2))
  // dz/dx = (cx-x)/sqrt(r^2 - ((cx-x)^2+(cy-y)^2))
  // dz/dy = (cy-y)/sqrt(r^2 - ((cx-x)^2+(cy-y)^2))
  double sq = sqrt(r*r - (cx-x)*(cx-x) - (cy-y)*(cy-y));
  double dzdx = -(cx-x) / sq;
  double dzdy = -(cy-y) / sq;
  double* toreturn = (double*) malloc(sizeof(double)*3);
  double scale = b / sqrt(dzdx*dzdx + dzdy*dzdy + 1);
  toreturn[0] = dzdx * scale;
  toreturn[1] = dzdy * scale;
  toreturn[2] = scale;
  return toreturn;
}


int main(int nargin, char** argv){
  if(nargin != 8){
    printf("Usage: p2 <input params> <image 1> <image 2> <image 3> <image 4> <image 5> <output directions file>");
    return -1;
  }
  
  FILE *file = fopen(argv[1], "r");
  if(file == NULL){
    printf("Cannot read %s", argv[1]);
    return -1;
  }

  char line[1024];
  fgets(line, 1024, file);
  int x = atoi(strtok(line, " "));
  int y = atoi(strtok(NULL, " "));
  int r = atoi(strtok(NULL, " "));

  im = (Image**) malloc(sizeof(Image*)*5);
  int i, j, k;
  for(i = 0; i < 5; i++){
    im[i] = (Image*) malloc(sizeof(Image));
    readImage(im[i], argv[i+2]);
  }
  
  // max brightness, and (x,y) where seen
  int mB, mX, mY;
  double* test = getNormal(2,2,1,0,0,4);
  double** normals = (double**) malloc(sizeof(double*) *5);
  for(k = 0; k < 5; k++){
    mB = mX = mY = 0;
    for(i = 0; i < getNRows(im[k]); i++){
      for(j = 0; j < getNCols(im[k]); j++){
	if(getPixel(im[k], i, j) > mB){
	  mB = getPixel(im[k],i,j);
	  mX = i;
	  mY = j; 
	}
      }
    }
    normals[k] = getNormal(mX, mY, mB, x, y, r);
    
    for(i = -2; i < 2; i++)
      for(j = -2; j < 2; j++)
	setPixel(im[k], mX+i, mY+j, 0);

    char f[20];
    sprintf(f, "test%d.pgm", k);
    writeImage(im[k], f);
  }
  

  FILE *writeFile = fopen(argv[7], "w+");
  for(k = 0 ; k < 5; k++){
    fprintf(writeFile, "%d %d %d\n", (int) normals[k][0], (int) normals[k][1], (int) normals[k][2]);
  }
  fclose(writeFile);
  fclose(file);
  
  
  free(im);
}
