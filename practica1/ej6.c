#include <stdio.h>
#include <signal.h>
#include <unistd.h>
// void handler(int signal){
//     printf("Señal recibida\n");
// }

int main(){

   signal(SIGTSTP,SIG_IGN); // registramos para ignorar
    
    printf("Enviando señal...\n");
    
    raise(SIGTSTP);
    
    printf("Señal ignorada\n");
    return 0;
}