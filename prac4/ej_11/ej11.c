#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 8

int main(int argc, char **argv)
{
    int my_id, numprocesses;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocesses);

    int (*A)[N] = NULL;
    int v[N];

    /* Cantidad de filas para cada proceso */
    int sendcounts[numprocesses];
    int displs[numprocesses];

    int filas_base = N / numprocesses;
    int extra = N % numprocesses;

    int offset = 0;

    for (int p = 0; p < numprocesses; p++)
    {
        int filas = filas_base + (p < extra ? 1 : 0);

        sendcounts[p] = filas * N; /* cantidad de enteros */
        displs[p] = offset;

        offset += sendcounts[p];
    }

    int local_rows = sendcounts[my_id] / N;

    int *A_local = malloc(sendcounts[my_id] * sizeof(int));
    int *x_local = malloc(local_rows * sizeof(int));

    /* Root crea matriz y vector */
    if (my_id == 0)
    {
        A = malloc(sizeof(int[N][N]));

        srand(1234);

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                A[i][j] = rand() % 10;
        }

        for (int i = 0; i < N; i++)
            v[i] = rand() % 10;

        printf("Matriz A:\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%3d ", A[i][j]);
            printf("\n");
        }

        printf("\nVector v:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", v[i]);
        printf("\n\n");
    }

    /* Todos reciben el vector */
    MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);

    /* Repartir filas de A */
    MPI_Scatterv(
        A,
        sendcounts,
        displs,
        MPI_INT,
        A_local,
        sendcounts[my_id],
        MPI_INT,
        0,
        MPI_COMM_WORLD);

    /* Producto matriz-vector local */
    for (int i = 0; i < local_rows; i++)
    {
        x_local[i] = 0;

        for (int j = 0; j < N; j++)
        {
            x_local[i] += A_local[i * N + j] * v[j];
        }
    }

    /* Preparar recepción en root */
    int *x = NULL;

    int recvcounts[numprocesses];
    int recvdispls[numprocesses];

    offset = 0;

    for (int p = 0; p < numprocesses; p++)
    {
        recvcounts[p] = sendcounts[p] / N; /* filas */
        recvdispls[p] = offset;

        offset += recvcounts[p];
    }

    if (my_id == 0)
        x = malloc(N * sizeof(int));

    MPI_Gatherv(
        x_local,
        local_rows,
        MPI_INT,
        x,
        recvcounts,
        recvdispls,
        MPI_INT,
        0,
        MPI_COMM_WORLD);

    if (my_id == 0)
    {
        printf("\nResultado x = A*v:\n");

        for (int i = 0; i < N; i++)
            printf("x[%d] = %d\n", i, x[i]);

        free(A);
        free(x);
    }

    free(A_local);
    free(x_local);

    MPI_Finalize();
    return 0;
}