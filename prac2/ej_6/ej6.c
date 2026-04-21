#include <pthread.h>
#include <stdio.h>

#define MAX_MILIS  40000

// static inline void incl(int *p) {
//     asm("incl %0" : "+m"(*p) : : "memory");
// }

static inline void incl(int *p) {
    asm("lock; incl %0" : "+m"(*p) : : "memory");
}

int milis_count  = 0 ;
volatile int  flags[2] = {0} ; 
volatile int turno = 0; // turno 0 puerta de otto1 
               // turno 1 puerta de otto2 

void * puerta1(void *arg){
    for (int i = 0; i < MAX_MILIS / 2 ; i++)
    {
        flags[0] = 1;
        turno = 1;
        while(flags[1] && turno == 1){;}
        // seccion critica
        //incl(&milis_count); 
        milis_count = milis_count + 1;
        //printf("T1  milis actuales %d\n", milis_count);
        //printf("X ");
        flags[0] = 0;
    }
    return NULL;
}
void * puerta2(void *arg){
    
    for (int i = 0; i < MAX_MILIS / 2 ; i++)
    {
        flags[1] = 1;
        turno = 0;
        while(flags[0] && turno == 0){;}
        // seccion critica
        //incl(&milis_count); 
        milis_count = milis_count + 1;
        //printf("T2  milis actuales %d\n", milis_count);
        //printf("O");
        flags[1] = 0;
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

//-o2 optimizacion
//S
