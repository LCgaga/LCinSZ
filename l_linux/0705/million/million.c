#include <stdio.h>
#include <pthread.h>
#define NUM 20000000
int m;
pthread_mutex_t mutex;
void* threadfunc(void *p)
{
	int i;
	for(i=0;i<NUM;i++)
	{
		pthread_mutex_lock(&mutex);
		m++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit((NULL));
}
int main()
{
	pthread_mutex_init(&mutex,NULL);
	pthread_t pth;
	pthread_create(&pth,NULL,threadfunc,NULL);
	int i;
	for(i=0;i<NUM;i++)
	{
		pthread_mutex_lock(&mutex);
		m++;
		pthread_mutex_unlock(&mutex);
	}
//	printf("I am the main thread\n");
	pthread_join(pth,NULL);
	pthread_mutex_destroy(&mutex);
	printf("the value is %d!\n",m);
}
