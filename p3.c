#include"vision_utilities.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
Image* im;
Image* tmp;

int main(int nargin, char** argv){
  if(nargin != 7){
    printf("Usage: p3 <image 1> <image 2> <image 3> <image 4> <image 5> <output mask>");
    return -1;
  }
  
  FILE *file = fopen(argv[6], "w+");
  
  im = (Image*) malloc(sizeof(Image));
  int i, j, k;
  for(k = 0; k < 5; k++){
    tmp = (Image*) malloc(sizeof(Image));
    if( k == 0 ){
      readImage(im, argv[k+1]);
    } else {
      readImage(tmp, argv[k+1]);
      for(i = 0; i < getNRows(tmp); i++)
	for(j = 0; j < getNCols(tmp); j++)
	  if(getPixel(tmp,i,j))
	    setPixel(im, i, j, 1);
      free(tmp);
    }
  }
  setColors(im, 1);
  for(i = 0; i < getNRows(im); i++)
    for(j = 0; j < getNCols(im); j++)
      setPixel(im, i, j, getPixel(im, i, j) > 0);
  writeImage(im, argv[6]);
  fclose(file);
  free(im);
}
