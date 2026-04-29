#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*
 * ESTRUCTURA DE UN MUTEX EN Pthreads:
 * -----------------------------------
 * pthread_mutex_t es un tipo opaco (no es transparente).
 * Internamente contiene:
 *   - Un indicador de estado (locked/unlocked)
 *   - Un identificador del hilo que lo posee (cuando está locked)
 *   - Un contador de bloqueos (para mutex recursivos)
 *   - Información de la política de scheduling
 *
 * El mutex NO se copia por valor - es un objeto interno del sistema.
 * Cuando haces "pthread_mutex_t b = a;" estás copiando los bytes
 * del struct, pero el nuevo mutex es INDEPENDIENTE del original.
 */

// Mutex global para proteger la sección crítica
// PTHREAD_MUTEX_INITIALIZER inicializa el mutex en modo "no bloqueado"
pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;

void *foo(void *arg)
{
    /*
     * TIPOS DE DATOS EN LA RECEPCIÓN DEL MUTEX:
     * -----------------------------------------
     * pthread_create() pasa un puntero (void *) al hilo nuevo.
     * En foo(), arg es un puntero genérico que apunta a &a.
     *
     * Conversión de tipos:
     *   void *arg          →  pthread_mutex_t *m
     *   (el puntero genérico se castea al tipo correcto)
     *
     * &a es de tipo pthread_mutex_t*
     * Se pasa por referencia (dirección de memoria), no por valor.
     */
    pthread_mutex_t *m = (pthread_mutex_t *)arg;

    /*
     * pthread_mutex_lock(m):
     *   - Recibe la dirección del mutex (&a)
     *   - Si el mutex está desbloqueado, lo bloquea y retorna
     *   - Si está bloqueado por otro hilo, espera hasta que se desbloquee
     *   - Si está bloqueado por el mismo hilo, comportamiento depende
     *     del tipo de mutex (PTHREAD_MUTEX_RECURSIVE o normal)
     */
    pthread_mutex_lock(m);

    printf("Foo!\n");
    sleep(1);

    pthread_mutex_unlock(m);
    return NULL;
}

int main()
{
    /*
     * CREACIÓN DEL HILO CON REFERENCIA AL MUTEX:
     * -------------------------------------------
     * pthread_create(&th, NULL, foo, &a);
     *
     * Parámetros:
     *   &th    → dirección donde se almacenará el ID del hilo
     *   NULL   → atributos por defecto del hilo
     *   foo    → función que ejecutará el hilo
     *   &a     → argumento pasado a foo() (puntero a mutex)
     *
     * IMPORTANTE: Se pasa &a (dirección), no a (valor).
     * Si se pasara "a" por valor, el hilo recibiría una COPIA
     * del mutex, que sería independiente del original.
     */
    pthread_t th;
    pthread_create(&th, NULL, foo, &a); // Pasa la dirección del mutex

    /*
     * USO DEL MISMO MUTEX POR REFERENCIA:
     * ------------------------------------
     * pthread_mutex_lock(&a);
     *
     * &a es la dirección de memoria del mutex global.
     * Ambos hilos (principal y foo) usan la MISMA dirección,
     * por lo tanto el MISMO mutex.
     *
     * Si aquí hubiéramos hecho:
     *   pthread_mutex_t b = a;  // COPIA por valor - INCORRECTO
     *   pthread_mutex_lock(&b);
     * Estaríamos bloqueando un mutex DIFERENTE, sin relación con &a.
     */
    pthread_mutex_lock(&a);
    printf("Bar!\n");
    sleep(1);
    pthread_mutex_unlock(&a);

    // Espera a que el hilo foo termine
    pthread_join(th, NULL);

    return 0;
}

// se hace una copia del mutex a para usarlo en el hilo principal, pero esto no es necesario ni correcto, ya que el mutex se debe compartir entre los hilos para proteger la sección crítica. En este caso, ambos hilos deberían usar el mismo mutex a para sincronizar el acceso a la sección crítica.