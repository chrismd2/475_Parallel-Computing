#include<stdlib.h>
#include<stdio.h>
#include "mpi.h"


int main(int argc, char *argv[])
{
  int N, npes;
	int myrank;
 	int i, size, grp_size;
	int* buf; /*receive buffer*/
	int recv_count;
	int myresult = 0;
	int result;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_size(MPI_COMM_WORLD, &grp_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	/* find rank */

  N = npes;
	int A[64][64];	/*data to be distribued from the root process*/
  int b[64];

	if(myrank == 0)
	{
    //open file and place it into 2d array
    FILE *data;
    data = fopen("Data2.csv", "r");
    char currentChar = getc(data);
    char str[] = "";
    for(i=0;i<64; i++){
      for(int j=0; j<64; j++){
        A[i][j] = 0;
        while(currentChar!=',' && currentChar!=EOF){
          A[i][j] = 10 * A[i][j] + (currentChar - '0');
          //printf("A[%d][%d]: %d\n", i, j, A[i][j]);
          currentChar = getc(data);
        }//end of searching for end of number
        currentChar = getc(data);
        //printf("currentChar: %c\n", currentChar);
        //printf("\n");
      }//end of filling current row
      //printf("\n");
    }//end of filling matrix
    //printf("\n");
	}//end of initialization by P0

  /*
  for (size_t i = 0; i < 64; i++) {
    for (size_t j = 0; j < 64; j++) {
      printf("%d ", A[i][j]);
    }//end print row
    printf("\n");
  }//end print matrix
  printf("\n");
  */

	recv_count = 64*64/npes;
	buf = (int* )malloc(recv_count*sizeof(int));

	MPI_Scatter(A,recv_count,MPI_INT,buf,recv_count, MPI_INT,0,MPI_COMM_WORLD);

	printf("The data received by process %d\n", myrank);
	for (i = 0; i<recv_count; i++)
		printf("%d ", buf[i]);
	printf("\n");

	MPI_Finalize();

	return 0;
}//end main
