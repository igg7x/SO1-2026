#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>


#define SOCK_PATH "/tmp/echo_socket" // definimos el path de la direccion del socket
#define BUF_SIZE 100 // tamaño buffer

int main(){
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_un addr, cliaddr; // delcaramos addr para guardar al socket
    socklen_t len = sizeof(cliaddr); // declaramos el tamaño del cliente

    memset(&addr, 0, sizeof(addr)); // seteamos la estructura en 0
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0); // generamos el socket
    addr.sun_family = AF_UNIX; // nombre dle socket
    strcpy(addr.sun_path, SOCK_PATH); // path de direccion 

    unlink(SOCK_PATH); // bora si ya existia el archivo.

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)); // bindeamos el socket

    printf("servidor unix esperando ... \n");

    while (1)
    {
        ssize_t nBytes = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&cliaddr, &len);

        buffer[nBytes] = '\0';

        printf("Servidor recibe: %s\n", buffer);

        sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&cliaddr, len);
    }
    //no va a terminar pero si en otro ej lo hace es necesario.
    close(sockfd);
    unlink("/tmp/echo_socket");
    return 0;

}