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
#include <sys/epoll.h>
#include "mini.h"
#define PORT 3942
#define PUT "PUT"
#define GET "GET"
#define DEL "DEL"
#define EXIT "EXIT"
#define MAX_ITEMS 1000
#define MAX_EVENTS 10
pthread_t workers[MAX_EVENTS];
Item *store[MAX_ITEMS];

// Mini memcached con epoll y threads. Implemente un servidor que provea un key-value store a sus clientes.
pthread_mutex_t mutex_store = PTHREAD_MUTEX_INITIALIZER;

void quit(char *msg)
{
    perror(msg);
    exit(1);
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

// Aceptar nueva conexión y agregarla a epoll
void handle_new_connection(int epfd, int lsock)
{
    int csock = accept(lsock, NULL, NULL);
    if (csock < 0)
        return;

    fcntl(csock, F_SETFL, O_NONBLOCK);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = csock;

    epoll_ctl(epfd, EPOLL_CTL_ADD, csock, &ev);
}

// Procesar comando PUT: agregar clave-valor al store
void handle_put(int fd, char *key, char *value)
{
    if (!key || !value)
    {
        write(fd, "EINVAL\n", 7);
        return;
    }

    pthread_mutex_lock(&mutex_store);

    if (is_key_exists(key, store, MAX_ITEMS))
    {
        pthread_mutex_unlock(&mutex_store);
        write(fd, "KEY EXISTS\n", 11);
    }
    else
    {
        Item *item = malloc(sizeof(Item));
        item->key = strdup(key);
        item->value = strdup(value);
        item->eliminated = 0;

        put_item(item, store, MAX_ITEMS);

        pthread_mutex_unlock(&mutex_store);
        write(fd, "OK\n", 3);
    }
}

// Procesar comando GET: obtener valor por clave
void handle_get(int fd, char *key)
{
    if (!key)
    {
        write(fd, "EINVAL\n", 7);
        return;
    }

    pthread_mutex_lock(&mutex_store);
    Item *item = get_item(key, store, MAX_ITEMS);
    pthread_mutex_unlock(&mutex_store);

    if (item)
    {
        write(fd, "OK ", 3);
        write(fd, item->value, strlen(item->value));
        write(fd, "\n", 1);
    }
    else
    {
        write(fd, "NOTFOUND\n", 10);
    }
}

// Procesar comando DEL: eliminar clave del store
void handle_del(int fd, char *key)
{
    if (!key)
    {
        write(fd, "EINVAL\n", 7);
        return;
    }

    pthread_mutex_lock(&mutex_store);
    del_item(key, store, MAX_ITEMS);
    pthread_mutex_unlock(&mutex_store);

    write(fd, "OK\n", 3);
}

// Procesar pedido de un cliente
void handle_client_request(int epfd, int fd)
{
    char buf[256];
    int rc = read(fd, buf, sizeof(buf));

    if (rc <= 0)
    {
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        return;
    }

    char *newline = memchr(buf, '\n', rc);
    if (!newline)
        return;

    *newline = '\0';

    char *token = strtok(buf, " ");
    if (!token)
    {
        write(fd, "EINVAL\n", 7);
        return;
    }

    if (strcmp(token, PUT) == 0)
    {
        char *key = strtok(NULL, " ");
        char *value = strtok(NULL, " ");
        handle_put(fd, key, value);
    }
    else if (strcmp(token, GET) == 0)
    {
        char *key = strtok(NULL, " ");
        handle_get(fd, key);
    }
    else if (strcmp(token, DEL) == 0)
    {
        char *key = strtok(NULL, " ");
        handle_del(fd, key);
    }
    else
    {
        write(fd, "EINVAL\n", 7);
    }
}

// Función principal que espera y procesa eventos con epoll
void wait_clients_with_epoll(int epfd, int lsock, struct epoll_event evlist[])
{
    while (1)
    {
        int n = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        if (n < 0)
            quit("epoll_wait");

        for (int i = 0; i < n; i++)
        {
            int fd = evlist[i].data.fd;

            if (fd == lsock)
            {
                // Nueva conexión
                handle_new_connection(epfd, lsock);
            }
            else if (evlist[i].events & EPOLLIN)
            {
                // Datos de un cliente
                handle_client_request(epfd, fd);
            }
        }
    }
}

int main()
{
    printf("Mini memcached server listening on port %d\n", PORT);

    int epfd = epoll_create(MAX_EVENTS);
    struct epoll_event ev, evlist[MAX_EVENTS];

    int lsock = mk_lsock();
    ev.events = EPOLLIN;
    ev.data.fd = lsock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lsock, &ev);
    wait_clients_with_epoll(epfd, lsock, evlist);
    return 0;
}