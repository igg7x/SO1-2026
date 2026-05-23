#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "/tmp/echo_stream" // definimos el path de la direccion del socket
#define BUF_SIZE 100 // tamaño buffer

int main(){
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_un addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    char* msg = "Hola desde cliente";

    write(sockfd, msg, strlen(msg));

    ssize_t nBytes = read(sockfd, buffer, BUF_SIZE);
    buffer[nBytes] = '\0';

    printf("cliente recibio: %s\n", buffer);


    close(sockfd);
    return 0;
}