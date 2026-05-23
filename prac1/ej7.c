#include <stdio.h>
#include <signal.h>
#include <unistd.h>
void handleSignals(int sig){
    if(sig == SIGINT){
        printf("You pressed CTRL+C\nNow reverting SIGINT signal to default action\n");

        signal(SIGINT,SIG_DFL); //restauramos la accion default para SIGINT
    }
    else if(sig == SIGQUIT)
        printf("You pressed CTRL+/\n");
}

int main(){
    signal(SIGINT, handleSignals); //registramos el handler de sigint
    signal(SIGQUIT, handleSignals); //registramos el handler de sigquit
    printf("%d\n",getpid());
    printf("To terminate this program, perform the following:\n");
    printf("1. Open another terminal\n");
    printf("2. Issue command: kill 74 or issue CTRL+C 2 times (second time it terminates)\n");
    
    while(1)
        pause();
    return 0;
}