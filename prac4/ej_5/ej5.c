#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 1000

int A[N][N], B[N][N], C[N][N];
void mult(int A[N][N], int B[N][N], int C[N][N])
{
    int i, j, k;
#pragma omp parallel for private(j, k)
    for (i = 0; i < N; i++)
        for (k = 0; k < N; k++)
            for (j = 0; j < N; j++)
                C[i][j] += A[i][k] * B[k][j];
}

int main()
{
    int i, j;

    for (i = 0; i < N; i++)
    {

#pragma omp parallel for
        for (j = 0; j < N; j++)
        {
            A[i][j] = random() % 1000;
            B[j][i] = random() % 1000;
        }
    }
    double t0 = omp_get_wtime();
    mult(A, B, C);
    double timempo_mutex = omp_get_wtime() - t0;
    printf("tiempo mutl patrices %d*%d: %f\n", N, N, timempo_mutex);
    return 0;
}