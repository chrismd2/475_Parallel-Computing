#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"


int main(int argc, char *argv[])
{
	int npes, myrank;
  int totalElements = 10000;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&npes);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

	printf("Process %d out of %d started\n", myrank, npes);

  //Open file
  FILE *data;
  data = fopen("Data.csv", "r");
  //Compute my position = (my rank * total data elements) / number of processes
  int myPosition = 4 * (myrank * totalElements)/npes;

  printf("%d positioned at %d\n", myrank, myPosition);
  //Go to my position
  fseek(data, myPosition, 0);
  char currentChar = getc(data);
  //printf("%d found %c", myrank, currentChar);
  //consider trying to find the previous comma if my rank != 0
  //read numbers
  char str[] = "";
  int best = 1000;
  for(int i = 0; i < 4*(totalElements/npes); i++){
    int tBest = 0;
    while(currentChar!=',' && currentChar!=EOF){
      tBest = 10 * tBest + (currentChar - '0');
      //printf("%c", currentChar);
      currentChar = getc(data);
    }
    //printf("\n%d current number is %d", myrank, tBest);
    if(best > tBest && tBest > 0){best = tBest;}
  }
  printf("\n%d final number is %d\n", myrank, best);

  int size, grp_size;
  int *buf;

  MPI_Barrier(MPI_COMM_WORLD);

  if (myrank == 0){
    MPI_Comm_size(MPI_COMM_WORLD, &grp_size);
    size = grp_size*sizeof(int);
    buf = (int *)malloc(size);
  }

  int mBest[npes];
  mBest[0] = best;
  MPI_Gather(mBest, 1, MPI_INT, buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if(myrank == 0){
    for (size_t i = 0; i < npes; i++) {
      printf("%d: %d\n", i, buf[i]);
      if(best>buf[i]){best=buf[i];}
    }
    printf("Best is %d\n", best);
  }

	MPI_Finalize();

  //close(data);
	return 0;
}
