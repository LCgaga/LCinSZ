#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	int pid=fork();
	if(pid>0)
	{
		return 0;
	}
	else
	{
		int sid=setsid();
		if(sid==-1)
		{
			perror("sid");
			return -1;
		}
		chdir("/");
		umask(0);
		int i;
		for(i=0;i<1024;i++)
		{
			close(i);
		}
		int fd=open("/tmp/log",O_RDWR|O_CREAT,0666);
		if(fd==-1)
		{
			perror("open");
			return -1;
		}
		while(1)
		{
			int wr=write(fd,"I have run 5s.\n",15);
//			printf("I am writing\n");
			if(wr==-1)
			{
				perror("write");
				return -1;
			}
			sleep(5);
		}
	}
}
