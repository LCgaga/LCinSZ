#include <stdio.h>
#include <unistd.h>

int main()
{
	int fds[2];
	if(pipe(fds)!=0)
	{
		perror("pipe\n");
		return -1;
	}
	char a[20]={0};
	int pid=fork();
	if(pid>0)
	{
		close(fds[1]);
		read(fds[0],a,sizeof(a));
		printf("I am parent,I hear %s.\n",a);
		sleep(1);
		if(close(fds[0])==-1)
		{
			perror("close");
			return -1;
		}
		printf("front\n");
	}
	else if(pid==0)
	{
		close(fds[0]);
		write(fds[1],"Hello",5);
		printf("I am child\n");
		if(close(fds[1])==-1)
		{
			perror("close");
			return -1;
		}
	}
}
