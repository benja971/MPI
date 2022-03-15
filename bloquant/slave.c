#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long int tab_size = 99999999;
    long int sub_size = tab_size / (size - 1);

    int *buf = malloc(sub_size * sizeof(int));

    MPI_Recv(buf, sub_size, MPI_INT, 0, 50, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    long long int sum = 0;

    for (long int i = 0; i < sub_size; i++)
        sum += buf[i];

    MPI_Send(&sum, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);

    free(buf);
    MPI_Finalize();

    return 0;
}