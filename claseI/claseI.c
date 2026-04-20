#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{

    int ID = fork(); // crea una copia identica del proceso llamante
    if (ID == 0)
    {
        printf("SOY HIJO \n");
        execl("./test1", "test1", (char *)NULL);
    }
    else
    {
        printf("El hijo tiene pid = %d \n", ID);
        wait(0);
    }
    // execl("./test1", "test1", (char *)NULL); si ubicamos aca el execl pisa el padre y el hijo
    return 0;
}

//     Concurrencia: Tras el fork, ambos procesos (padre e hijo) corren en paralelo.
//     Planificación: En ese caso particular, el procesador le dio tiempo de ejecución al padre un instante antes que al hijo.
//     Buffer de salida: El padre imprimió su mensaje y luego entró en el wait(0). Mientras el padre esperaba, el hijo finalmente ejecutó su printf.

// El factor clave
// Si lo ejecutás varias veces, podrías ver el orden inverso (SOY HIJO primero y luego el PID). El wait(0) en el padre garantiza que el proceso hijo termine antes de que el padre finalice el programa, pero no garantiza el orden de los printf que ocurren antes de que el hijo muera.
// Para forzar un orden específico, tendrías que usar mecanismos de sincronización (como señales o pipes), aunque en este nivel suele bastar con entender que el orden de ejecución entre procesos independientes es impredecible.
// ¿Querés que veamos cómo usar GDB para seguir específicamente al hijo o al padre mientras debugueás?
