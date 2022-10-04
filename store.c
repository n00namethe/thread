#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_OF_BUYER 3
#define SHOP_SIZE 1000
#define LOAD_SIZE 2000
#define NUM_OF_MASSIV_SHOP 5
#define COUNT_STORE 10000

pthread_mutex_t mutex[NUM_OF_MASSIV_SHOP];
pthread_mutex_t mutex_loader = PTHREAD_MUTEX_INITIALIZER;

int shop[NUM_OF_MASSIV_SHOP] = {};

int init(void)
{   
    for (int i = 0; i < NUM_OF_MASSIV_SHOP; i++)
    {
        shop[i] = COUNT_STORE;
        printf("init shop[%d] = %d\n", i, COUNT_STORE);
        if (pthread_mutex_init(&mutex[i], NULL) != 0)
        {
            perror("mutex[%d] don't init\n");
            return -1;
        }
    }
    return 0;
}

void *func(void*arg)
{
    int *i = (int*)arg;
    printf("Я поток № %d зашел в func\n", *i);
    int buyer_size = 0;
    while(buyer_size < 20000)
    {
        for (int j = 0; j < NUM_OF_MASSIV_SHOP; j++)
        {
            if (pthread_mutex_trylock(&mutex[j]) == 0)
            {
                if (shop[j] == 0)
                {
                    printf("sold out dates. shop[%d] = %d\n", j, shop[j]);
                    pthread_mutex_unlock(&mutex[j]);
                    sleep(10);
                }
                else
                {
                    shop[j] -= SHOP_SIZE;
                    buyer_size += SHOP_SIZE;
                    printf("buyer get from shop[%d] = %d\n", j, buyer_size);
                    printf("thread №%d.\t shop[%d]\t = \t%d\n", *i, j, shop[j]);
                    usleep(250000);
                    pthread_mutex_unlock(&mutex[j]);
                }
            } 
            else
            {
                printf("this shop[%d] don't available for buyer, go next..\n", j);
                usleep(300000);
            }
        }
    }
    return NULL;
}

void *loader()
{
    while(1)
    {
        for (int j = 0; j < NUM_OF_MASSIV_SHOP; j++)
        { 
            if (pthread_mutex_trylock(&mutex_loader) == 0)
            {
                shop[j] += LOAD_SIZE;
                sleep(2);
                printf("shop[%d] + load = %d\n", j, shop[j]);
                pthread_mutex_unlock(&mutex_loader);
            }
            else
            {
                printf("this shop[%d] don't available for loader, go next\n", j);
            }
        }
    }
    return NULL;
}

int main()
{
    int i;
    if (init() != 0)
    {
        printf("init not success, prorgamm stop\n");
        return -1;
    }
    pthread_t buyer[NUM_OF_BUYER];
    int index[NUM_OF_BUYER];
    for (i = 0; i < NUM_OF_BUYER; i++)
    {
        index[i] = i + 1;
        if (pthread_create(&buyer[i], NULL, &func, &index[i]) != 0)
        {
            printf("Failed to create thread[%d]\n", i);
        }
    }
    pthread_t load;
    pthread_create(&load, NULL, loader, NULL);
    for (i = 0; i < NUM_OF_BUYER; i++)
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