#include"vision_utilities.h"
#include"util.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
Image** im;
Image* mask;

int main(int nargin, char** argv){
  if(nargin != 11){
    printf("Usage: p4 <input directions> <image 1> <image 2> <image 3> <image 4> <image 5> <mask> <normal map> <albedo map> <output gradient>");
    return -1;
  }
  

  FILE* file = fopen(argv[1], "r");
  
  im = (Image**) malloc(sizeof(Image*) * 5);
  mask = (Image*) malloc(sizeof(Image));
  readImage(mask, argv[7]);
  
  char line[1024];
  int i, j, k;
  Matrix* S = matrix(5,3);
  for(i = 0; i < 5; i++){
    line[0] = 0;
    im[i] = (Image*) malloc(sizeof(Image));
    //printf("%s\n", argv[i+2]);
    readImage(im[i], argv[i+2]);
    fgets(line, 1024, file);

    int x = atoi(strtok(line, " "));
    int y = atoi(strtok(NULL, " "));
    int z = atoi(strtok(NULL, " "));
    setM(S, i, 0, (double) x);
    setM(S, i, 1, (double) y);
    setM(S, i, 2, (double) z);
  }


  Matrix* I = matrix(5,1);
  Matrix* T = times(inverse(times(transpose(S),S)),transpose(S));
  double** norm = (double**) malloc(sizeof(double*) * getNRows(mask) * getNCols(mask));
  
  for(i = 0; i < getNRows(mask); i++){
    for(j = 0; j < getNCols(mask); j++){
      int index = j + i * getNRows(mask);
      if(!getPixel(mask, i, j)){
	double* tmp = (double*) malloc(sizeof(double)*3);
	norm[index] = tmp;
	continue;
      }
      
      for(k=0;k<5;k++)
	setM(I, k, 0, getPixel(im[k],i,j));

      norm[index] = times(T,I)->data;
    }
  }
  
  Gradient* grid = (Gradient*) malloc(sizeof(Gradient)*getNRows(mask)*getNCols(mask));
  for(i = 0; i < getNRows(mask); i++){
    for(j = 0; j < getNCols(mask); j++){
      int index = j + i * getNRows(mask);
      Gradient* g = (Gradient*) malloc(sizeof(Gradient));
      g->p = norm[index][0]/(norm[index][2]+0.1);
      g->q = norm[index][1]/(norm[index][2]+0.1);
      //printf("%.2f, %.2f\n", g->p, g->q);
      grid[index] = *g;
    }
  }
  FILE* fh2 = fopen(argv[10], "wb+");
  fwrite(grid,sizeof(Gradient), getNRows(mask)*getNCols(mask), fh2);
  fclose(fh2);

  ImageColor* normalMap = (ImageColor*) malloc(sizeof(ImageColor));
  setSizeColor(normalMap, getNRows(mask), getNCols(mask));
  setColorsColor(normalMap, 255);
  Image* albedo = (Image*) malloc(sizeof(Image));
  setSize(albedo, getNRows(mask), getNCols(mask));
  setColors(albedo, 255);

  int index, R,G,B;
  double* M;
  double mag;
  double maxMag = 0;
  for(i = 0; i < getNRowsColor(normalMap); i++)
    for(j = 0; j < getNColsColor(normalMap); j++){
      index = j + i*getNRowsColor(normalMap);
      M = norm[index];
      mag = sqrt(M[0]*M[0] + M[1]*M[1] + M[2]*M[2]);
      maxMag = (mag > maxMag) ? mag : maxMag;
      R = (int) (128 * M[0]/mag) + 127;
      G = (int) (128 * M[1]/mag) + 127;
      B = (int) (128 * M[2]/mag) + 127;
      setPixelColor(normalMap, i, j, R, G, B);
    }

  for(i = 0; i < getNRows(mask); i++)
    for(j = 0; j < getNCols(mask); j++){
      index = j + i*getNRows(mask);
      M = norm[index];
      mag = sqrt(M[0]*M[0] + M[1]*M[1] + M[2]*M[2])/maxMag * 255;
      setPixel(albedo,i,j,mag);
    }
  
  
  writeImageColor(normalMap, argv[8]);
  writeImage(albedo, argv[9]);
}
