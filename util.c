#include"util.h"
Matrix* inverse(Matrix* m){
  if(m->rows != m->cols)
    return NULL;
  Matrix* tmp = matrix(m->rows, m->cols * 2);
  int i,j,k;
  for(i = 0; i < tmp->rows; i++)
    for(j = 0; j < m->cols; j++){
      setM(tmp, i, j, getM(m, i, j));
      if(i == j)
	setM(tmp, i, j+m->cols, 1);
    }

  for(i = 0; i < tmp->rows; i++){
    double pivot = getM(tmp, i, i);
    for(j = 0; j < tmp->cols; j++)
      setM(tmp, i, j, getM(tmp, i, j)/pivot);
    for(j=i+1; j < tmp->rows; j++){
      pivot = getM(tmp, j, i);
      for(k=0; k < tmp->cols; k++)
	setM(tmp, j, k, getM(tmp, j, k) - getM(tmp, i, k) * pivot);
    }
  }

  for(i = tmp->rows-1; i >=0; i--){
    double pivot = getM(tmp, i, i);
    for(j = 0; j < tmp->cols; j++)
      setM(tmp, i, j, getM(tmp, i, j)/pivot);
    for(j=i-1; j >= 0; j--){
      pivot = getM(tmp, j, i);
      for(k=0; k < tmp->cols; k++)
	setM(tmp, j, k, getM(tmp, j, k) - getM(tmp, i, k) * pivot);
    }
  }

  Matrix* toreturn = matrix(tmp->rows, tmp->rows);
  for(i = 0; i < tmp->rows; i++)
    for(j = 0; j < tmp->rows; j++)
      setM(toreturn,i,j,getM(tmp,i,j+tmp->rows));
  free(tmp);
  return toreturn;
  
}

void print(Matrix* m){
  int i, j;
  char toprint[1024];
  toprint[0] = 0;
  for(i = 0; i < m->rows; i++){
    for(j = 0; j < m->cols; j++)
      sprintf(toprint, "%s%.1f ", toprint, getM(m, i, j));
    sprintf(toprint, "%s\n", toprint);
  }
  printf("%s", toprint);
}

void setM(Matrix* m, int i, int j, double k){
  int rows = m->rows;
  int cols = m->cols;
  double* data = m->data;
  if(i >= 0 && i < rows && j >= 0 && j < cols) 
    data[i+rows*j] = k;
}

double getM(Matrix* m, int i, int j){
  if(i >= 0 && i < m->rows && j >= 0 && j < m->cols)
    return m->data[i+(m->rows)*j];
}

Matrix* matrix(int nRows, int nCols){
  if(nRows <= 0 || nCols <= 0)
    return NULL;
  Matrix* toreturn = (Matrix*) malloc(sizeof(Matrix));
  toreturn->rows = nRows;
  toreturn->cols = nCols;
  toreturn->data = (double*)malloc(sizeof(double) * nRows * nCols);
  return toreturn;
}

Matrix* transpose(Matrix* m){
  Matrix* toreturn = matrix(m->cols, m->rows);
  int i, j;
  for(i = 0; i < m->rows; i++)
    for(j = 0; j < m->cols; j++)
      setM(toreturn, j, i, getM(m, i, j));
  return toreturn;
}

Matrix* times(Matrix* m1, Matrix* m2){
  if(m1->cols != m2->rows)
    return NULL;
  int i,j,k;
  Matrix* toreturn = matrix(m1->rows, m2->cols);
  for(i = 0; i < m1->rows; i++)
    for(j = 0; j < m2->cols; j++)
      for(k = 0; k < m1->cols; k++)
	setM(toreturn, i, j, getM(toreturn, i, j) + getM(m1, i, k) * getM(m2, k, j));
  return toreturn;
}
