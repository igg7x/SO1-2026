#include <pthread.h>
#include <assert.h>
#include <stdio.h>
int x = 0, y = 0, a = 0, b = 0;

void * foo(void *arg) { x = 1; 
                        a = y; 
                        return NULL; }
void * bar(void *arg) { y = 1; 
                        b = x; 
                        return NULL; }

int main() {
 
    pthread_t t0, t1;
 
    pthread_create(&t0, NULL, foo, NULL);
    pthread_create(&t1, NULL, bar, NULL);
 
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
 
    printf("a %d : b:%d\n" , a, b);
    if(a==0 && b==0)
        printf("\n\n\n\n\n\n");
    assert (a || b);
    return 0;
}