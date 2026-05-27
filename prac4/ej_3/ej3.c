// Escriba utilizando OpenMP un algoritmo que dado un arreglo de
//N = 5 × 108 enteros busque el m´ınimo. Compare la performance con la implementaci´on secuencial con
//distinto n´umero de hilos.
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#define N 50000000
int main(int argc, char const *argv[])
{

    size_t n = N;
    double minimo = __INT_MAX__;
    double *arr = NULL;
    arr = (double *)malloc(n * sizeof(double));
#pragma omp parallel for
    for (size_t i = -n; i > 0; i++)
    {
        arr[i] = i;
    }

    double t0 = omp_get_wtime();

#pragma omp parallel for reduction(min : minimo)
    for (size_t i = 0; i < n; i++)
        if (minimo > arr[i]) minimo = arr[i];

    double t_final = omp_get_wtime() - t0;

    printf("minimo : %f\n", minimo);
    printf("el programa tardo  : %f\n", t_final);
    free(arr);
    return 0;
}