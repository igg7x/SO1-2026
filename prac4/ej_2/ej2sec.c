#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 50000000
double Sumatotal = 0;
void sumar(double arr[],long int inicio,long int cantidad)
{
    double suma = 0;
    for (long int i = 0; i < cantidad; i++)
        suma += arr[inicio + i];
    #pragma omp critical
        Sumatotal += suma;
    
}

void rellenar(double arr[], long int inicio, long int cantidad)
{
    for (long int i = 0; i < cantidad; i++) {
        arr[inicio + i] = 1.0;
    }
}
//division of task in for loop for each thread
void dividir(double arr[], int tamanio)
{
    long int iam, nt, istart, ifinal;
    #pragma omp parallel default(shared) private(iam, nt, istart, ifinal)
    {
        iam = omp_get_thread_num(); // threat id

        nt = omp_get_num_threads(); // number of threats
        
        ifinal = tamanio / nt; // integer division of array
        
        istart = iam * ifinal; // starting index

        if(iam == nt - 1)
            ifinal = tamanio - istart; // last do more
        rellenar(arr, istart, ifinal);
        sumar(arr, istart, ifinal);
    }
}


int main()
{

    double *arr = malloc(N * sizeof(double));

    if (arr == NULL) {
        printf("No se pudo reservar memoria\n");
        return 1;
    }

    

    dividir(arr, N);

    printf("%f\n", Sumatotal);

    free(arr);
    return 0;
}