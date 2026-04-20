#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int main()
{
    int pip[2];
    pipe(pip); // creamos el pipe
    //  pip[0] es el extremo de lectura, pip[1] es el extremo de escritura
    //  el pipe es un canal de comunicación unidireccional, el padre escribirá y el hijo leerá
    pid_t pid = fork();
    if (pid == 0)
    {
        close(pip[1]); // el child no escribe, cierra el extremo de escritura
        char c;
        while (read(pip[0], &c, 1) > 0)
        {                                              // leemos del pipe de a un byte
            printf("Leyendo desde el padre: %c\n", c); // imprimimos lo leido
            sleep(1);
        }
        close(pip[0]); // cerramos el extremo de lectura
    }
    else if (pid > 0)
    {
        close(pip[0]);                     // el parent no lee, cierra el extremo de lectura
        write(pip[1], "Hola mundo\n", 11); // escribimos al pipe
        close(pip[1]);                     // cerramos el extremo de escritura
        wait(NULL);                        // esperamos a que el hijo termine
    }
    return 0;
}