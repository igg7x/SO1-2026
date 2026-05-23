#include <stdio.h>
#include <signal.h>
#include <unistd.h>
//definicion del handler
void handler(int signal){
    if(signal == SIGCONT)
    printf("Received signal SIGCONT\n");
}
int main(){
    __pid_t pid = getpid(); //obtenemos el pid del proceso

    signal(SIGCONT,handler); //registramos el manejador y esperamos la señal

    printf("Testing SIGSTOP\n");

    printf("Open Another Terminal and issue following command\n");

    printf("kill -SIGCONT %d or kill -CONT %d or kill -18 %d\n",pid,pid,pid);

    raise(SIGSTOP); //detiene el proceso

    printf("Se reanudo correctamente\n");
    
    return 0;
}