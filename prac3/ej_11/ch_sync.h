#ifndef CH_SYNC
#define CH_SYNC
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct channel
{
    int value;
    int reader_ready; // hay un lector listo para leer
    int has_value;    // hay un valor para leer
    pthread_mutex_t mutex;
    pthread_cond_t cond_writer;
    pthread_cond_t cond_reader;
} Channel_S;

void channel_init(Channel_S *);
void channel_write(Channel_S *, int v);
int channel_read(Channel_S *);
#endif // !CH_SYNC