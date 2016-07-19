#include "factory.h"
void factory_init(pfactory pfy,int size,int queue_capcity)
{
	pthread_cond_init(&(pfy->cond),NULL);
	queue_init(&(pfy->que),queue_capcity);
//	printf("factory_init pfy->que.capcity is %d\n",(pfy->que).capcity);
//	printf("size of pfy->que is %d\n",sizeof(pfy->que));
	pthread_mutex_init(&(pfy->cond_mutex),NULL);
	pthread_mutex_init(&(pfy->path_mutex),NULL);
	pfy->size=size;
	pfy->log_fd=open("ftp.log",O_RDWR|O_CREAT|O_APPEND,0600);
}

void factory_start(pfactory pfy,pfunc func)
{
	int i;
	pthread_t* pth=calloc(pfy->size,sizeof(pthread_t));
	for(i=0;i<pfy->size;i++)
	{
//		printf("now I am creating %dth child!\n",i);
		if(pthread_create(pth+i,NULL,func,pfy)!=0)
		{
			perror("pthread_create");
			return ;
		}
	}
}
