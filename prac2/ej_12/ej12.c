#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// creo un mutex para proteger la seccion critica del programa 
pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER; 

void *foo(void *_arg)
{
    pthread_mutex_lock(&a);
    printf("Foo!\n");
    sleep(1);
    pthread_mutex_unlock(&a);
    return NULL;
}

// en este programa se crea un hilo que ejecuta la funcion foo, la cual imprime "Foo!" y luego duerme por 1 segundo. Mientras tanto, el hilo principal imprime "Bar!" y luego espera a que el hilo foo termine su ejecucion antes de finalizar el programa. El mutex a se utiliza para asegurar que no haya condiciones de carrera al acceder a la seccion critica del programa, que en este caso es la impresion de los mensajes en pantalla.
int main()
{

    // creo un hilo para ejecutar la funcion foo en paralelo con el hilo principal
    pthread_t th;
    pthread_create(&th, NULL, foo, NULL);
    
    // creo una copia del mutex a para usarlo en el hilo principal y evitar condiciones de carrera al acceder a la seccion critica
    pthread_mutex_t b = a;
    
    pthread_mutex_lock(&b);

    printf("Bar!\n");
    sleep(1);

    pthread_mutex_unlock(&b);
    
    pthread_join(th, NULL);
    return 0;
}
