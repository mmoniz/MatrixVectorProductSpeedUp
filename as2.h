void genMatrix(int, int, double*);
void genVector(int, double*);  
void getResult(int m, int n, const double *A, const double *b, const double *r);
void compMatrixTimesVector(int, int, const double*, const double*, double*);
void parallelMatrixTimesVector(int local_rows, int cols,
			       double *local_A, double *b, double *y, 
			       int root, int my_rank, int p, MPI_Comm comm);
void * Malloc(int num_elements, int el_size, MPI_Comm comm, int rank);
void calcNumsPerProcess(int N, int p, int *nums_per_process);
