#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
int main()
{
	key_t key=ftok("a.c",1);
	if(key==-1)
	{
		perror("ftok\n");
		return -1;
	}
	int shmid=shmget(key,4096,IPC_CREAT|0600);
	if(shmid==-1)
	{
		perror("shmget");
		return -1;
	}
	char* ch=(char*)shmat(shmid,NULL,0);
	if((int)ch==-1)
	{
		perror("ch");
		return -1;
	}
	strcpy(ch,"How are you");
	while(1);
	shmdt(ch);
	return 0;
}
