#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define B 2
#define K (2 * B)

// cola concurrente entre dos procesos productor y consumidor, sin sincronización
// s y r siempre estan en [0,K-1]
// s mantiene la cantidad de elementos escritos por el productor,
// r mantiene la cantidad de elementos leidos por el consumidor
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;

volatile int s, r, buf[B];

static inline int diff()
{
    return (K + s - r) % K;
}

void *prod(void *_arg)
{
    int cur = 0; // el siguiente número a producir
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (diff() == B) // mientras  la diferencia entre s y r sea igual a B, el buffer esta lleno, entonces el productor espera (busy waiting)
        {
            pthread_cond_wait(&cond_prod, &mutex); // el productor se bloquea y libera el mutex, hasta que el consumidor lo despierte con pthread_cond_signal(&cond_prod) cuando consuma un elemento y deje espacio en el buffer
        }
        buf[s % B] = cur++; // primero
        s = (s + 1) % K;
        pthread_cond_signal(&cond_cons); // luego despierta al consumidor para que consuma el elemento que acaba de producir
        pthread_mutex_unlock(&mutex);    // finalmente libera el mutex para que el consumidor pueda acceder al buffer
    }
}

void *cons(void *_arg)
{
    int cur; // el siguiente número a consumir
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (diff() == 0)
        {
            pthread_cond_wait(&cond_cons, &mutex);
        }
        cur = buf[r % B];
        r = (r + 1) % K; // actualiza r para indicar que se ha consumido un elemento , se hace módulo K para mantenerlo en el rango [0,K-1]
        printf("Le´ı %d\n", cur);
        pthread_cond_signal(&cond_prod);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t p, c;

    pthread_create(&p, NULL, prod, NULL);
    pthread_create(&c, NULL, cons, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}