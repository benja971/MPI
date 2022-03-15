#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <mpi.h>

void show_tab(int size, int *tab)
{
    for (int i = 0; i < size; i++)
    {
        printf("%3d", tab[i]);
    }
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    long clk_tck = CLOCKS_PER_SEC;
    double start, end;
    clock_t t1, t2;

    srand(time(NULL));

    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long int tab_size = 99999999;
    int sub_size = tab_size / (size - 1);

    int *tab = (int *)malloc(tab_size * sizeof(int));
    if (tab == NULL)
    {
        printf("Error: malloc failed\n");
        exit(1);
    }

    for (long int i = 0; i < tab_size; i++)
        tab[i] = (rand() % 100) + 1;

    printf("tab rempli\n");

    start = MPI_Wtime();
    long long int sum = 0;

    for (int i = 2; i <= size; i++)
    {
        long int limit = sub_size * (i - 1);
        int *sub_tab = malloc(sub_size * sizeof(int));

        if (sub_tab == NULL)
        {
            exit(1);
            printf("Error: malloc failed\n");
        }

        long int k = 0;
        for (long int j = limit - sub_size; j < limit; j++)
        {
            sub_tab[k] = tab[j];
            k++;
        }

        printf("sous tableau rempli\n");

        long long int tmp = 0;

        MPI_Send(sub_tab, sub_size, MPI_INT, (i - 1), 50, MPI_COMM_WORLD);
        MPI_Recv(&tmp, 1, MPI_LONG_LONG_INT, (i - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        sum += tmp;
    }

    printf("sum = %lli\n", sum);

    end = MPI_Wtime();
    printf("Temps de calcul MPI pour %ld nombres: %f\n\n", tab_size, end - start);

    MPI_Finalize();

    t1 = clock();

    //    summ tab
    int sum_tab = 0;
    for (long int i = 0; i < tab_size; i++)
        sum_tab += tab[i];

    printf("sum_tab = %d\n", sum_tab);

    t2 = clock();

    printf("Temps de calcul C pour %li nombres: %f\n", tab_size, (double)(t2 - t1) / clk_tck);

    free(tab);

    return 0;
}
