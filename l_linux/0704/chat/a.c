#include "chat.h"
int a1_pid;
int sem;
int sem1;
struct sembuf pp;
int rw;
int sh;
void func(int sig)
{	
//	printf("the num of a1 is %d\n",a1_pid);
	kill(a1_pid,2);
	write(rw,"\n\n\n",3);
	shmctl(sh,IPC_RMID,NULL);
	semctl(sem,0,IPC_RMID);
	semctl(sem1,0,IPC_RMID);
	remove("pipe1");
	remove("pipe2");
}
int main()
{
	signal(2,func);
//动态内存的申请与连接
	sh=shmget((key_t)1000,256,0600|IPC_CREAT);
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
	sem=semget((key_t)1000,1,0600|IPC_CREAT);
	if(sem==-1)
	{
		perror("semget");
		return -1;
	}
	sem1=semget((key_t)1100,1,0600|IPC_CREAT);
	if(sem==-1)
	{
		perror("semget");
		return -1;
	}

	if(semctl(sem,0,SETVAL,0)==-1)
	{
		perror("semctl");
		return -1;
	}
	pp.sem_num=0;
	pp.sem_op=-1;
	pp.sem_flg=SEM_UNDO;
	struct sembuf v;
	v.sem_num=0;
	v.sem_op=1;
	v.sem_flg=SEM_UNDO;
//管道文件的打开
	mkfifo("pipe1",0600);
	mkfifo("pipe2",0600);
	rw=open("pipe1",O_WRONLY);
	if(rw==-1)
	{
		perror("pipe1 open failed\n");
		return -1;
	}
	int rr=open("pipe2",O_RDONLY);
	if(rr==-1)
     {
         perror("pipe2 open failed\n");
         return -1;
     }
	semop(sem,&pp,1);
    a1_pid=*((int*)p);
//	printf("the pid is %d\n",a1_pid);
	semop(sem,&v,1);
//	kill(a1_pid,10);
//	strcpy(p,"abc");
//	printf("%s",p);
	char buf[254]={0};
	char say[256]={0};
	char hear[256]={0};
	fd_set set;
	while(1)
	{
		FD_ZERO(&set);
		FD_SET(0,&set);
		FD_SET(rr,&set);
		if(select(rr+1,&set,NULL,NULL,NULL)>0)
		{
			if(FD_ISSET(0,&set))
			{
				semop(sem,&pp,1);
				memset(say,0,sizeof(say));
				read(0,buf,sizeof(buf)-1);
				if(strlen(buf)==1)
				{
					memset(buf,0,sizeof(buf));
					semop(sem1,&v,1);
					continue;
				}
//				printf("say:%s\n",buf);
				strcpy(say,"A:");
				strcat(say,buf);
				memset(buf,0,sizeof(buf));
//				printf("%s\n",say);
				write(rw,say,strlen(say)-1);
				memset(p,0,256);
				strncpy(p,say,strlen(say)-1);
				semop(sem1,&v,1);
			}
			if(FD_ISSET(rr,&set))
			{
				semop(sem,&pp,1);
				read(rr,hear,sizeof(hear));
				if(hear[1]=='\n')
				{
//					printf("I know that!\n");
					kill(getpid(),2);
					break;
				}
				strcpy(p,hear);
//				printf("%s\n",hear);
				memset(hear,0,sizeof(hear));
				semop(sem1,&v,1);
			}
		}
	}
	remove("pipe1");
	remove("pipe2");
	return 0;
}

