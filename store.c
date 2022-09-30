#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_THREAD 3
#define SHOP_SIZE 1000
#define LOAD_SIZE 500
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
    for (int j = 0; j < CONST_FIVE; j++)
    {
        if (shop[j] - LOAD_SIZE <= 0)
        {
            printf("sold out dates. shop[%d] = %d\n", j, shop[j]);
            break;
        }
        else
        {
            pthread_mutex_lock(&mutex[j]);
            shop[j] -= SHOP_SIZE;
            printf("thread №%d.\t shop[%d]\t = \t%d\n", *i, j, shop[j]);
            usleep(300000);
            pthread_mutex_unlock(&mutex[j]);
        }
        if ((j == CONST_FIVE - 1) && (shop[j] != 0))
        {
            j = -1;
        }
    }
    return NULL;
}

void* loader()
{
    for (int j = 0; j < CONST_FIVE; j++)
    { 
        if(shop[j] - LOAD_SIZE >= LOAD_SIZE)
        {
            pthread_mutex_lock(&mutex_loader);
            shop[j] += LOAD_SIZE;
            pthread_mutex_unlock(&mutex_loader);
        }
        else
        {
            break;
        }
        usleep(200000);
        printf("shop[%d] + load = %d\n", j, shop[j]);
        if ((j == CONST_FIVE - 1) && (shop[j] != 0))
        {
            j = -1;
        }
    }
    return 0;
}

int main()
{
    pthread_t buyer[COUNT_THREAD];
    int index[COUNT_THREAD];
    for (int i = 0; i < CONST_THREE; i++)
    {
        index[i] = i + 1;
        pthread_create(&buyer[i], NULL, &func, &index[i]);
    }
    pthread_t load;
    pthread_create(&load, NULL, loader, NULL);

    for (int j = 0; j < CONST_THREE; j++)
    {
        pthread_join(buyer[j], NULL);
        pthread_cancel(buyer[j]);
    }
    pthread_cancel(load);
    return 0;
}