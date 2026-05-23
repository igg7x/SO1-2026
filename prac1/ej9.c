#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handler(int sig){
    printf("Señal recibida\n");
    exit(0);
}
int main(){
    
    pid_t pid = fork();
    if (pid == 0) //child
    {
        struct sigaction sa;
        sa.sa_handler = handler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);

        sigaction(SIGHUP, &sa, NULL);

        printf("Child listo (PID: %d)\n", getpid());

        while(1)
            pause();
    }
    else //parent
    {
        printf("Enviando señal ... \n");
        sleep(0);
        kill(pid, SIGHUP);

        wait(NULL);
        printf("Parent: hijo finalizó tarea\n");
    }
    return 0;
}