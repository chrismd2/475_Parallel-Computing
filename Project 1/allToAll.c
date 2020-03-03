#include<stdlib.h>
#include<stdio.h>
#include "mpi.h"

#define N 4 //total amount of data to distribute to all processes
//#define SEND_COUNT 3  //amount of data to send to each process

int main(int argc, char *argv[])
{
	int myrank;
 	int i, size, grp_size;
	int* buf;
 	int data[N];  // data to distribute
	int sendcount;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	/* find rank */
	MPI_Comm_size(MPI_COMM_WORLD, &grp_size); /*find group size*/

	for(i=0;i<N; i++)
		data[i]= myrank*N + i;

	//print my local data
	printf("Data at process %d: \n", myrank);
	for(i=0;i<N; i++)
		printf("%d ", data[i]);
	printf("\n");

	sendcount = N/grp_size;
	size=sendcount*grp_size*sizeof(int);
        buf = (int *)malloc(size); /*allocate receving memory*/

	MPI_Alltoall(data,sendcount,MPI_INT,buf,sendcount,MPI_INT,MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
	printf("The received data by process %d\n", myrank);
	for(i=0; i<sendcount*grp_size; i++)
//		printf("\n from rank %d: buf[%d] = %d", myrank, i, buf[i]);
		printf("%d ", buf[i]);

	printf("\n");

	MPI_Finalize();

	return 0;
}
