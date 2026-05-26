// Escriba utilizando OpenMP un algoritmo que calcule la suma de un arreglo
// de N = 5 × 10 a la 8 doubles. Compare la performance con la implementaci´on secuencial usando distintos
// n´umeros de hilos. Compare tambi´en con una versi´on paralela que usa un mutex para proteger la variable
// que lleva la suma.
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#define N 500000000
int main(int argc, char const *argv[])
{

    size_t n = N;
    double sum = 0.0;
    double *arr = NULL;
    arr = (double *)malloc(n * sizeof(double));
#pragma omp parallel for
    for (size_t i = 0; i < n; i++)
    {
        arr[i] = 1.0;
    }

    double t0 = omp_get_wtime();

#pragma omp parallel for reduction(+ : sum)
    for (size_t i = 0; i < n; i++)
    {
        sum += arr[i];
    }

    double t_final = omp_get_wtime() - t0;

    printf("sum : %f\n", sum);
    printf("el programa tardo  : %f\n", t_final);
    free(arr);
    return 0;
}
