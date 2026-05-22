#include <stdio.h>
#include <mpi.h>


int main(int argc, char const *argv[])
{
    // rank es el process ID
    int num_procs , my_id ; 
    MPI_Init(&argc , &argv); // desde aca todos corren el mismo programa -- la cantidad de cores que ternga mi compu
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id); // cada uno va a guardar  ssu ID en la variable my_id        
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    // -- con la bandera -np puedo indicar la cantidad de cores que quiero que corran
    int value; 
    if (my_id == 0)
    {
        printf("manda algo:\t");
        scanf("%d",&value);
        MPI_Send(&value ,1 , MPI_INT ,my_id+1,0,MPI_COMM_WORLD);
    }
    else {
            MPI_Recv(&value, 1 , MPI_INT , my_id-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            if (my_id < num_procs -1)
            {
                MPI_Send(&value ,1 , MPI_INT ,my_id+1,0,MPI_COMM_WORLD);
            }
            printf("SOY EL PROC : %i , RECIVI EL VALUE : %d\n" , my_id , value);
    }
    MPI_Finalize();
    return 0;
}
