#include <stdio.h>
#include <pthread.h>
void* threadfunc(void *p)
{
	printf("I am child thread%d!\n",(int)p);
}
int main()
{
	pthread_t pth[10];
	int i;
	for(i=0;i<10;i++)
	{
		pthread_create(&pth[i],NULL,threadfunc,(void*)i);		
		printf("I am the main thread\n");
	}
	sleep(1);
}
