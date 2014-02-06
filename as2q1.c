/* as1.c 
   Main program for the A1 solution
   1 February 2012

   Copyright 2012 N. Nedialkov
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

#include "as2.h"

int main(int argc, char* argv[]) 
{
  double *A, *B, *b, *y;
  int n;
  
  int my_rank, p;
  int i;
  
  /* Obtain number of rows and columns. We do not check for eroneous
     input. */
  n = atoi(argv[1]);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  
  /* Find how many rows per process. */
  int *rows;
  rows = (int*)Malloc(p, sizeof(int), MPI_COMM_WORLD, my_rank);
  calcNumsPerProcess(n, p, rows);
  
  /* Allocate memory. */
  b = Malloc(n, sizeof(double), MPI_COMM_WORLD, my_rank);
  if (my_rank == 0) 
    {
      A = (double*)Malloc(n*n, sizeof(double), MPI_COMM_WORLD, my_rank);
      y = (double*)Malloc(n, sizeof(double), MPI_COMM_WORLD, my_rank);
    } 
  B = (double*)Malloc(rows[my_rank]*n, sizeof(double), MPI_COMM_WORLD, my_rank);
  
  /* Generate matrix and vector */
  if (my_rank == 0) 
    { 
      genMatrix(n, n, A);
      genVector(n, b);
    }
  
  /* Distribute A */
  int *displs;
  int *sendcounts;
  if (my_rank == 0)
    {
      displs = malloc(sizeof(int)*p);
      sendcounts = malloc(sizeof(int)*p);
      for (i=0; i<p; i++)
	sendcounts[i] = rows[i]*n;
      displs[0] = 0;
      for (i=1; i<p; i++)
	displs[i] = displs[i-1] + sendcounts[i-1];
    }

  MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE, 
	       B, rows[my_rank]*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  /* Distribute b */
  MPI_Bcast(b, n, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
  
  double time = MPI_Wtime();
  parallelMatrixTimesVector(rows[my_rank], n, B, b, y, 0, my_rank, p, MPI_COMM_WORLD);
  time = MPI_Wtime()-time;
  
  /* Collect the max time from all processes. */
  double timerecv;

  MPI_Reduce(&time,&timerecv, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (my_rank==0) {
    printf("%d %d % .2e\n", p, n, timerecv);
  }
  
  if (my_rank==0){
    free(sendcounts);
    free(displs);
    free(y);
  }
  free(A);
  free(b);
  free(rows);
  
  MPI_Finalize();
  return 0;
}  



void parallelMatrixTimesVector(int local_rows, int cols,
			       double *local_A, double *b, double *y, 
			       int root, int my_rank, int p, MPI_Comm comm)
{
  /*
    This function performs parallel matrix-vector multiplication of a
    matrix A times vector b.  The matrix is distributed by rows. Each
    process contains (local_rows)x(cols) matrix local_A stored as a
    one-dimensional array.  The vector b is stored on each process.
    Each process computes its result and then process root
    collects the resutls and returns it in y.

    local_rows is the number of rows on my_rank
    cols       is the number of columns on each process
    local_A    is a pointer to the matrix on my_rank
    b          is a pointer to the vector b of size cols
    y          is a pointer to the result on the root process. 
               y is significant only on root.
  */

  double *local_y = malloc(sizeof(double)*local_rows);
  /* Compute the local matrix times vector */
  compMatrixTimesVector(local_rows, cols, local_A, b, local_y);
  int sendcount = local_rows; /* number of doubles sent by process my_rank  */
  int *reccounts; /* reccounts[i] is the number of doubles received from process i */
  int *displs;    /* displs for the MPI_Gatherv function */

  if (my_rank != root)
    {
      /* Send the sendcounts to the root process. reccounts does not matter here. */
      MPI_Gather(&sendcount, 1, MPI_INT, reccounts, 1, MPI_INT, root, comm);
      /* Send the computed results to the root process. The receive
	 buffer, reccounts, and displs do not matter here. */
      MPI_Gatherv(local_y, sendcount, MPI_DOUBLE, 
		  y, reccounts, displs, MPI_DOUBLE, root, comm);
    }
  else /* we are on root process */
    {
      /* Gatter the receive counts from each process */
      reccounts = malloc(sizeof(int)*p);
      MPI_Gather(&sendcount, 1, MPI_INT, reccounts, 1, MPI_INT, 0, comm);

      /* Calculate displs for MPI_Gatterv */
      displs = malloc(sizeof(int)*p);
      int i;
      displs[0] = 0;
      for (i = 1; i < p; i++) 
	displs[i] = displs[i-1] + reccounts[i-1];
      
      /* Gather the results on process 0 */
      MPI_Gatherv(local_y, sendcount, MPI_DOUBLE, 
		  y, reccounts, displs, MPI_DOUBLE, root, comm);
      
      free(displs);
      free(reccounts);
    }
  free(local_y);
}  
