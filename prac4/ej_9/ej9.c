#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int my_id , num_processes, Npasos; 
    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD , &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    Npasos = num_processes - 1;
    int local_sum = my_id ;
    printf("Proceso numero : %d , valor de la suma inicial : %d\n" , my_id , local_sum);
    fflush(stdout);
    int value = local_sum;
    for (int i = Npasos; i > 0; i--)
    {
        MPI_Send(&value, 1, MPI_INT, (my_id + 1) % num_processes, 0, MPI_COMM_WORLD);
        MPI_Recv(&value, 1 , MPI_INT , (my_id == 0) ? (num_processes - 1) : (my_id - 1) , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_sum += value;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    printf("Proceso numero : %d , valor de la suma final : %d\n" , my_id , local_sum);
    MPI_Finalize();
    

    return 0;
}
