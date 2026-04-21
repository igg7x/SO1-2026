#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
/* Producer/consumer program illustrating conditional variables */

/* Size of shared buffer */
#define BUF_SIZE 3

int buffer[BUF_SIZE];							/* shared buffer */ 
int add=0;										/* place to add next element *z|*/
int rem=0;										/* place to remove next element */
int num=0;										/* number elements in buffer */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ; 
pthread_cond_t cond_non_full =PTHREAD_COND_INITIALIZER ;  // condicion que queremos esperar , en este caso cuando el buffer no este lleno
pthread_cond_t cond_non_empty =PTHREAD_COND_INITIALIZER ; 

/* Produce value(s) */
void *producer(void *param)
{
	int i;
	for (i=1; i<=20; i++) {
		pthread_mutex_lock(&mutex);
		while (BUF_SIZE == num)
		{
			pthread_cond_wait(&cond_non_full, &mutex);
		}
		/* Insert into buffer */
		/* if executing here, buffer not full so add element */
		buffer[add] = i;
		add = (add+1) % BUF_SIZE;
		num++;
		printf ("producer: inserted %d\n", i);  fflush (stdout);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond_non_empty); 
	}
	printf ("producer quiting\n");  fflush (stdout);
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param)
{
	int i;
	while () {		
		pthread_mutex_lock(&mutex);
		while (num == 0 )
		{
			pthread_cond_wait(&cond_non_empty , &mutex);
		}
		/* if executing here, buffer not empty so remove element */
		i = buffer[rem];
		rem = (rem+1) % BUF_SIZE;
		num--;
		printf ("Consume value %d\n", i);  fflush(stdout);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond_non_full);
	}
}

int main (int argc, char *argv[])
{
	pthread_t tid1, tid2;		/* thread identifiers */
	int i;

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
	printf ("Parent quiting\n");
	return 0 ;
}