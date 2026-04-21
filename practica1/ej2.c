#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if(argc <= 2)
        return 1;
    char* dir_binario = argv[1];
    char path[256];
    int seg = atoi(argv[2]);
    sprintf(path,"./%s", dir_binario);
    while (1)
    {
        pid_t pid = fork();
        if (!pid)
        {
            execl(path, dir_binario, (char *)NULL);
        }
        else {
            wait(NULL);
            sleep(seg);
        }
    }
    return 0;
}