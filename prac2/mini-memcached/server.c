#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "mini.h"
#define PORT 3942
#define PUT "PUT"
#define GET "GET"
#define DEL "DEL"
#define EXIT "EXIT"
#define MAX_ITEMS 1000
Item *store[MAX_ITEMS];

// Mini memcached. Implemente un servidor que provea un key-value store a sus clientes. El
// servidor debe esperar conexiones en el puerto 3942 TCP (AF_INET, SOCK_STREAM) y atender los pedidos
// de cada cliente. Un pedido es siempre una secuencia de palabras separadas por espacios, terminado por
// un caracter de nueva l´ınea (’\n’). La primer palabra es el comando y el resto (alguna cantidad) son los
// argumentos al comando. Ninguna palabra contiene espacios ni caracteres no alfanum´ericos. Los pedidos
// posibles son:

// PUT k v: introduce al store el valor v bajo la clave k. El valor viejo para k, si exist´ıa, es pisado. El
// servidor debe responder con OK.
// DEL k: Borra el valor asociado a la clave k. El servidor debe responder con OK.
// GET k: Busca el valor asociado a la clave k. El servidor debe contestar con OK v si el valor es v, o con NOTFOUND si no hay valor asociado a k.

// Ante cualquier otro mensaje el servidor responde con EINVAL. Las respuestas del servidor siempre terminan
// con ’\n’. (El servidor puede probarse f´acilmente con netcat.) Por supuesto, deben soportarse conexiones
// simult´aneas de varios clientes.

//  Creo el mutex para proteger el store, que es compartido entre los hilos que atienden a los clientes
pthread_mutex_t mutex_store = PTHREAD_MUTEX_INITIALIZER;

void quit(char *msg)
{
	perror(msg);
	exit(1);
}
int fd_readline(int fd, char *buf)
{
	int rc;
	int i = 0;
	/*
	 * Leemos de a un caracter (no muy eficiente...) hasta
	 * completar una línea.
	 */

	while ((rc = read(fd, buf + i, 1)) > 0)
	{
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc < 0)

		return rc;

	buf[i] = 0;
	return i;
}

void *handle_conn(void *arg)
{
	int csock = *(int *)arg;
	char buf[200];
	int rc;

	while (1)
	{
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0)
		{
			/* linea vacia, se cerró la conexión */
			close(csock);
			return NULL;
		}
		/* Aquí se debería parsear el comando, y ejecutar la acción correspondiente... */
		char *token = strtok(buf, " ");
		if (strcmp(token, PUT) == 0)
		{
			char *key = strtok(NULL, " ");
			char *value = strtok(NULL, " ");
			if (is_key_exists(key, store, MAX_ITEMS))
			{
				write(csock, "KEY ALREADY EXISTS , TRY ANOTHER KEY\n", 37);
			}
			else
			{
				Item *item = malloc(sizeof(Item));
				// tengo que hacer strdup de key y value porque strtok devuelve punteros a la misma linea, que se va a sobreescribir con el siguiente pedido
				item->key = strdup(key);
				item->value = strdup(value);
				item->eliminated = 0;
				pthread_mutex_lock(&mutex_store);
				// primera version , no se maneja el caso de que el store este lleno
				put_item(item, store, MAX_ITEMS);
				// Guardar key y value en el store
				// Responder con OK
				pthread_mutex_unlock(&mutex_store);
				write(csock, "OK\n", 3);
			}
		}
		else if (strcmp(token, GET) == 0)
		{
			char *key = strtok(NULL, " ");
			// Buscar el valor asociado a key en el store
			pthread_mutex_lock(&mutex_store);
			Item *item = get_item(key, store, MAX_ITEMS);
			pthread_mutex_unlock(&mutex_store);
			if (item != NULL)
			{
				// Si se encuentra, responder con OK value\n
				write(csock, "OK , value : ", 13);
				write(csock, item->value, strlen(item->value));
				write(csock, "\n", 1);
			}
			else
			{
				// Si no se encuentra, responder con NOTFOUND\n
				write(csock, "NOTFOUND\n", 10);
			}
		}
		else if (strcmp(token, DEL) == 0)
		{
			char *key = strtok(NULL, " ");
			// Borrar el valor asociado a key en el store
			pthread_mutex_lock(&mutex_store);
			del_item(key, store, MAX_ITEMS);
			// Responder con OK
			write(csock, "OK\n", 3);
			pthread_mutex_unlock(&mutex_store);
		}
		else if (strcmp(token, EXIT) == 0)
		{
			close(csock);
			return NULL;
		}
		else
		{
			write(csock, "EINVAL\n", 7);
		}
	}
	return NULL;
}
// Creacion del socket de escucha, bind y listen
int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 3942 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

void wait_for_clients(int lsock)
{
	while (1)
	{
		// tengo que aceptar la conexion y crear un hilo para atenderla

		// creo un nuevo puntero para pasarle el socket del cliente al hilo, porque si paso la direccion de una variable local, puede ser sobreescrita por la siguiente conexion

		int *csock_ptr = malloc(sizeof(int));
		*csock_ptr = accept(lsock, NULL, NULL);

		pthread_t t_client;

		int status = pthread_create(&t_client, NULL, (void *)handle_conn, (void *)csock_ptr);

		if (status != 0)
		{
			free(csock_ptr);
			quit("pthread_create");
		}
		pthread_detach(t_client); // Desasociamos el hilo para que libere sus recursos al terminar
	}
}

int main()
{
	printf("Mini memcached server listening on port %d\n", PORT);
	printf("Para conectarse, usar: nc localhost %d\n", PORT);

	int lsock = mk_lsock();
	wait_for_clients(lsock);
	return 0;
}
