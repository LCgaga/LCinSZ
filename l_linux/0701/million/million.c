#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
int main()
{
/*	int sh=shmget(IPC_PRIVATE,4,0600);
	if(sh==-1)
	{
		perror("shmeget");
		return -1;
	}
*/
	int* p=shmat(2424836,NULL,0);
	if((int)p==-1)
	{
		perror("shmat");
		return -1;
	}
	*p=0;
/*	int sem=semget(IPC_PRIVATE,1,0600|IPC_CREAT);
	if(sem==-1)
	{
		perror("semget");
		return -1;
	}
*/	
	int sem=163840;
	if(semctl(163840,0,SETVAL,1)==-1)
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
	int pid=fork();
	if(pid>0)
	{
		int i=0;
		while(i<10000000)
		{
			semop(sem,&pp,1);
			(*p)++;
			semop(sem,&v,1);
			i++;
		}
		wait(0);
//		shmctl(sh,IPC_RMID,NULL);		
//		semctl(sem,0,IPC_RMID);
		printf("%d\n",*p);
	}
	else
	{
		int i=0;
		while(i<10000000)
		{
			semop(sem,&pp,1);
			(*p)++;
			semop(sem,&v,1);
			i++;
		}
	}
}
