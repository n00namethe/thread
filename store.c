#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_THREAD 3
#define SHOP_SIZE 1000
#define LOAD_SIZE 500
#define N 5
#define COUNT_STORE 10000
pthread_mutex_t mutex[N] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_loader = PTHREAD_MUTEX_INITIALIZER;

int shop[N] = {COUNT_STORE, COUNT_STORE, COUNT_STORE, COUNT_STORE, COUNT_STORE};

void* func(void*arg)
{
    int *i = (int*)arg;
    printf("Я поток № %d зашел в func\n", *i);
    while (1)
    {    
        for (int j = 0; j < N; j++)
        {
            if (pthread_mutex_trylock(&mutex[j]) == 0)
            {
                if ((shop[j] == 0) || (shop[j] == LOAD_SIZE))
                {
                    printf("sold out dates. shop[%d] = %d\n", j, shop[j]);
                    goto end;
                }
                else
                {
                    shop[j] -= SHOP_SIZE;
                    printf("thread №%d.\t shop[%d]\t = \t%d\n", *i, j, shop[j]);
                    usleep(300000);
                }
                pthread_mutex_unlock(&mutex[j]);
                break;
            }
            else
            {
                printf("mutex don't lock, wait...\n");
            }
        }
    }
    end: return 0;
}

void* loader()
{
    for (int j = 1; j < 6; j++)
    {
        while(shop[j] > 0)
        {
        
            pthread_mutex_lock(&mutex_loader);    
            shop[j] += LOAD_SIZE;
            pthread_mutex_unlock(&mutex_loader);
            printf("shop[%d] + load = %d\n", j, shop[j]);
            sleep(2);
        }
    }
    return 0;
}

int main()
{
    pthread_t buyer[COUNT_THREAD];
    int index[COUNT_THREAD];
    for (int i = 0; i < 3; i++)
    {
        index[i] = i + 1;
        pthread_create(&buyer[i], NULL, &func, &index[i]);
    }
    pthread_t load;
    pthread_create(&load, NULL, loader, NULL);

    for (int j = 0; j < 3; j++)
    {
        pthread_join(buyer[j], NULL);
        pthread_cancel(buyer[j]);
    }
    for (int i = 0; i > 4; i++)
    {
        if (pthread_mutex_destroy(&mutex[i]) != 0)
        {
            perror("Не удалось уничтожить mutex\n");
        }
    }
    pthread_cancel(load);
    return 0;
}