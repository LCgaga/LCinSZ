#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	int pid =fork();
	if(pid==0)
	{
		printf("I am child process %d.\n",getpid());
		sleep(5);
		printf("child return\n");
		return 0;
	}
	else if(pid>0)
	{
		printf("I am parent process %d.\n",getpid());
		int r;
		r=wait(NULL);
		printf("now my child %d has returned.\n",r);
		
	}
}
