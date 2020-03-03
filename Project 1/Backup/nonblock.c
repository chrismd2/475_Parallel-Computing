//This example demonstrates nonblocking routines.
#include<stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
        int myrank;
        int rc;
        MPI_Request request;

        MPI_Init(&argc,&argv);

        MPI_Comm_rank(MPI_COMM_WORLD,&myrank); /* find rank */

        if (myrank == 0) {
                int x;
                x = 100;
                int msgtag=0;
                int flag;
                MPI_Status status;
                MPI_Isend(&x, 1, MPI_INT, 1, msgtag, MPI_COMM_WORLD, &request);
                MPI_Test(&request, &flag, &status);
                while(!flag)
                        MPI_Test(&request, &flag, &status);
                x = x + 1;
                printf("From proces %d: x is changed to %d \n", myrank, x);
        } else if (myrank == 1) {
                int y;
                int msgtag=0;
                MPI_Status status;
                int c;
                MPI_Irecv(&y, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &request);
                MPI_Wait(&request, &status);
                printf("the message is received from %d \n", status.MPI_SOURCE);
                MPI_Get_count(&status, MPI_INT, &c);
                printf("the message has %d integers: %d \n", c, y);

        }

        MPI_Finalize();

	return 0;
}
