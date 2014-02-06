/* common.c 

   Contains functions Malloc and calcNumsPerProcess. 
   Malloc allocates memory through malloc and aborts MPI, if malloc
   fails
   calcNumsPerProcess calculates how many "numbers" to be 
   allocated to a process.
   1 February 2013
   Copyright 2013 Ned Nedialkov
*/
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"


void * Malloc(int num_elements, int el_size, MPI_Comm comm, int rank)
{
  void *b = malloc(num_elements*el_size);
  if (b==NULL)
    {
      fprintf(stderr, "*** PROCESS %d: MALLOC COULD NOT ALLOCATE %d ELEMENTS OF SIZE %d BYTES\n", 
	      rank, num_elements, el_size);
      MPI_Abort( comm, 1);      
    }
  return b;
}

void calcNumsPerProcess(int N, int p, int *nums_per_node)
{
  int i;
  for (i = 0; i < p; i++) 
    nums_per_node[i] = N/p;
  
  for (i = 0; i < N % p; i++) 
    nums_per_node[i]++;
}



  
