#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// compartiendo memoria entre un proceso padre y un proceso hijo
int main()
{
    // Crear un archivo temporal para compartir memoria
    const char *filename = "shared_memory.bin";
    // siempre debe ser un archivo binario para evitar problemas de formato en diferentes sistemas operativos
    
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Error al crear el archivo");
        return EXIT_FAILURE;
    }

    // Establecer el tamaño del archivo
    size_t size = 4096; // Tamaño de la memoria compartida(4KB)

    if (ftruncate(fd, size) == -1)
    {
        perror("Error al establecer el tamaño del archivo");
        close(fd);
        return EXIT_FAILURE;
    }

    // Mapear el archivo en memoria
    void *shared_memory = mmap(NULL, size, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 0);

    if (shared_memory == MAP_FAILED)
    {
        perror("Error al mapear el archivo");
        close(fd);
        return EXIT_FAILURE;
    }

    // Escribir datos en la memoria compartida
    const char *message = "Hola desde el proceso padre!";
    snprintf((char *)shared_memory, size, "%s", message);
    // Crear un proceso hijo
    pid_t pid = fork();
    if (pid == 0)
    {
        // Proceso hijo: leer datos de la memoria compartida
        printf("Mensaje del proceso padre: %s\n", (char *)shared_memory);
        // Desmapear la memoria compartida
        munmap(shared_memory, size);
        close(fd);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        // Proceso padre: esperar a que el hijo termine
        wait(NULL);
        // Desmapear la memoria compartida
        munmap(shared_memory, size);
        close(fd);
    }
    else
    {
        perror("Error al crear el proceso hijo");
        munmap(shared_memory, size);
        close(fd); // cierra el file descriptor antes de salir
        return EXIT_FAILURE;
    }
    return 0;
}