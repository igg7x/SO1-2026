#include "ch_sync.h"
#include <pthread.h>

void channel_init(Channel_S *channel)
{
    channel->has_value = 0;
    channel->reader_ready = 0;
    pthread_cond_init(&channel->cond_writer, NULL);
    pthread_cond_init(&channel->cond_reader, NULL);
    pthread_mutex_init(&channel->mutex, NULL);
}

void channel_write(Channel_S *channel, int value)
{
    pthread_mutex_lock(&channel->mutex);
    // esperamos hasta que el valor anterior haya sido consumido
    while (channel->has_value)
        pthread_cond_wait(&channel->cond_writer, &channel->mutex);

    channel->has_value = 1;
    channel->value = value;
    // si hay un lector enviamos la señal para que lea
    if (channel->reader_ready)
        pthread_cond_signal(&channel->cond_reader);

    // esperamos de nuevo hasta que el valor anterior sea leido
    while (channel->has_value)
        pthread_cond_wait(&channel->cond_writer, &channel->mutex);

    pthread_mutex_unlock(&channel->mutex);
}

int channel_read(Channel_S *channel)
{
    pthread_mutex_lock(&channel->mutex);
    // establecemos que hay un lector listo para leer
    channel->reader_ready = 1;
    // mientras no haya un valor para leer esperamos
    while (!channel->has_value)
    {
        pthread_cond_wait(&channel->cond_reader, &channel->mutex);
    }
    // consumimos el valor que recibimos
    int value_recived = channel->value;
    channel->reader_ready = 0;
    channel->has_value = 0;
    // siganaleamos al writer que el valor  ya fue consumido
    pthread_cond_signal(&channel->cond_writer);
    pthread_mutex_unlock(&channel->mutex);
    return value_recived;
}