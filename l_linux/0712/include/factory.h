#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "work_que.h"
#define FILENAME "hello.avi"
typedef  void* (*pfunc)(void*);

typedef struct factory_t{
	pthread_cond_t cond;
	pthread_mutex_t cond_mutex;
	que que;
	int size;
}factory,*pfactory;

typedef struct{
	int num;
	char cont[1000];
}seg,*pseg;

void factory_init(pfactory pfy,int size,int queue_capcity);
void factory_start(pfactory pfy,pfunc func);

void* child_work(void* pfy);
int sendfile(int sock,char* filename);
int sendbuf(int sock,void* p,int len);
#endif
