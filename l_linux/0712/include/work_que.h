#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"
typedef struct fd_node{
	int fd;
	struct fd_node* next;
}node,*pnode;

typedef struct queue{
	pnode head;
	pnode tail;
	int size;
	int capcity;
	pthread_mutex_t mutex;
}que,*pque;

void queue_init(pque p,int capcity);
int queue_get(pque);//返回队首元素，返回-1表示为空
int queue_insertwork(pque,int);//插入队尾，返回0表示插入成功，返回-1表示插入失败
#endif
