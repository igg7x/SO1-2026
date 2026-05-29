#include "VCond.h"
#include <stdio.h>
#include <stdlib.h>
/* Producer/consumer program illustrating conditional variables */

/* Size of shared buffer */
#define BUF_SIZE 3

int buffer[BUF_SIZE];					/* shared buffer */
int add=0;										/* place to add next element */
int rem=0;										/* place to remove next element */
int num=0;										/* number elements in buffer */
int quit = 0; //if pquit
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
v_cond_t cond_non_full;  // condicion que queremos esperar , en este caso cuando el buffer no este lleno
v_cond_t cond_non_empty; 
/* Produce value(s) */
void *producer(void *param)
{
	int i;
	for (i=1; i<=20; i++) {
		/* Insert into buffer */
		pthread_mutex_lock(&mutex);
		while (num == BUF_SIZE) 
			my_cond_wait(&cond_non_full, &mutex);	/* overflow */
		/* if executing here, buffer not full so add element */
		buffer[add] = i;
		add = (add+1) % BUF_SIZE;
		num++;
		my_cond_signal(&cond_non_empty);
		pthread_mutex_unlock(&mutex);
    printf ("producer: inserted %d\n", i);  fflush (stdout);
	}
	quit = 1;
	printf ("producer quiting\n");  fflush (stdout);
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param)
{
	int i;
	while (!quit) {
		pthread_mutex_lock(&mutex);
		while (num == 0) 
			my_cond_wait(&cond_non_empty, &mutex);   /* underflow */
		/* if executing here, buffer not empty so remove element */
		i = buffer[rem];
		rem = (rem+1) % BUF_SIZE;
		num--;
		my_cond_signal(&cond_non_full);
		pthread_mutex_unlock(&mutex);
    printf ("Consume value %d\n", i);  fflush(stdout);
	}
}

int main (int argc, char *argv[])
{
	pthread_t tid1, tid2;		/* thread identifiers */
	int i;
    my_cond_init(&cond_non_empty);
    my_cond_init(&cond_non_full);
	/* create the threads; may be any number, in general */
	if (pthread_create(&tid1,NULL,producer,NULL) != 0) {
		fprintf (stderr, "Unable to create producer thread\n");
		exit (1);
	}
	if (pthread_create(&tid2,NULL,consumer,NULL) != 0) {
		fprintf (stderr, "Unable to create consumer thread\n");
		exit (1);
	}
	/* wait for created thread to exit */
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

    my_cond_destroy(&cond_non_empty);
    my_cond_destroy(&cond_non_full);
	printf ("Parent quiting\n");
	
	return 0;
}