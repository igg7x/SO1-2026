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
    printf("HEllo world SOY EL PROCESO %i  %i!\n",my_id , num_procs);
   
    if (my_id == 0)
    {
        //--- codigo del proceso 0 
    }else if(my_id == 1){
    // --codigo del proceso 1
    }
    else if(my_id == 2){
    // --codigo del proceso 2
    }
    

    MPI_Finalize();
    return 0;
}
