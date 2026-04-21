#include <pthread.h>
#include <assert.h>
#include <stdio.h>

volatile int x = 0;
volatile int y = 0;
void * wr(void *arg) { 
    sleep(1);
    x = 123; 
    y = 1; 
}
void * rd(void *arg) {
        while (!y)
            ;
    assert(x == 123);
}

int main() {
 
    pthread_t t0, t1;
 
    pthread_create(&t0, NULL, wr, NULL);
    pthread_create(&t1, NULL, rd, NULL);
 
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    printf("x : %d y : %d\n", x,y);
    return 0;
}