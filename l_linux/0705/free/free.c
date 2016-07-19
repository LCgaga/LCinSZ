#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void func(void* p)
{
	printf("%s is contented\n",(char*)p);
	printf("now I am going to free it!\n");
	free(p);
	printf("%s is contented\n",(char*)p);
}
void* threadfunc(void *p)
{
	p=malloc(20);
	pthread_cleanup_push(func,p);
	strcpy((char*)p,"hello");
	printf("%s is contented\n",(char*)p);
	char ch[20]={0};
	read(0,ch,20);
	pthread_cleanup_pop(1);
	pthread_exit((void*)4);
}
int main()
{
	void **pp;
	printf("I have recevied %d!\n",(int)(*pp));
	pthread_t pth;
	void* p;
	pthread_create(&pth,NULL,threadfunc,p);		
//	printf("I am the main thread\n");
	sleep(1);
	pthread_cancel(pth);//子进程被杀死的时候exit没能正常返回4,如果是正常结束就能返回4
	pthread_join(pth,pp);
	printf("I have recevied %d!\n",(int)(*pp));
}
