#include"vision_utilities.h"
#include"util.h"
#include<stdio.h>
#include<stdlib.h>
Image* im;
int thresh;
int main(int nargin, char** argv){
  if(nargin != 3){
    printf("Usage: p1 <input img> <output file>");
    return -1;
  }
  
  FILE *file = fopen(argv[2], "w+");
  im = (Image*) malloc(sizeof(Image));
  readImage(im, argv[1]);
  int i, j;
  double x, y, count;
  for(i = 0; i < getNRows(im); i++){
    for(j = 0; j < getNCols(im); j++){
      if(getPixel(im, i, j)){
	count++;
	x+=i;
	y+=j;
      }
    }
  }
  int cx = (int) round(x/count);
  int cy = (int) round(y/count);
  int r = (int) round(sqrt(count / PI));
  fprintf(file, "%d %d %d\n", cx, cy, r);
  
  fclose(file);
  free(im);
}
