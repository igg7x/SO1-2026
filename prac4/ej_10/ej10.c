#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
// PARA LOGRAR LOG2(N) ES NECESARIO PARTICIONAR LAS SUMAS, ASI CADA PROCESO INTERCAMBIA LAS SUMAS CON EL DE DISTANCIA K.
// COMPARTIMOS LA SUMA NO EL VALOR INICIAL.
// IT 0: 0 <-> 1 , VALOR SUMA = 1, 2 <-> 3, VALOR SUMA = 5.
// IT 1: 0 <-> 2, VALOR SUMA = 6, 1 <-> 3, VALOR SUMA = 6.
int main(int argc, char* argv[])
{
    int my_id, num_processes;
    MPI_Init(&argc , &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD , &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    
    int LocalValue = my_id; // el valor inicial a enviar es el id
    int suma = LocalValue; // el registro de la suma
    int recibido; // el valor recibido
    
    printf("Proceso numero : %d , valor de la suma inicial : %d\n" , my_id , suma);
    fflush(stdout);
    
    for (int k = 0; (1 << k) < num_processes; k++)
    {
        int partner = my_id ^ (1 << k); // con quien intercambio la info, mi id xor k².
        MPI_Sendrecv(&suma, 1, MPI_INT, partner, 0,
        &recibido, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //envia y recibe en un mismo llamado.
        suma +=recibido;    
    }
    
    printf("Proceso numero : %d , valor de la suma final : %d\n" , my_id , suma);
    fflush(stdout);
    
    MPI_Finalize();

    return 0;
}