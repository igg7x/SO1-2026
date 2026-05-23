#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    char entrada[256];
    char* args[256];
    int i=0;

    while (1)
    {
        printf("BashCasaCasera:$ ");
        
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';
        char* token = strtok(entrada," ");
        
        while (token != NULL && i < 256)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        
        args[i] = NULL;
        
        pid_t pid = fork();
        
        if(pid == 0){
        execvp(args[0], args);
        exit(1);
        }
        else
        wait(NULL);

        i=0;
    }
    return 0;
}