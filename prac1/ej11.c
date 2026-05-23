#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(){

    const char* file = "file_shared.bin";
    int fd = open(file ,  O_RDONLY | O_RDWR| O_CREAT , 0666) ;
    size_t size = 4096;
    if (fd == -1 )
    {
        printf("ERROR");
        exit(1);
    }

    void* address_shared = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    ftruncate(fd, size);

    pid_t pid = fork() ; 

    if (pid == -1)
    {   
        printf("ERR");
        exit(1);
    }
    if (pid == 0 )
    {
        sprintf((char*)address_shared, "Hello from the child process!");
        exit(0);

    }else{

        wait(0);
        printf("Parent reads: %s\n", (char*)address_shared);
        munmap(address_shared,size);
        close(fd);
        unlink(file);
    }

    return 0 ; 
}