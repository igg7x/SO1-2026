#include <pthread.h>
#include <stdio.h>

#define MAX_MILIS  40000

int milis_count  = 0 ;


void * puerta1(void *arg){
    for (int i = 0; i < MAX_MILIS / 2 ; i++)
    {
       
        // seccion critica
        milis_count++ ; 
    } 
}
void * puerta2(void *arg){
    
    for (int i = 0; i < MAX_MILIS / 2 ; i++)
    { 
        // seccion critica
        milis_count++;
       
    }
    return NULL;
}

int main(){

    pthread_t t1 ,t2 ; 
    pthread_create(&t1 ,NULL , puerta1, NULL);
    pthread_create(&t2 ,NULL , puerta2 ,NULL); 

    pthread_join(t1 ,NULL);
    pthread_join(t2 ,NULL);

    printf("En otto hay %d milis\n", milis_count);    

    return 0 ; 
}