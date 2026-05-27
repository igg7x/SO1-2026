#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("pone un entero amigo\n");
        return 0;
    }

    long n = atol(argv[1]);
    long sqrt_n = (long)sqrtl((double)n);
    int is_prime = 1; //
    long divisor = 0;
    // printf("n : %ld , sqrt(n): %ld", n, sqrt_n);
#pragma omp parallel for shared(is_prime, divisor)
    for (long i = 2; i <= sqrt_n; i++)
    {
        if (!is_prime)
            continue;

        if (n % i == 0)
        {
#pragma omp critical // encierro una seccion
            {
                is_prime = 0; //
                divisor = i;
            }
        }
    }

    if (!is_prime)
        printf("%ld NO es primo, divisor: %ld\n", n, divisor);
    else
        printf("%ld ES primo\n", n);

    return 0;
}