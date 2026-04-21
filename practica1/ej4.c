#include <signal.h>
#include <stdio.h>

int den=0;

void handler(int sig){
    printf("Intento de división por 0, ERROR\n");
    den = 1;
}

int main(){
    int res;
    signal(SIGFPE,handler); //handler maneja la señal
    res = 1/den;
    
    printf("El resultado es %d\n", res);
    return 0;
}