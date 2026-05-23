/* calculadora.x */
struct operandos {
    int a;
    int b;
};

program CALCULADORA_PROG {
    version CALCULADORA_VERS {
        int SUMAR(operandos) = 1;
    } = 1;
} = 0x20000001;
/* Numero de programa unico (asignado arbitrariamente) */