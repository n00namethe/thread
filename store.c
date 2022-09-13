#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_THREAD 3
#define SHOP_SIZE 1000
#define LOAD_SIZE 500
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int shop[5] = {10000, 10000, 10000, 10000, 10000};


void* func(void*arg)
{
    int *i = (int*)arg;
    printf("Я поток № %d зашел в func\n", *i);
    for (int j = 1; j < 6; j++)
    {
        if (shop[j] == 0)
        {
            exit(1);
        }
        while (shop[j] > 0)
            {   
                pthread_mutex_lock(&mutex);
                if (shop[j] > 0)
                {
                    shop[j] -= SHOP_SIZE;
                }
                pthread_mutex_unlock(&mutex);
                printf("%d.\t shop[%d]\t = \t%d\n", *i, j, shop[j]);
                sleep(1);
            }
    } 
}

void* loader()
{
    for (int j = 1; j < 6; j++)
    {
        while(shop[j] > 0)
        {
        
            pthread_mutex_lock(&mutex);
            shop[j] += LOAD_SIZE;
            pthread_mutex_unlock(&mutex);
            printf("shop[%d] + load = %d\n", j, shop[j]);
            sleep(2);
        }
    }
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
    if (pthread_mutex_destroy(&mutex) != 0)
    {
        perror("Не удалось уничтожить mutex\n");
    }
    pthread_cancel(load);
    return 0;
}