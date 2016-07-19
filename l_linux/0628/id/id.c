#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	printf("pid:%d\nppid:%d\nuid:%d\ngid:%d\neuid:%d\negid:%d\n",getpid(),getppid(),getuid(),getgid(),geteuid(),getegid());
	return 0;
}
