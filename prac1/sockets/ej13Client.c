#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/echo_socket" // path del server
#define CLIENT_PATH "/tmp/echo_client" // path cliente
#define BUF_SIZE 100 // tamaño buffer

int main(){
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_un serveraddr, cliaddr; // delcaramos addr para guardar al socket
    socklen_t len = sizeof(cliaddr); // declaramos el tamaño del cliente

    memset(&cliaddr, 0, len); // seteamos la estructura en 0
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0); // generamos el socket
    cliaddr.sun_family = AF_UNIX; // nombre dle socket
    strcpy(cliaddr.sun_path, CLIENT_PATH); // path de direccion 

    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, SERVER_PATH);       


    unlink(CLIENT_PATH); // bora si ya existia el archivo.

    bind(sockfd, (struct sockaddr*)&cliaddr, len); // bindeamos el socket

    char* msg = "Hola servidor desde client";
    
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    size_t nBytes = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);

    buffer[nBytes] = '\0';

    printf("El cliente recibe la respuesta: %s\n", buffer);

    close(sockfd);
    unlink(CLIENT_PATH);

    return 0;

}