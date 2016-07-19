#include "chat.h"
int b1_pid;
int sh;
int sem;
int sem1;
struct sembuf pp;
int rw;
void func(int sig)
{
	kill(b1_pid,2);
	write(rw,"\n\n\n",3);
	shmctl(sh,IPC_RMID,NULL);
	semctl(sem,0,IPC_RMID);
	semctl(sem1,0,IPC_RMID);	
}
int main()
{
	signal(2,func);
	sh=shmget((key_t)1001,256,0600|IPC_CREAT);
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
	sem=semget((key_t)1001,1,0600|IPC_CREAT);
	if(sem==-1)
	{
		perror("semget");
		return -1;
	}
	sem1=semget((key_t)1101,1,0600|IPC_CREAT);
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
	int rr=open("pipe1",O_RDONLY);
	if(rr==-1)
    {
         perror("pipe1 open failed\n");
         return -1;
    }
	rw=open("pipe2",O_WRONLY);
	if(rw==-1)
    {
         perror("pipe2 open failed\n");
         return -1;
    }
	semop(sem,&pp,1);
	b1_pid=*((int*)p);
//	printf("the pid is %d\n",a1_pid);
	semop(sem,&v,1);
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
//				printf("say:%s\n",buf);
				if(strlen(buf)==1)
				{
					memset(buf,0,sizeof(buf));
					semop(sem1,&v,1);
					continue;
				}
				strcpy(say,"B:");
				strcat(say,buf);
				memset(buf,0,sizeof(buf));
				write(rw,say,strlen(say)-1);
				strncpy(p,say,strlen(say)-1);
				semop(sem1,&v,1);
			}
			if(FD_ISSET(rr,&set))
			{
				semop(sem,&pp,1);
				read(rr,hear,sizeof(hear));
//				printf("I hear that:%s\n",hear);
				if(hear[1]=='\n')
				{
//				    printf("I know that!\n");
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
	return 0;
}
