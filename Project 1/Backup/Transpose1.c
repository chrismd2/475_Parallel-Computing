#include<stdlib.h>
#include<stdio.h>
#include "mpi.h"


int main(int argc, char *argv[])
{
  int rows = 4;
  int N, npes;
	int myrank;
 	int i, size, grp_size;
	int* buf; /*receive buffer*/
  int* vbuf;
	int recv_count;
	int myresult = 0;
	int result;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_size(MPI_COMM_WORLD, &grp_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	/* find rank */

  N = npes;
	int A[rows][rows];	/*data to be distribued from the root process*/
  int b[rows];

	if(myrank == 0)
	{
    //open file and place it into 2d array
    FILE *data;
    data = fopen("Data2.csv", "r");
    char currentChar = getc(data);
    char str[] = "";
    for(i=0;i<rows; i++){
      for(int j=0; j<rows; j++){
        A[i][j] = 0;
        while(currentChar!=',' && currentChar!=EOF){
          A[i][j] = 10 * A[i][j] + (currentChar - '0');
          currentChar = getc(data);
        }//end of searching for end of number
        currentChar = getc(data);
      }//end of filling current row
    }//end of filling matrix

    for(int i = 0; i < rows; i++){
      b[i] = i + 1;
    }//end filling vector b

    //reorder A
    int tA[rows][rows];
    for (size_t i = 0; i < npes; i++) {
      int s = 2 * rows - i % (rows/2);
      for (size_t j = 0; j < 2*rows/npes; j++) {
        tA[i][j] = A[i][s+j];
      }
      for (size_t j = 2*rows/npes; j < rows; j++) {
        tA[i][j] = A[i][s+rows+j];
      }
    }

    printf("alternate order complete\n");
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < rows; j++) {
        printf("%d ", tA[i][j]);
      }
      printf("\n");
    }
	}//end of initialization by P0

	recv_count = rows*rows/npes;
	buf = (int* )malloc(recv_count*sizeof(int));
  vbuf = (int* )malloc(recv_count*sizeof(int));

  MPI_Scatter(A,recv_count,MPI_INT,buf,recv_count, MPI_INT,0,MPI_COMM_WORLD);

	printf("The data from matrix A received by process %d\n", myrank);
	for (i = 0; i<recv_count; i++)
		printf("%d ", buf[i]);
	printf("\n\n");

  MPI_Request request;

  if(myrank == 0){
    int flag;
    int tbuf;
    MPI_Status status;
    MPI_Irecv(&tbuf, 1, MPI_INT, MPI_ANY_TAG, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    printf("%d waited\n", myrank);
  }//P0 sent message to continue
  if(myrank > 0){
    int flag;
    MPI_Status status;
    MPI_Isend(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    MPI_Test(&request, &flag, &status);
    printf("%d going to wait\n", myrank);
    while (!flag) {
      MPI_Test(&request, &flag, &status);
    }
    printf("%d done waiting\n", myrank);
  }//other proceses wait for P0

  MPI_Scatter(b,rows/npes,MPI_INT,vbuf,rows/npes, MPI_INT,0,MPI_COMM_WORLD);

  printf("The data recieved from vector b recieved by process %d\n", myrank);
  for (size_t i = 0; i < rows/npes; i++) {
    printf("%d ", vbuf[i]);
  }//end of printing vector b
  printf("\n");

	MPI_Finalize();

	return 0;
}//end main
