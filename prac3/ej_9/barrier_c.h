#ifndef BARRIER_C
#define BARRIER_C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct barrier
{
    int count;             // Número de hilos que han llegado a la barrera
    int num_threads;       // Número total de hilos que deben llegar a la barrera
    pthread_mutex_t mutex; // Mutex para proteger el acceso a la variable count
    pthread_cond_t cond;   // Variable de condición para bloquear los hilos hasta que todos lleguen a la barrera
    int generation;        // Generación actual de la barrera, para evitar que los hilos se despierten prematuramente
} Barrera;
// generation se utiliza para evitar que los hilos se despierten prematuramente. Cuando un hilo llega a la barrera, incrementa el contador y verifica si es el último en llegar. Si es así, incrementa la generación y reinicia el contador. Los hilos que llegan después de que la generación ha cambiado saben que deben esperar hasta que la generación vuelva a cambiar, lo que garantiza que todos los hilos hayan llegado a la barrera antes de continuar.

// si no utilizamos
void barrier_init(Barrera *b, int n);
void barrier_wait(Barrera *b);
void barrier_destroy(Barrera *b);

#endif // !BARRIER_C
