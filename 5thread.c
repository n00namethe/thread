#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MIL 10

void *thread_calc(void *arg)
	{
	int index = *(int*)arg;
	int a = 0;
	for (int i = 0; i < MIL; ++i)
		{
			++a;
			printf("thread %d tick %d\n",index, a);
		}
	return NULL;
	}

int main(void)
{
	int i, i1;
	int index[5];
	int *s;
	pthread_t thread[5];	

	for (int i = 0; i < 5; ++i)
	{
		index[i]=i + 1;

		pthread_create(&thread[i], NULL, thread_calc, (void *) &index[i]);

		
	}

	for (int i1 = 0; i1 < 5; ++i1)
	{
		//printf("id thread: %d\n", index[i1]);
		pthread_join(thread[i1], (void **) &s);
		pthread_cancel(thread[i1]);
	}
	return 0;
}