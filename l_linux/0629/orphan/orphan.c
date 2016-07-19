#include <unistd.h>
#include <stdio.h>
int main()
{
	int pid=fork();
	if(pid==0)
	{
		while(1)
		printf("I am child process,my pid is %d,my ppid is %d\n",getpid(),getppid());
		return 0;
	}
	else if(pid>0){
		printf("I am parent process,my pid is %d,now I am exiting.\n",getpid());
		return 0;
	}
}
