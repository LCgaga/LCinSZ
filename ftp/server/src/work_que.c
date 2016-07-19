#include "work_que.h"

void queue_init(pque p,int capcity)
//初始化队列
{
	p->head=NULL;
	p->tail=NULL;
	p->size=0;
//	printf("capcity in queue_init is %d\n",capcity);
	p->capcity=capcity;
	if(pthread_mutex_init(&(p->mutex),NULL)!=0)
	{
		perror("queue_mutex_init");
	}
}

int queue_get(pque p)
{
	pthread_mutex_lock(&(p->mutex));
	if(p->size==0)
	{
		pthread_mutex_unlock(&(p->mutex));
		return -1;
	}
	int result;
	result=p->head->fd;
	p->size--;
	if(p->head==p->tail)
	{
		p->head=NULL;
		p->tail=NULL;
		pthread_mutex_unlock(&(p->mutex));
//		printf("got %d\n",result);
		return result;
	}
	pnode tmp=p->head;
	p->head=p->head->next;
	free(tmp);
	pthread_mutex_unlock(&(p->mutex));
	return result;
}

int queue_insertwork(pque p,int work)
{
//	printf("the size is%d\n",p->size);
	pthread_mutex_lock(&(p->mutex));
//	printf("the capcity is%d\n",p->capcity);
	if(p->size==p->capcity)
	{
//		printf("no way!\n");
		pthread_mutex_unlock(&(p->mutex));
		return -1;
	}
	pnode tmp=(pnode)malloc(sizeof(node));
	tmp->fd=work;
	tmp->next=NULL;
	p->size++;
//	printf("check in queue_insertwork\n");
	if(p->head==NULL)
	{
		p->head=tmp;
		p->tail=tmp;
		pthread_mutex_unlock(&(p->mutex));
//		printf("%d inserted!",tmp->fd);
		return 0;
	}
	p->tail->next=tmp;
	p->tail=tmp;
	pthread_mutex_unlock(&(p->mutex));
	return 0;
}
