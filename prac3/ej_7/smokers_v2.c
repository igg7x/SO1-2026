#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_FUMADORES 7

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t ingredient_sem[NUM_FUMADORES];
sem_t smoker_sem[NUM_FUMADORES];
sem_t otra_vez;
int countTabaco = 0, countPapel = 0, countFosforos = 0;

void agente(void)
{
    while (1)
    {
        sem_wait(&otra_vez);
        int ingredient = random() % NUM_FUMADORES;
        sem_post(&ingredient_sem[ingredient]);
    }
}

void fumar(int fumador)
{
    printf("Fumador %d: Puf! Puf! Puf!\n", fumador + 1);
    sleep(1);
}

void *fumador(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        sem_wait(&smoker_sem[id]);
        fumar(id);
        sem_post(&otra_vez);
    }
    return NULL;
}

void *pusher(void *arg)
{
    int ingredient = *(int *)arg;

    while (1)
    {
        sem_wait(&ingredient_sem[ingredient]);
        pthread_mutex_lock(&mutex);

        mesa[ingredient] = 1;
        int count = 0;
        for (int i = 0; i < NUM_FUMADORES; i++)
            count += mesa[i];

        if (count == NUM_FUMADORES - 1)
        {
            int missing = -1;
            for (int i = 0; i < NUM_FUMADORES; i++)
            {
                if (!mesa[i])
                {
                    missing = i;
                    break;
                }
            }

            for (int i = 0; i < NUM_FUMADORES; i++)
                mesa[i] = 0;

            sem_post(&smoker_sem[missing]);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t fumadores[NUM_FUMADORES];
    pthread_t pushers[NUM_FUMADORES];
    sem_init(&otra_vez, 0, 1);

    for (int i = 0; i < NUM_FUMADORES; i++)
    {
        sem_init(&smoker_sem[i], 0, 0);
    }

    for (int i = 0; i < NUM_FUMADORES; i++)
        pthread_create(&fumadores[i], NULL, fumador, i + (void *)0);

    for (int i = 0; i < NUM_FUMADORES; i++)
        pthread_create(&pushers[i], NULL, pusher, i + (void *)0);

    agente();
    return 0;
}
