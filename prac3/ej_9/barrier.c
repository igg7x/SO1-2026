#include "barrier_c.h"
#include <pthread.h>
#include <stdio.h>

void barrier_init(Barrera *b, int n)
{
    b->generation = 0;
    b->count = 0;
    b->num_threads = n;
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
}

void barrier_wait(Barrera *b)
{
    pthread_mutex_lock(&b->mutex);
    b->count++;
    int gen = b->generation;

    if (b->count == b->num_threads)
    {
        b->generation++;
        b->count = 0;
        pthread_cond_broadcast(&b->cond);
    }
    else
    {
        while (gen == b->generation)
        {
            pthread_cond_wait(&b->cond, &b->mutex);
        }
    }
    pthread_mutex_unlock(&b->mutex);
}

void barrier_destroy(Barrera *b)
{
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cond);
}