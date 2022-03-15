#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Rank: %d/%d\n", rank, size);

    int tab_size = 99999999;
    int sub_tab_size = tab_size / (size - 1);

    int *tab = malloc(sizeof(int) * sub_tab_size);

    MPI_Recv(tab, sub_tab_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int sum = 0;
    for (int i = 0; i < sub_tab_size; i++)
        sum += tab[i];

    MPI_Request request;

    MPI_Isend(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);

    MPI_Finalize();

    return 0;
}