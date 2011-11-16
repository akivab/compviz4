#include"vision_utilities.h"
#include"util.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
Image* mask;

int main(int nargin, char** argv){
  if(nargin != 5){
    printf("Usage: p5 <input gradient> <seed point> <mask> <output depth>");
    return -1;
  }
  
  int index, i,j;
  mask = (Image*) malloc(sizeof(Image));
  readImage(mask, argv[3]);
  
  FILE* file = fopen(argv[1], "rb");
  FILE* seeds = fopen(argv[2], "r");
  Gradient* grid = (Gradient*) malloc(sizeof(Gradient)*getNRows(mask)*getNCols(mask));
  fread(grid, getNRows(mask)*getNCols(mask), sizeof(Gradient), file);
  double** points = (double**) malloc(sizeof(double)*getNRows(mask));
  for(i = 0; i < getNRows(mask); i++) points[i] = (double*)malloc(sizeof(double)*getNCols(mask));
  double* allpoints = (double*) malloc(sizeof(double)*getNRows(mask)*getNCols(mask));

  char line[1024];
  int r = getNRows(mask), c = getNCols(mask);
  int max = 0;
  while(fgets(line, 1024, seeds) != NULL){
    int x0 = atoi(strtok(line, " "));
    int y0 = atoi(strtok(NULL, " "));
    for(i = 0; i < getNRows(mask); i++)
      for(j = 0; j < getNCols(mask); j++) 
	points[i][j] = 0;
    int x, y;
    for(x = x0-1; x >=0; x--)
      points[x][y0] = (points[x+1][y0] + (grid+x*r+y0)->p);
    for(x = x0+1; x < r; x++)
      points[x][y0] = (points[x-1][y0] - (grid+x*r+y0)->p);
    for(y = y0-1; y >=0; y--)
      points[x0][y] = (points[x0][y+1] + (grid+x0*r+y)->q);
    for(y = y0+1; y < r; y++)
      points[x0][y] = (points[x0][y-1] - (grid+x0*r+y)->q);
    
    for(x = x0-1; x >=0; x--){
      for(y = y0-1; y >=0; y--)
	points[x][y] = (0.5 * points[x][y+1] + (grid+x*r+y)->q +
			0.5 * points[x+1][y] + (grid+x*r+y)->p);
      for(y = y0+1; y < r; y++)
	points[x][y] = (0.5 * points[x][y-1] - (grid+x*r+y)->q +
			0.5 * points[x+1][y] + (grid+x*r+y)->p);
    }
    for(x = x0+1; x < r; x++){
      for(y = y0-1; y >=0; y--)
	points[x][y] = (0.5 * points[x][y+1] + (grid+x*r+y)->q +
			0.5 * points[x-1][y] - (grid+x*r+y)->p);
      for(y = y0+1; y < r; y++)
	points[x][y] = (0.5 * points[x][y-1] - (grid+x*r+y)->q +
			0.5 * points[x-1][y] - (grid+x*r+y)->p);
    }
    for(x = 0; x < r; x++)
      for(y = 0; y < r; y++){
	allpoints[x*r+y] += points[x][y];
	max = (allpoints[x*r+y] > max) ? allpoints[x*r+y] : max;
      }
  }
  Image* im = (Image*) malloc(sizeof(Image));
  setSize(im, r, c);
  setColors(im, 255);
  int x,y;
  for(x = 0; x < r; x++)
    for(y = 0; y < c; y++)
      setPixel(im, x, y, getPixel(mask,x,y) ? allpoints[x*r+y] / (double) max * 255 : 0);
  writeImage(im, argv[4]);
}
