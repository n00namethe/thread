#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_THREAD 3
#define SHOP_SIZE 1000
#define LOAD_SIZE 1000
#define CONST_FIVE 5
#define CONST_THREE 3
#define COUNT_STORE 10000
pthread_mutex_t mutex[CONST_FIVE] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_loader = PTHREAD_MUTEX_INITIALIZER;

int shop[CONST_FIVE] = {COUNT_STORE, COUNT_STORE, COUNT_STORE, COUNT_STORE, COUNT_STORE};

void* func(void*arg)
{
    int *i = (int*)arg;
    printf("Я поток № %d зашел в func\n", *i);
    size_t j; 
    for (j = 0; j < CONST_FIVE; j++)
    {
        if (pthread_mutex_trylock(&mutex[j]) == 0)
        {
            if (shop[j] <= 0)
            {
                printf("sold out dates. shop[%ld] = %d\n", j, shop[j]);
            }
            else
            {
                shop[j] -= SHOP_SIZE;
                printf("thread №%d.\t shop[%ld]\t = \t%d\n", *i, j, shop[j]);
                sleep(1);
            }
            pthread_mutex_unlock(&mutex[j]);
        } 
        else
        {
            printf("no store available for buyer, next..\n");
        }
    }
    return NULL;
}

void* loader()
{
    size_t j;
    for (j = 0; j < CONST_FIVE; j++)
    { 
        if (pthread_mutex_trylock(&mutex_loader) == 0)
        {
            if (shop[j] - LOAD_SIZE < 0)
            {
                break;
            }
            else
            {
                shop[j] += LOAD_SIZE;
                sleep(2);
                printf("shop[%ld] + load = %d\n", j, shop[j]);
            }
            pthread_mutex_unlock(&mutex_loader);
        }
        else
        {
            printf("no store available for loader, next try\n");
        }
    }
    return 0;
}

int main()
{
    size_t i;
    pthread_t buyer[COUNT_THREAD];
    pthread_t load;
    int index[COUNT_THREAD];
    while(shop[i] > 0)
    {
        for ( i = 0; i < CONST_THREE; i++)
        {
            index[i] = i + 1;
            if (pthread_create(&buyer[i], NULL, &func, &index[i]) != 0)
            {
                printf("Failed to create tread[%ld]\n", i);
            }
        }
        pthread_create(&load, NULL, loader, NULL);
        for (i = 0; i < CONST_THREE; i++)
        {
            if (pthread_join(buyer[i], NULL) != 0)
            {
                printf("Failed to join thread[%ld]\n", i);
            }
        }
    }
    pthread_cancel(load);
    pthread_mutex_destroy(&mutex[i]);
    return 0;
}