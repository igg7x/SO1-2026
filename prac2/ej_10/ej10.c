#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <stdio.h>
#define N 6

int i = 0;
int j = N / 2;
int suma_arr = 0;

pthread_mutex_t mutex_suma = PTHREAD_MUTEX_INITIALIZER;

void *suma_parte1(void *arg)
{
    int *arr = (int *)arg;
    for (; i < j; i++)
    {
        pthread_mutex_lock(&mutex_suma);
        suma_arr += arr[i];
        pthread_mutex_unlock(&mutex_suma);
    }
    return NULL;
}
void *suma_parte2(void *arg)
{
    int *arr = (int *)arg;
    for (; j < N; j++)
    {
        // seccion critica
        
        // tomo el lock para asegurar que no haya condiciones de carrera al modificar la variable suma_arr
        pthread_mutex_lock(&mutex_suma);
        suma_arr += arr[j];
        pthread_mutex_unlock(&mutex_suma);
        // suelto el lock para que el otro hilo pueda modificar la variable suma_arr sin problemas

    }
    return NULL;
}

int main()
{

    int arr[6] = {10, 20, 30, 40, 50, 60};
    pthread_t t1, t2;
    pthread_create(&t1, NULL, suma_parte1, arr);
    pthread_create(&t2, NULL, suma_parte2, arr);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("La suma del arreglo es: %d\n", suma_arr);

    return 0;
}
