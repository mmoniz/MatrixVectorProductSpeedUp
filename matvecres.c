/* 
   matvecres.c

   getResult checks if the computed result of a matrix times vector is
   correct.This result is passed through the array y, storing y = A*b.
   1 February 2013
   Copyright 2013 N. Nedialkov
*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <mpi.h>

#include "as2.h"


void getResult(int m, int n, const double *A, const double *b, const double *y) {
  double *r;
  int i;

  r = malloc(m * sizeof(double));
  compMatrixTimesVector(m, n, A, b, r);

  for (i = 0; i < m; i++) {
    if (fabs(r[i]-y[i]) > 1e-14) {
      printf("\n!!! Error in parallel computation\n");
      free(r);
      MPI_Finalize();
      exit(1);
    }
  }
  free(r);
  printf("\n*** Multiplication is correct \n");
}

double getSerialTime(int m, int n, const double *A, const double *b, const double *y) {
  double *r;
  int i;

  r = malloc(m * sizeof(double));
  
  double ts = MPI_Wtime();
  compMatrixTimesVector(m, n, A, b, r);
  ts = MPI_Wtime()-ts;
  
  free(r);
  return ts;
}
