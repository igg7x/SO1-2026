#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Una barber´ıa tiene una sala de espera con N sillas y
// un barbero. Si no hay clientes para atender, el barbero se pone a dormir. Si un cliente llega y todas
// las sillas est´an ocupadas, se va. Si el barbero est´a ocupado pero hay sillas disponibles, se sienta en
// una y espera a ser atendido. Si el barbero est´a dormido, despierta al barbero. El cliente y el barbero
// deben ejecutar concurrentemente las funciones me_cortan() y cortando() y al terminar los dos ejecutar
// concurrentemente pagando() y me_pagan()

#define N_SILLAS 3
#define N_CLIENTS 20

sem_t sillas,barbero,pagar,cliente; 
void pagando(int index){
    printf("Cliente %d pagando\n",index );
}
void me_pagan(){
    printf("Cliente  me pago\n");

}

void* me_cortan(void *arg){

    int index = arg - (void*)0;
    if (sem_trywait(&sillas) == 0) // si haria un wait se los clientes se quedan espernado hasta que alla una silla libre , con esya implmentacion se van directamente 
    {
        sem_post(&cliente);
        printf("Cliente %d se sento , espera ser atendido ....\n" ,index) ;
        sem_wait(&barbero); // esperamos al barbero
        printf("Cliente %d cortandose\n" , index);
        sem_post(&sillas) ;
        sleep(random() % 3);
        pagando(index);    
        sem_post(&pagar);
    }else{
        printf("Todas las sillas ocupadas , cliente se fue\n");
    }
    return NULL;
}


void* cortando(void *arg){

    
    while (1) {
        sem_wait(&cliente);      // Se bloquea AQUÍ de verdad (así duerme si no hay nadie)
        printf("Barbero despierto, llamando cliente...\n");
        
        sem_post(&barbero);      // Le dice al cliente: "ven a la silla de corte"
        
        // --- AQUÍ SE EJECUTA EL CORTE CONCURRENTE ---
        printf("Barbero cortando el pelo...\n");
        sleep(random() % 3);     // El barbero es quien debería marcar el tiempo del corte
        
        // sem_post(&corte_terminado); // Le dice al cliente: "ya terminé, puedes pararte"
        
        // --- SECCIÓN DE PAGO ---
        sem_wait(&pagar);        // Espera a que el cliente le dé el dinero
        me_pagan();              // Guarda el dinero
    }
    return NULL;
}



int main(int argc, char const *argv[])
{

    sem_init(&barbero , 0 ,0);
    sem_init(&sillas , 0 , N_SILLAS);
    sem_init(&pagar,0,0);
    sem_init(&cliente ,0,0);
    pthread_t barber ;
    pthread_create(&barber , NULL,cortando,NULL);

    pthread_t clients[N_CLIENTS];
    for (int i = 0; i < N_CLIENTS; i++)
    {
        pthread_create(&clients[i] ,NULL,me_cortan , i + (void*)0);
    }
    
    for (int i = 0; i < N_CLIENTS; i++)
    {
        pthread_join(clients[i] ,NULL);
    }
    pthread_join(barber ,NULL);

    return 0;
}
