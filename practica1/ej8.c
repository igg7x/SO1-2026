#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void handler(int sig){
    printf("He recibido la señal\n");
}
int main(){
    signal(SIGHUP, handler);

    __pid_t pid = fork();
    
    if (pid != 0){
        sleep(1);
        raise(SIGHUP);
        wait(NULL);
    }
    else
        exit(0);
    return 0;
}