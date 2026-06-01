// compilar : mpicc file.c -o binarry.out
// ejecutar :mpirun -np [number of processes] ./binary_file
// -- con la bandera -np puedo indicar la cantidad de cores que quiero que corran
//  mpirun --use-hwthread-cpus -np 4 ./ej8.out

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
// primera version , asumimos que el largo del arreglo es divisible por la cantidad de procesos, es decir, que cada proceso va a recibir la misma cantidad de elementos del arreglo, y que el proceso 0 va a ser el encargado de crear el arreglo y llenarlo con los valores del 1 al len, luego va a repartir esos valores entre los procesos, incluido el mismo, cada proceso va a sumar los valores que le toco, es decir, el valor que recibio mas los valores que le toco a los demas procesos, y luego el proceso 0 va a recibir la suma de los ID de todos los procesos

int main(int argc, char const *argv[])
{
    int num_procs, my_id;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    if (argc < 2)
    {
        if (my_id == 0)
            printf("Usage: %s <length of array>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int len = atoi(argv[1]);
    int cant_ele = len / num_procs; // cantidad de elementos que va a recibir cada proceso

    int *arr = NULL;
    if (my_id == 0) // solo el proceso master va a crear el arreglo y llenarlo con los valores del 1 al len, luego va a repartir esos valores entre los procesos, incluido el mismo
    {
        arr = malloc(sizeof(int) * len);
        for (int i = 0; i < len; i++)
            arr[i] = i + 1;
    }

    int *local_arr = malloc(sizeof(int) * cant_ele);
    MPI_Scatter(arr, cant_ele, MPI_INT, local_arr, cant_ele, MPI_INT, 0, MPI_COMM_WORLD); // el proceso 0 va a enviar un valor del arreglo a cada proceso, incluido el mismo
    int local_sum = 0;
    for (int i = 0; i < cant_ele; i++)
    {
        local_sum += local_arr[i]; // cada proceso va a sumar los valores que le toco, es decir, el valor que recibio mas los valores que le toco a los demas procesos
    }
    int reduction_result = 0;
    MPI_Reduce(&local_sum, &reduction_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); // el proceso 0 va a recibir la suma de los ID de todos los procesos
    if (my_id == 0)
    {
        printf("La suma de los ID de todos los procesos es: %d\n", reduction_result);
        free(arr);
    }
    free(local_arr);
    MPI_Finalize();
    return 0;
}
