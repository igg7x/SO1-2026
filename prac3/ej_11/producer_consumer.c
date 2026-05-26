#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "ch_sync.h"

#define N_PROD 3
#define N_CONS 3
#define ITEMS_POR_PROD 5

Channel_S *channel;

void *producer(void *arg)
{
    int id = (int)(arg - (void *)0);

    for (int i = 0; i < ITEMS_POR_PROD; i++)
    {
        int item = id * 100 + i;
        printf("[Productor %d] enviando %d\n", id, item);
        channel_write(channel, item);
        printf("[Productor %d] entregado %d\n", id, item);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int id = (int)(arg - (void *)0);

    while (1)
    {
        int item = channel_read(channel);
        printf("[Consumidor : %d] procesé %d\n", id, item);
    }
    return NULL;
}

int main()
{
    pthread_t producers[N_PROD], consumers[N_CONS];

    channel = malloc(sizeof(Channel_S));
    channel_init(channel);

    /* consumidores primero, para que estén listos cuando lleguen items */
    for (int i = 0; i < N_CONS; i++)
        pthread_create(&consumers[i], NULL, consumer, i + (void *)0);

    for (int i = 0; i < N_PROD; i++)
        pthread_create(&producers[i], NULL, producer, i + (void *)0);

    /* esperar que todos los productores terminen */
    for (int i = 0; i < N_PROD; i++)
        pthread_join(producers[i], NULL); /* sin & */

    /* un solo sentinel; los consumidores lo propagan en cadena */
    // printf("[MAIN] enviando sentinel\n");
    // channel_send(channel, SENTINEL);

    for (int i = 0; i < N_CONS; i++)
        pthread_join(consumers[i], NULL); /* sin & */

    free(channel);
    return 0;
}