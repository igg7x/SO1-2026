#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h>

#define N_FILOSOFOS 5
#define ESPERA 500000

pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_mutex_t * izq(int i) { return &tenedor[i]; }
pthread_mutex_t * der(int i) { return &tenedor[(i+1) % N_FILOSOFOS]; }
sem_t s_fil;
void pensar(int i)
{
    printf("Filosofo %d pensando...\n", i);
    usleep(random() % ESPERA);
}
void comer(int i)
{
    printf("Filosofo %d comiendo...\n", i);
    usleep(random() % ESPERA);
}
void tomar_tenedores(int i)
{
    // para el ej3
    // if (i == 3)
    // {
    //     pthread_mutex_lock(izq(i));
    //     pthread_mutex_lock(der(i));
        
    //     /* code */
    // }else {
    // pthread_mutex_lock(der(i));
    // pthread_mutex_lock(izq(i));
    // }
    
    sem_wait(&s_fil);
    pthread_mutex_lock(der(i));
    pthread_mutex_lock(izq(i));

}
void tomar_tenedores_2(int i) {
    int exito = 0;
    while (!exito) {
        pthread_mutex_lock(izq(i)); // Toma el izquierdo
        
        // Intenta tomar el derecho
        if (pthread_mutex_trylock(der(i)) == 0) {
            exito = 1; // Logró tomar ambos
        } else {
            // No pudo tomar el derecho, suelta el izquierdo (la sugerencia del filósofo)
            pthread_mutex_unlock(izq(i));
            
            // Espera un tiempo aleatorio para evitar sincronizarse con el vecino (backoff)
            usleep(random() % 1000); 
        }
    }
}
void dejar_tenedores(int i)
{

    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
    sem_post(&s_fil);
}
void * filosofo(void *arg)
{
    int i = arg - (void*)0;
    while (1) {
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    pensar(i);      
    }
}

int main()
{

pthread_t filo[N_FILOSOFOS];
    int i;
    sem_init(&s_fil , 0 ,N_FILOSOFOS - 1);
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_mutex_init(&tenedor[i], NULL);
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
    
        pthread_join(filo[0], NULL);
    sem_destroy(&s_fil);
    return 0;
}
