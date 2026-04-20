#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <ejecutable> <milisegundos>\n", argv[0]);
        return 1;
    }
    char *bin = argv[1];
    int time = atoi(argv[2]);
    char path[256];
    snprintf(path, sizeof(path), "./%s", bin);
    while (1)
    {
        pid_t pid = fork();
        if (pid == 0) // el hijo del programa forkeado va a ejecutar el execl, el padre va a dormir y luego volver a forkear
        {
            execl(path, bin, (char *)NULL);
            exit(1);
        }
        else if (pid < 0)
        {
            perror("Error en fork");
            exit(1);
        }
        else
        {
            wait(NULL);
            sleep(time);
        }
    }
    return 0;
}