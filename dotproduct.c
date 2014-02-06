/* 
   dotproduct.c

   compMatrixTimesVector computes the result of  matrix times vector. 

   The input matrix A is of size m x n, and it is stored as a
   one-dimensional array. The result is stored at y, which contains
   the computed y = A*b.
   
   1 February 2013
   Copyright 2013 Ned Nedialkov
*/
void compMatrixTimesVector(int m, int n, const double *A, 
		    const double *b, double *y) 
{
  int i, j;
  for (i = 0; i < m; i++) 
    {
      y[i] = 0.0;
      for (j = 0; j < n; j++) 
	{
	  const double *pA = A + i * n;
	  y[i] += *(pA + j) * b[j];
	}
    }
}  
