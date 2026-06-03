#ifndef __COND_V_H__
#define __COND_V_H__
#include <pthread.h>
#include <semaphore.h>
typedef struct
{
    sem_t sem; // semáforo para bloquear/desbloquear el hilo
    struct thread_node *next;
} Thread_Node; // estructura para representar un nodo en la cola de espera

typedef struct
{
    Thread_Node *head;     // puntero al primer nodo de la cola de espera
    Thread_Node *tail;     // puntero al último nodo de la cola de espera
    pthread_mutex_t mutex; // mutex para proteger el acceso a la cola de espera
    int count_waiters;     // contador de hilos esperando en la condición
} Cond_V;

void condv_init(Cond_V *cond);
void condv_wait(Cond_V *cond, pthread_mutex_t *mutex);
void condv_signal(Cond_V *cond);
void condv_broadcast(Cond_V *cond);
void condv_destroy(Cond_V *cond);
#endif // !COND_V.H
