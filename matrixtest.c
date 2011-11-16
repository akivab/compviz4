#include"util.h"

int main(){
  Matrix* m1 = matrix(3,3);
  int i;
  for(i=0; i< 9; i++){
    m1->data[i] = i*i+5*i+1;
  }
  print(m1);
  Matrix* m2 = inverse(m1);
  print(m2);
  print(times(m2,m1));
}
