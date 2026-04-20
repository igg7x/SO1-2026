#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int denom = 0 ;  
void handler(int s){
    printf("outc!\n"); 
    denom = 1;    
}
int main(){
    int r ; 
    signal(SIGFPE,handler); // handler es el manjador de la señal si se produce la signal SIGFPE el handler manejara esto
    r = 1 / denom ;
    printf("r %d\n",r) ; 
    return 0 ; 
}