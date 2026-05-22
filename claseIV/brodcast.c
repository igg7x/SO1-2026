#include <stdio.h>
#include <mpi.h>


int main(int argc, char const *argv[])
{
    int num_procs , my_id ; 
    MPI_Init(&argc , &argv); // desde aca todos corren el mismo programa -- la cantidad de cores que ternga mi compu
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id); // cada uno va a guardar  ssu ID en la variable my_id        
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int value ;
    if (my_id == 0)
    {
        printf("ingresa un valor");
        scanf("%d", &value);
    }else{
        printf("proceso %i antes del brodcast \n" , my_id);
    }
    MPI_Bcast(&value , 1, MPI_INT ,0,MPI_COMM_WORLD); // TODOS LOS PROCESO SE QUESDAN BLOQUEADOS ACA
    printf("proceso %i despues del brodcast , value : %d\n" , my_id , value);
    MPI_Finalize();
    return 0;
}
