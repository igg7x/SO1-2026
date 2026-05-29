#ifndef __MY_COND_H__
#define __MY_COND_H__

#include <pthread.h>
#include <semaphore.h>

typedef struct _my_cond_t {
    sem_t sem;
} my_cond_t;

// Inicializa la variable de condición.
void my_cond_init(my_cond_t *cond);

// Espera a que signalice o broadcastee la variable cond.
void my_cond_wait(my_cond_t *cond, pthread_mutex_t *mutex);

// Signaliza la varibale cond.
void my_cond_signal(my_cond_t *cond);

// Broadcastea la variable cond.
void my_cond_broadcast(my_cond_t *cond);

// Detruye la variable cond.
void my_cond_destroy(my_cond_t *cond);

#endif
