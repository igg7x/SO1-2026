#include "my_cond.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void my_cond_init(my_cond_t *cond) {
    // Inicializa el semáforo en 0, cuando se la haga wait quedará lockeado.
    sem_init(&(cond->sem), 0, 0);
}

void my_cond_wait(my_cond_t *cond, pthread_mutex_t *mutex) {
    // suelta el mutex.
    pthread_mutex_unlock(mutex);
    // Baja el semáforo y espera un signal o un broadcast.
    sem_wait(&(cond->sem));
    // pide el mutex.
    pthread_mutex_lock(mutex);
}

void my_cond_signal(my_cond_t *cond) {
    // Levanta el semáforo.
    sem_post(&(cond->sem));
}


void my_cond_destroy(my_cond_t *cond) {
    // Destruye el semáforo.
    sem_destroy(&(cond->sem));
}
