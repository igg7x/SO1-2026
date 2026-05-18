#ifndef READWRITE_H
#define READWIRTE_H
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

pthread_mutex_t mutex_escritores = PTHREAD_MUTEX_INITIALIZER 
typedef struct
{

} ;

void rw_lock();
void rw_unlock();
void read();


#endif // READWRITE_H