#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(){
    int fields[2];
    pipe(fields);

    __pid_t pid = fork();

    if (pid == 0)
    {
        //child lee
        char buffer[100];
        close(fields[1]);
        read(fields[0],buffer,sizeof(buffer));
        close(fields[0]);
        printf("El child de pid: %d, recibio el mensaje: %s\n",getpid(),buffer);
        exit(0);
    }
    else
    {
        //parent escribe
        close(fields[0]);
        char* msg ="Recurso utilizado exitosamente";
        write(fields[1], msg, strlen(msg) + 1);
        close(fields[1]);
        wait(NULL);
    }
    return 0;
}