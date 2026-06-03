#include "cond_v.h"

void condv_init(Cond_V *cond)
{
    cond->head = NULL;
    cond->tail = NULL;
    cond->count_waiters = 0;
    pthread_mutex_init(&cond->mutex, NULL);
}

void condv_wait(Cond_V *cond, pthread_mutex_t *mutex)
{
    // creo un nuevo nodo para el hilo que va a esperar
    Thread_Node *node = malloc(sizeof(Thread_Node));
    sem_init(&node->sem, 0, 0);
    node->next = NULL;

    // agrego el nodo a la cola de espera
    pthread_mutex_lock(&cond->mutex);
    if (cond->tail == NULL)
    {
        cond->head = node;
        cond->tail = node;
    }
    else
    {
        cond->tail->next = node;
        cond->tail = node;
    }
    cond->count_waiters++;

    pthread_mutex_unlock(&cond->mutex);
    // libero el mutex del hilo que va a esperar y espero a ser señalizadox
    pthread_mutex_unlock(mutex);
    // espero a ser señalizado
    sem_wait(&node->sem);
    pthread_mutex_lock(mutex);
}

void condv_signal(Cond_V *cond)
{

    pthread_mutex_lock(&cond->mutex);
    if (cond->count_waiters > 0)
    {
        // saco el primer nodo de la cola de espera y lo señalizo
        Thread_Node *node = cond->head;
        cond->head = node->next;
        if (cond->head == NULL)
        {
            cond->tail = NULL;
        }
        cond->count_waiters--;
        // señalizo al hilo que estaba esperando
        sem_post(&node->sem);
        // libero el nodo
        free(node);
    }
    pthread_mutex_unlock(&cond->mutex);
}

void condv_broadcast(Cond_V *cond)
{
    pthread_mutex_lock(&cond->mutex);
    while (cond->count_waiters > 0)
    {
        // saco el primer nodo de la cola de espera y lo señalizo
        Thread_Node *node = cond->head;
        cond->head = node->next;
        if (cond->head == NULL)
        {
            cond->tail = NULL;
        }
        cond->count_waiters--;
        // señalizo al hilo que estaba esperando
        sem_post(&node->sem);
        // libero el nodo
        free(node);
    }
    pthread_mutex_unlock(&cond->mutex);
}

void condv_destroy(Cond_V *cond)
{
    // destruyo todos los nodos de la cola de espera y el mutex
    pthread_mutex_lock(&cond->mutex);
    Thread_Node *current = cond->head;
    while (current != NULL)
    {
        Thread_Node *temp = current;
        current = current->next;
        sem_destroy(&temp->sem);
        free(temp);
    }
    pthread_mutex_unlock(&cond->mutex);
    pthread_mutex_destroy(&cond->mutex);
}