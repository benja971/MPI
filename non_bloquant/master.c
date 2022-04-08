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

    double start = MPI_Wtime();

    srand(time(NULL));

    printf("Rank: %d/%d\n", rank, size);

    long int tab_size = 99999999;
    int sub_tab_size = tab_size / (size - 1);

    MPI_Request request;

    int sum = 0;

    int *tab = malloc(sizeof(int) * tab_size);
    for (int i = 0; i < tab_size; i++)
        tab[i] = (rand() % 100) + 1;

    for (long int i = 1; i < size; i++)
    {
        int limit = tab_size / (size - 1) * i;
        int debut = limit - sub_tab_size;

        printf("debut: %d\n", debut);
        printf("limit: %d\n", limit);
        int *sub_tab = malloc(sizeof(int) * sub_tab_size);

        int l = 0;
        for (int j = debut; j < limit; j++)
        {
            // printf("tab[%d]: %d\n", j, tab[j]);
            sub_tab[l] = tab[j];
            l++;
        }

        int tmp;
        MPI_Isend(sub_tab, sub_tab_size, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
        // MPI_Wait(&request, MPI_STATUS_IGNORE);
        MPI_Irecv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
        sum += tmp;
    }

    printf("Sum: %d\n", sum);

    double end = MPI_Wtime();

    printf("Temps de calcul MPI pour %ld nombres: %f\n\n", tab_size, end - start);

    MPI_Finalize();

    return 0;
}