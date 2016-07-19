#include <stdio.h>
#include <pthread.h>
pthread_mutex_t mutex;
pthread_cond_t cond;
void* threadfunc(void *p)
{
	printf("I am going to wait.\n");
	pthread_cond_wait(&cond,&mutex);
	printf("I am back.\n");
	pthread_exit((NULL));
}
int main()
{
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_t pth;
	pthread_create(&pth,NULL,threadfunc,NULL);
//	printf("I am the main thread\n");
	sleep(3);
	printf("now I am waking him.\n");
	pthread_cond_signal(&cond);
	pthread_join(pth,NULL);
	printf("over\n");
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}
