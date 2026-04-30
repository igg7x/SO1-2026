#include "mini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int is_key_exists(char *key, Item *store[], int max_items)
{
    for (int i = 0; i < max_items; i++)
    {
        if (store[i] != NULL && !store[i]->eliminated && strcmp(store[i]->key, key) == 0)
        {
            return 1; // La clave existe
        }
    }
    return 0; // La clave no existe
}

void put_item(Item *item, Item *store[], int max_items)
{
    // Implementar la función para guardar el item en el store
    for (int i = 0; i < max_items; i++)
    {
        if (store[i] == NULL || store[i]->eliminated)
        {
            store[i] = item;
            return;
        }
    }
}

Item *get_item(char *key, Item *store[], int max_items)
{
    // Implementar la función para buscar el item en el store
    for (int i = 0; i < max_items; i++)
    {
        if (store[i] != NULL && !store[i]->eliminated && strcmp(store[i]->key, key) == 0)
        {
            return store[i];
        }
    }
    return NULL;
}

void del_item(char *key, Item *store[], int max_items)
{
    // Implementar la función para eliminar el item del store
    for (int i = 0; i < max_items; i++)
    {
        if (store[i] != NULL && !store[i]->eliminated && strcmp(store[i]->key, key) == 0)
        {
            store[i]->eliminated = 1;
            return;
        }
    }
}