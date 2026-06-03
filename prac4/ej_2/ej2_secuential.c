/*
 * sum_sequential.c
 * Suma secuencial de N = 5e8 doubles
 * Compilar: gcc -O2 -o sum_sequential sum_sequential.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 500000000 /* 5 × 10^8 */

int main(void)
{
    printf("Allocating %.2f GB...\n", (double)N * sizeof(double) / 1e9);

    double *arr = (double *)malloc(N * sizeof(double));
    if (!arr)
    {
        perror("malloc");
        return 1;
    }

    /* Inicializar arreglo */
    for (long i = 0; i < N; i++)
        arr[i] = 1.0 / (i + 1.0);

    /* Medir tiempo */
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    double sum = 0.0;
    for (long i = 0; i < N; i++)
        sum += arr[i];

    clock_gettime(CLOCK_MONOTONIC, &t1);

    double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) * 1e-9;

    printf("SEQUENTIAL | N=%d | sum=%.6f | time=%.4f s\n", N, sum, elapsed);

    free(arr);
    return 0;
}
