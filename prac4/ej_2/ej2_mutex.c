#include <pthread.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define N 500000000
double sum = 0.0;

int main(int argc, char const *argv[])
{

    size_t n = N;
    double *arr = NULL;
    arr = (double *)malloc(n * sizeof(double));
#pragma omp parallel for
    for (size_t i = 0; i < n; i++)
    {
        arr[i] = 1.0;
    }

    omp_lock_t lock;
    omp_init_lock(&lock);
    double sum_mutex = 0.0;

    double t0 = omp_get_wtime();

#pragma omp parallel for
    for (long i = 0; i < N; i++)
    {
        omp_set_lock(&lock);
        sum_mutex += arr[i];
        omp_unset_lock(&lock);
    }

    double timempo_mutex = omp_get_wtime() - t0;
    omp_destroy_lock(&lock);
    printf("tiempo con el mutex: %f\n", timempo_mutex);
    free(arr);
    return 0;
}
