#include <unistd.h>
#include <stdio.h>
int main()
{
	int pid =fork();
	if(pid==0)
	{
		printf("I am child process %d, now I am becoming zombie\n",getpid());
		return 0;
	}
	else if(pid>0)
	{
		printf("I am parent process %d,now I am sleeping\n",getpid());
		sleep(10);
		return 0;
	}
}
