#include "VCond.h"
// Inicializa la variable de condición.
void my_cond_init(v_cond_t *cond)
{
    // Inicializa el semáforo en 0, cuando se la haga wait quedará lockeado.
    sem_init(&(cond->sem), 0, 0);
    // Inicializa el mutex interno de la variable de condición.
    pthread_mutex_init(&(cond->mutex_interno), NULL);
    // Inicializa la cantidad de hilos esperando un lock.
    cond->cont_waiters = 0;
}

// Espera a que signalice o broadcastee la variable cond.
void my_cond_wait(v_cond_t *cond, pthread_mutex_t *mutex)
{
    // pide el mutex interno.
    pthread_mutex_lock(&(cond->mutex_interno));
    // se registra como waiter.
    cond->cont_waiters++;
    // suelta el mutex interno.
    pthread_mutex_unlock(&(cond->mutex_interno));
    // suelta el mutex de la condición.
    pthread_mutex_unlock(mutex);
    // baja el semáforo y espera un signal o un broadcast.
    sem_wait(&(cond->sem));
    // pide el mutex de la condición.
    pthread_mutex_lock(mutex);
}

// Signaliza la varibale cond.
void my_cond_signal(v_cond_t *cond)
{
    // pide el mutex interno.
    pthread_mutex_lock(&(cond->mutex_interno));
    // verifico que haya threads esperando para no alamacenar señales.
    if (cond->cont_waiters > 0)
    {
        cond->cont_waiters--; // consume un waiter.
        sem_post(&(cond->sem)); // levanta el semáforo.
    }
    // suelta el mutex interno.
    pthread_mutex_unlock(&(cond->mutex_interno));
}

// Broadcastea la variable cond.
void my_cond_broadcast(v_cond_t *cond)
{
    // pide el mutex interno.
    pthread_mutex_lock(&(cond->mutex_interno));
    // despierta a todos los hilos en espera.
    while (cond->cont_waiters > 0)
    {
        cond->cont_waiters--;
        sem_post(&(cond->sem));
    }
    // suelta el mutex interno.
    pthread_mutex_unlock(&(cond->mutex_interno));
}


// Detruye la variable cond.
void my_cond_destroy(v_cond_t *cond)
{
    // Destruye el semáforo.
    sem_destroy(&(cond->sem));
    // destruye el mutex.
    pthread_mutex_destroy(&(cond->mutex_interno));
}