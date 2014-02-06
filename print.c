
#include <stdio.h>

void printA(int m, int n, double *A) {
  int i, j;
  
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++)
      printf("%4.1f ", A[i * n + j]);
    printf("\n");
  }
}
  
void printB(double *b, int n) {
  int i;
  
  printf("\n");
  for (i = 0; i < n; i++)
    printf("%4.1f ", b[i]);
  printf("\n");
}  


void print_v(double *r, int m) {
    int i;

	printf("\n");
	for (i = 0; i < m; i++)
		printf("%4.1f \n", r[i]);
	printf("\n");
}  

