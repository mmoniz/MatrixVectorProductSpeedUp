#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "as1.h"

main(int argc, char* argv[]) {
  double *part_A; 
  double *A; 
  double *b;
  double *y;
  int rank, p, m, n, i ,j, local_m;
  int offseti, size, rem;
  MPI_Status *status;
  
  m = atoi(argv[1]);
  n = atoi(argv[2]);
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  // Find how many rows per process.
  local_m = m/p;  
  rem = m % p; 
  if (rank < rem) {
    local_m++;
  }
  
  // Allocate memory.
  b = malloc( n*sizeof(double));
  if (rank == 0) {
      A = malloc(m * n * sizeof(double));
      y = malloc(m * sizeof(double));
  }
  else {
      A = malloc(local_m * n * sizeof(double));
      y = malloc(local_m * sizeof(double));
  }	  

  // Generate matrix and vector
  if (rank == 0) { 
      genMatrix(m, n, A);
      genVector(n, b);
  }
  
  // Distribute the data
  if (rank == 0) {
	offset = 0;
    for (i = 1; i < p; i++) {
	  if (i - 1 < rem) {
		offset += m/p + 1;
	  } else {
	  	offset += m/p;
	  }
	  if (i < rem) {
		size = m/p + 1;
	  } else {
	  	size = m/p;
	  }
      MPI_Send(A + offset, size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
  	}
  } else {
    MPI_Recv(A, local_m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
  }

  MPI_Bcast(b, n, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
  
  compMatrixTimesVector(local_m, n, A, b, y);
  
  // Distribute the data
  if (rank > 0) {
    MPI_Send(y, local_m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  } else {
    for (i = 1; i < p; i++) {
      if (i - 1 < rem) {
		offset += m/p + 1;
	  } else {
	  	offset += m/p;
	  }
	  if (i < rem) {
		size = m/p + 1;
	  } else {
	  	size = m/p;
	  }
      // compute offset
      MPI_Recv(y + offset, size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
    }
  }

  if (rank == 0) {
    getResult(m, n, A, b, y);
  }
  
  MPI_Finalize();
}  


void getResult(int m, int n, double *A, double *b, double *y) {
  // allocate buffer, r
  compDorProduct(m,n, A, b, r);
  for (i = 0; i < m*n; i++) {
    if (fabs(r[i]-y[i]) > 1e-14) {
		printf("Error in Parallel computation");
		exit(1);
    }
  }	
}

void compMatrixTimesVector(int m, int n, double *A, double *b, double *y) {
  int i, j;

  for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++) {
	  double *pA = A+i*n;
	  part_y[i] += *(pA+j) * b[j];
	}
  }
}  
  
void printMatrix(int m, int n, double *A) {
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
