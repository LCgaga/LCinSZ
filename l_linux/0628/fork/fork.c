#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
	int i=fork();
	while(1)
	{
		printf("my pid is %d\n",getpid());
	}
}
