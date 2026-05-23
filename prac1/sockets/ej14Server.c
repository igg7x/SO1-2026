#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCK_PATH "/tmp/echo_stream" // definimos el path de la direccion del socket
#define BUF_SIZE 100 // tamaño buffer

int main(){
    int sockfd, clientfd;
    char buffer[BUF_SIZE];
    struct sockaddr_un addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    unlink(SOCK_PATH);

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
   
    listen(sockfd, 5);

    printf("Servidor esperando mensaje ...\n");

    clientfd = accept(sockfd, NULL, NULL);

    ssize_t nBytes = read(clientfd, buffer, BUF_SIZE);
    buffer[nBytes] = '\0';

    printf("Servidor recibio: %s\n", buffer);

    write(clientfd, buffer, nBytes);

    close(clientfd);
    close(sockfd);
    unlink(SOCK_PATH);

    return 0;
}