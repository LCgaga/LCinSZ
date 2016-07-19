#include <signal.h>
#include <stdio.h>
void func(int sig)
{
	printf("the %dth signal has come.\n",sig);
	sleep(3);
	printf("waked from %d\n",sig);
}
int main()
{
	signal(SIGINT,func);
	signal(SIGQUIT,func);
	while(1);
}
