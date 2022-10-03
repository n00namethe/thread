#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_OF_THREAD 3
#define SHOP_SIZE 1000
#define LOAD_SIZE 1000
#define NUM_OF_MASSIV_SHOP 5
#define COUNT_STORE 10000

pthread_mutex_t mutex[NUM_OF_MASSIV_SHOP];
pthread_mutex_t mutex_loader = PTHREAD_MUTEX_INITIALIZER;

int shop[NUM_OF_MASSIV_SHOP] = {COUNT_STORE, COUNT_STORE, COUNT_STORE, COUNT_STORE, COUNT_STORE};

void* func(void*arg)
{
    int *i = (int*)arg;
    printf("Я поток № %d зашел в func\n", *i);
    for (int j = 0; j < NUM_OF_MASSIV_SHOP; j++)
    {
        if (pthread_mutex_init(&mutex[j], NULL) != 0)
        {
            perror("mutex don't init\n");
        }
        if (pthread_mutex_trylock(&mutex[j]) == 0)
        {
            if (shop[j] - SHOP_SIZE < 0)
            {
                printf("sold out dates. shop[%d] = %d\n", j, shop[j]);
            }
            else
            {
                shop[j] -= SHOP_SIZE;
                printf("thread №%d.\t shop[%d]\t = \t%d\n", *i, j, shop[j]);
                sleep(1);
            }
            pthread_mutex_unlock(&mutex[j]);
        } 
        else
        {
            printf("this shop[%d] don't available for buyer, go next..\n", j);
            usleep(300000);
        }
    }
    return NULL;
}

void* loader()
{
    for (int j = 0; j < NUM_OF_MASSIV_SHOP; j++)
    { 
        if (pthread_mutex_trylock(&mutex_loader) == 0)
        {
            if (shop[j] - LOAD_SIZE < 0)
            {
                printf("loader stop\n");
            }
            else
            {
                shop[j] += LOAD_SIZE;
                sleep(2);
                printf("shop[%d] + load = %d\n", j, shop[j]);
            }
            pthread_mutex_unlock(&mutex_loader);
        }
        else
        {
            printf("this shop[%d] don't available for loader, go next\n", j);
        }
    }
    return NULL;
}

int main()
{
    int i;
    pthread_t buyer[NUM_OF_THREAD];
    pthread_t load;
    int*index;
    int a;
    index = &a;
    for (i = 0; i < NUM_OF_THREAD; i++)
    {
        *index = i + 1;
        if (pthread_create(&buyer[i], NULL, &func, index) != 0)
        {
            printf("Failed to create thread[%d]\n", i);
        }
    }
    pthread_create(&load, NULL, loader, NULL);
    for (i = 0; i < NUM_OF_THREAD; i++)
    {
        if (pthread_join(buyer[i], NULL) != 0)
        {
            printf("Failed to join thread[%d]\n", i);
        }
    }
    for (i = 0; i < NUM_OF_MASSIV_SHOP; i++)
    {
        int destroy;
        destroy = pthread_mutex_destroy(&mutex[i]);
        if (destroy != 0)
        {
            printf("pthread_mutex_destroy = %d\n", destroy);
        }
    }
    pthread_cancel(load);
    return 0;
}