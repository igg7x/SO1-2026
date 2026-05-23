#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
// Tres procesos tratan de fumar cada vez que pueden.
// Para hacerlo necesitan tres ingredientes: tabaco, papel y f´osforos. Cada uno tiene una cantidad ilimitada
// de uno de estos ingredientes. Esto es, un fumador tiene tabaco, otro tiene papel y el ´ultimo tiene f´osforos.
// Los fumadores no se prestan los ingredientes entre ellos, pero hay un cuarto proceso, el agente, con
// cantidad ilimitada de todos los ingredientes, que repetidamente pone a disposici´on de los fumadores dos
// de los tres ingredientes, eligi´endolos al azar. Cada vez que esto pasa, el fumador que tiene el ingrediente
// restante deber´ıa proceder a hacerse un cigarrillo y fumar. Al terminar de fumar (y no antes) el fumador
// avisa al agente que termin´o, para que el mismo pueda seguir atendiendo

// -- version 1 , no se puede modificar el agente

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex para proteger el acceso a las variables de estado
int isTabaco = 0, isPapel = 0, isFosforos = 0;
sem_t tabaco, papel, fosforos, otra_vez, sem_fumador1, sem_fumador2, sem_fumador3;
void agente()
{
	while (1)
	{
		sem_wait(&otra_vez);
		int caso = random() % 3;
		if (caso != 0)
			sem_post(&fosforos);
		if (caso != 1)
			sem_post(&papel);
		if (caso != 2)
			sem_post(&tabaco);
	}
}

void fumar(int fumador)
{
	printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
	sleep(1);
}

void *fumador1(void *arg)
{
	while (1)
	{

		// espera que le pongan el tabaco y el papel para fumar
		// sem_wait(&tabaco);
		// sem_wait(&papel);
		sem_wait(&sem_fumador1);
		fumar(1);
		sem_post(&otra_vez);
	}
}

void *fumador2(void *arg)
{
	while (1)
	{
		// espera que le pongan el papel y los fosforos para fumar
		// sem_wait(&fosforos);
		// sem_wait(&tabaco);
		sem_wait(&sem_fumador2);
		fumar(2);
		sem_post(&otra_vez);
	}
}

void *fumador3(void *arg)
{
	while (1)
	{
		// este fumador tiene el tabaco, asi que espera que le pongan el papel y los fosforos para fumar
		// espera que le pongan el tabaco y los fosforos para fumar
		// sem_wait(&fosforos);
		// sem_wait(&papel);
		sem_wait(&sem_fumador3);
		fumar(3);
		sem_post(&otra_vez);
	}
}
void *pusher2(void *arg)
{
	while (1)
	{
		sem_wait(&papel);			// espera a que el agente ponga el papel
		pthread_mutex_lock(&mutex); // bloquea el mutex para modificar las variables de estado
		if (isFosforos)				// pregunta si los fosforos ya estan en la mesa
		{
			isFosforos = 0; // si los fosforos ya estan en la mesa, entonces el fumador 1 puede fumar, asi que se resetea la variable de estado de los fosforos
			sem_post(&sem_fumador3);
		}
		else if (isTabaco)
		{
			isTabaco = 0;
			sem_post(&sem_fumador1);
		}
		else
			isPapel = 1;

		pthread_mutex_unlock(&mutex);
	}
}

void *pusher1(void *arg)
{
	while (1)
	{
		sem_wait(&fosforos);		// espera a que el agente ponga los fosforos
		pthread_mutex_lock(&mutex); // bloquea el mutex para modificar las variables de estado
		if (isTabaco)				// pregunta si el tabaco ya esta en la mesa
		{
			isTabaco = 0; // si el tabaco ya esta en la mesa, entonces el fumador 2 puede fumar, asi que se resetea la variable de estado del tabaco
			sem_post(&sem_fumador2);
		}
		else if (isPapel)
		{
			isPapel = 0;
			sem_post(&sem_fumador3);
		}
		else // si no hay ni tabaco ni papel, entonces se pone el estado de los fosforos a 1 para indicar que hay fosforos en la mesa
			isFosforos = 1;

		pthread_mutex_unlock(&mutex);
	}
}

void *pusher3(void *arg)
{
	while (1)
	{
		sem_wait(&tabaco);			// espera a que el agente ponga el tabaco
		pthread_mutex_lock(&mutex); // bloquea el mutex para modificar las variables de estado
		if (isPapel)				// pregunta si el papel ya esta en la mesa
		{
			isPapel = 0; // si el papel ya esta en la mesa, entonces el fumador 3 puede fumar, asi que se resetea la variable de estado del papel
			sem_post(&sem_fumador1);
		}
		else if (isFosforos)
		{
			isFosforos = 0;
			sem_post(&sem_fumador2);
		}
		else
			isTabaco = 1;

		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	pthread_t s1, s2, s3; // tres procesos tratan de fumar , cada uno tiene un ingrediente distinto
	pthread_t p1, p2, p3;
	sem_init(&tabaco, 0, 0);

	sem_init(&papel, 0, 0);

	sem_init(&fosforos, 0, 0);

	sem_init(&otra_vez, 0, 1); // valor inicial 1 para que el agente pueda empezar a poner cosas en la mesa

	// un semaforo para cada fumador, que se va a usar para indicarles a los fumadores cuando pueden fumar, es decir, cuando el agente ponga los ingredientes que necesitan en la mesa
	sem_init(&sem_fumador1, 0, 0); 
	sem_init(&sem_fumador2, 0, 0);
	sem_init(&sem_fumador3, 0, 0);

	pthread_create(&s1, NULL, fumador1, NULL); // tiene los fosforos
	pthread_create(&s2, NULL, fumador2, NULL); // tiene el papel
	pthread_create(&s3, NULL, fumador3, NULL); // tiene el tabaco

	// un hilo que avisa a los fumadores cuando pueden fumar, es decir, cuando el agente ponga los ingredientes que necesitan en la mesa
	pthread_create(&p1, NULL, pusher1, NULL);
	pthread_create(&p2, NULL, pusher2, NULL);
	pthread_create(&p3, NULL, pusher3, NULL);

	agente();

	return 0;
}
