#include "chat.h"
void func(int sig)
{
//	printf("num is %d\n",sig);
}
int main()
{
	signal(10,func);

//动态内存的申请与连接
	int sh=shmget((key_t)1000,256,0600|IPC_CREAT);
	if(sh==-1)
	{
		perror("sheget");
		return -1;
	}
	char* p=shmat(sh,NULL,0);
	if((int)p==-1)
	{
		perror("shmat");
		return -1;
	}
//信号量的申请与操作的设置
	int sem=semget((key_t)1000,1,0600|IPC_CREAT);
	if(sem==-1)
	{
		perror("semget");
		return -1;
	}
	int sem1=semget((key_t)1100,1,0600|IPC_CREAT);
    if(sem==-1)
    {
        perror("semget");
        return -1;
    }
	if(semctl(sem1,0,SETVAL,0)==-1)
	{
		perror("semctl");
		return -1;
	}
	struct sembuf pp;
	pp.sem_num=0;
	pp.sem_op=-1;
	pp.sem_flg=SEM_UNDO;
	struct sembuf v;
	v.sem_num=0;
	v.sem_op=1;
	v.sem_flg=SEM_UNDO;
	*((int*)p)=getpid();
//	printf("my pid is %d,the p is %d\n",getpid(),*((int*)p));
//	sleep(2);
//	printf("now I am v it!\n");
	semop(sem,&v,1);
//	char p[256]={0};
	while(1)
	{
		semop(sem1,&pp,1);
		if(strlen(p)==0)
		{
			semop(sem,&v,1);
			continue;
		}
		printf("%s\n",p);
		memset(p,0,256);
		semop(sem,&v,1);	
	}
	return 0;
}
