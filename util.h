#include<stdio.h>
#include<stdlib.h>
typedef struct _matrix{
  double* data;
  int rows;
  int cols;
} Matrix;

Matrix* transpose(Matrix* m);
Matrix* inverse(Matrix* m);
Matrix* times(Matrix* m1, Matrix* m2);
Matrix* matrix(int rows, int cols);
void print(Matrix* m);
void setM(Matrix* m, int i, int j, double val);
double getM(Matrix* m, int i, int j);
