#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


pthread_mutex_t mutex;
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
                shop[j] -= 1000;
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
            shop[j] += 500;
            pthread_mutex_unlock(&mutex);
            printf("shop[%d] + load = %d\n", j, shop[j]);
            sleep(2);
        }
    }
}

int main()
{
    pthread_t buyer[3];
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("не удалось инициализировать mutex\n");
    }
    int index[3];
    for (int i = 0; i < 3; i++)
    {
        index[i] = i + 1;
        pthread_create(&buyer[i], NULL, &func, &index[i]);
        //sleep(1);
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