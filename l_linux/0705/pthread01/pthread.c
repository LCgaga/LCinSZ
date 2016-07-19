#include <stdio.h>
#include <pthread.h>
void* threadfunc(void *p)
{
	printf("I am child thread%d!\n",(int)p);
	pthread_exit((void*)2);
	printf("I am the thread\n");
}
int main()
{
	void** p;
	pthread_t pth;
	pthread_create(&pth,NULL,threadfunc,(void*)1);		
	printf("I am the main thread\n");
	pthread_join(pth,p);
	printf("I have recevied %d!\n",(int)(*p));
}
