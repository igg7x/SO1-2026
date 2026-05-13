#include <pthread.h>
#include <stdio.h>

phtread_cond_t cond1 , cond2 = PTHREAD_COND_INITIALIZER

void * thread_a(){
    printf("a1");
    pthread_cond_signal(&cond)
    pthread_cond_wait(&cond)
    printf("a2");
    return NULL
}
void * thread_b(){
    pthread_cond_wait(&cond2)
    printf("b1");
    pthread_cond_signal(&cond)
    printf("b2");
    return NULL
}


int main(int argc, char const *argv[])
{
    pthread_t a , b 
    pthread_create()
    pthread_create()
    pthread_join()
    pthread_join()

    return 0;
}
