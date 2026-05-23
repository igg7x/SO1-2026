#include "calculadora.h"
#include <stdio.h>
/* ONC RPC requiere devolver un puntero */
int * sumar_1_svc(operandos *argp, struct svc_req *rqstp) {
    static int resultado;
    printf("Servidor: sumar %d + %d\n", argp->a, argp->b);
    resultado = argp->a + argp->b;
    return &resultado;
}