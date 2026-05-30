#ifndef __VCOND_H__
#define __VCOND_H__

#include <pthread.h>
#include <semaphore.h>
// estructura de variable de condición hecha con semáforos.
typedef struct _v_cond_t 
{
    sem_t wait, intr, sig;/*semáforos que representan: el esperado por el waiter, proteccion a la variable count_waiter 
    * y el indicador si hubo un thread que haya recibido la signal*/
    int cont_waiters; // contador de hilos esperando una señal.
} v_cond_t;

// Inicializa la variable de condición.
void my_cond_init(v_cond_t *cond);

// Espera a que signalice o broadcastee la variable cond.
void my_cond_wait(v_cond_t *cond, pthread_mutex_t *mutex);

// Signaliza la varibale cond.
void my_cond_signal(v_cond_t *cond);

// Broadcastea la variable cond.
void my_cond_broadcast(v_cond_t *cond);

// Detruye la variable cond.
void my_cond_destroy(v_cond_t *cond);

#endif // __VCOND_H__
