#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(){
    while (1)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execl("/usr/bin/make", "make", "test", NULL);
            exit(1);
        }
        wait(NULL);
    }
    
    return 0;
}