#include<stdlib.h>
#include<stdio.h>
#include "mpi.h"

#define N 12

int main(int argc, char *argv[])
{
	int myrank;
 	int i, size, grp_size;
	int* buf; /*receive buffer*/
	int data[N][N];	/*data to be distribued from the root process*/
	int recv_count;
	int myresult = 0;
	int result;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &grp_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	/* find rank */

	if(myrank == 0)
	{
    for(i=0;i<N; i++){
      for(int j = 0; j < N; j++){
        data[i][j]=(i+1)*(j+1);
        printf("%d\t", data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
	}

	recv_count = N*N/grp_size;
	buf = (int* )malloc(recv_count*sizeof(int));

	MPI_Scatter(data,recv_count,MPI_INT,buf,recv_count, MPI_INT,0,MPI_COMM_WORLD);

	printf("The data received by process %d\n", myrank);
	for (i = 0; i<recv_count; i++)
		printf("%d ", buf[i]);
	printf("\n");

	for (i = 0; i < recv_count; ++i)
		myresult += buf[i];

 	printf("Local sum computed by process %d --> %d", myrank, myresult);
	printf("\n");

	/* Compute global sum */
	MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (myrank == 0)
		printf("The total sum is %d.\n", result);
	printf("\n");

	MPI_Finalize();

	return 0;
}
