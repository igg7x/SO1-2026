#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int sv[2]; // socket pair
    char buffer[100]; // buffer de comunicacion

    if (socketpair(AF_UNIX, SOCK_STREAM, 0,sv) == -1)
    {
        perror("socketpair");
        exit(1);
    }
    
    pid_t pid = fork();

    if (pid == 0) //hijo
    {
        close(sv[0]); // cierro extremo que no uso
        
        char* msgHijo = "Hola desde child\n";
        write(sv[1], msgHijo, strlen(msgHijo) + 1);
        // los fd leen y escriben al ser socket
        read(sv[1], buffer, sizeof(buffer));

        printf("Child leyo: %s\n", buffer);
        
        close(sv[1]);

        exit(0);
    }

    // padre

    close(sv[1]);

    char* msgPadre = "Hola desde parent\n";
    write(sv[0], msgPadre, strlen(msgPadre) + 1);  

    read(sv[0], buffer, sizeof(buffer));

    printf("Parent leyo: %s\n", buffer);

    close(sv[0]);

    wait(NULL);
    return 0;
}