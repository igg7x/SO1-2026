#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("estoy en el proceso hijo y devolvere el codigo 67\n");
        exit(67);
    }
    else
    {
        int exitCode;
        wait(&exitCode);
        if (WIFEXITED(exitCode))        
        printf("Soy el proceso padre y recibi el codigo: %d\n",WEXITSTATUS(exitCode));
        else
            printf("No se quiteo correctamente"); 
    }
    return 0;
}