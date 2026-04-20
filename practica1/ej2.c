#include <stdio.h>
#include <unistd.h>

// int main()
// {
//     printf("Hola mundo\n");
//     close(1); // Cerramos el descriptor de salida estandar
//     write(1, "Esto no se imprimirá\n", 22); // Intentamos escribir en el descriptor cerrado
//     printf("Esto no se imprimirá\n");
//     return 0;

// }


int main(){

    close(0) ; // Cerramos el descriptor de entrada estandar
    char buffer[100];
    printf("Introduce un texto: \n");
    fgets(buffer, 100, stdin); // Intentamos leer del descriptor cerrado
    printf("El texto introducido es: %s\n", buffer); // Esto no se imprimirá correctamente
    return 0;
}