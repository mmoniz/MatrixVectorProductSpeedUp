/* genmatvec.c
   genMatrix generates a random n*m matrix
   genVector generates a random n vector
   1 February 2013
   Copyright 2013 Ned Nedialkov 
*/
#include <stdlib.h>

void genMatrix(int m, int n, double *A) {  
  int i, j; 
  for (i = 0; i < m; i++)  
    for (j = 0; j < n; j++) {
      A[i * n + j] = drand48()*rand();
    }
}   

void genVector(int n, double *b) { 
  int i; 
  for (i = 0; i < n; i++)  
    b[i] = drand48()*rand();
}   
