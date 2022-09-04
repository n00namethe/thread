#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


void *func(void *arg) {
	int *ptr = (int*)arg;
	printf("%d\n", *ptr);
	int ret = pthread_self();
	if (ret < 0)
	{
		exit(1);
	}
}

int main(void)
{
	pthread_t tid[5];
	int *s;
	for (int i = 0; i < 5; ++i)
	{
		pthread_create(&tid[i], NULL, func, (void *) &i);
	}
	for (int i = 0; i < 5; ++i)
	{
		pthread_join(tid[i], (void **)&s);
	}
	return 0;
}