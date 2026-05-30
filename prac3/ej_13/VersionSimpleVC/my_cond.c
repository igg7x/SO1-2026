#include "VCond.h"
// Inicializa la variable de condición.
void my_cond_init(v_cond_t *cond)
{
    // Inicializa los semáforo en 0, cuando se la haga wait quedará lockeado.
    sem_init(&(cond->wait), 0, 0);
    sem_init(&(cond->sig), 0, 0);
    // El semáforo que bloquea count_waiters inicializa en 1 esperando un solo modificador a la vez.
    sem_init(&(cond->intr), 0, 1);
    // Inicializa la cantidad de hilos esperando un lock.
    cond->cont_waiters = 0;
}

// Espera a que signalice o broadcastee la variable cond.
void my_cond_wait(v_cond_t *cond, pthread_mutex_t *mutex)
{
    // baja el semáforo para poder modificar el contador.
    sem_wait(&(cond->intr));
    // se registra como waiter.
    cond->cont_waiters++;

    sem_post(&(cond->intr));
    // suelta el mutex de la condición.
    pthread_mutex_unlock(mutex);
    // baja el semáforo y espera un signal o un broadcast.
    sem_wait(&(cond->wait));
    // sube el semáforo indicando que recibió la signal.
    sem_post(&(cond->sig));
    // pide el mutex de la condición.
    pthread_mutex_lock(mutex);
}

// Signaliza la varibale cond.
void my_cond_signal(v_cond_t *cond)
{
    sem_wait(&(cond->intr));
    // verifico que haya threads esperando para no alamacenar señales.
    if (cond->cont_waiters > 0)
    {
        cond->cont_waiters--; // consume un waiter.
        sem_post(&(cond->wait)); // levanta el semáforo.
        sem_wait(&(cond->sig)); // baja el semaforo esperando un receptor de la señal.
    }
    sem_post(&(cond->intr));
}

// Broadcastea la variable cond.
void my_cond_broadcast(v_cond_t *cond)
{
    sem_wait(&(cond->intr));
    // despierta a todos los hilos en espera.
    for (int i = 0; i < cond->cont_waiters; i++)
        sem_post(&(cond->wait)); 
    // verifica que todos hayan recibido la señal.
    while (cond->cont_waiters > 0)
    {
        cond->cont_waiters--;
        sem_wait(&(cond->sig));
    }
    sem_post(&(cond->intr));
}


// Detruye la variable cond.
void my_cond_destroy(v_cond_t *cond)
{
    // Destruye los semáforos.
    sem_destroy(&(cond->intr));
    sem_destroy(&(cond->sig));
    sem_destroy(&(cond->wait));
}
