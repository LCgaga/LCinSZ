#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void func(int signum)
{
	printf("the %dth signal has came\n",signum);
	sleep(3);
	printf("returned from %dth signal\n",signum);
}

int main()
{
	struct sigaction sig;
	memset(&sig,0,sizeof(sig));
	sig.sa_handler=func;
	sig.sa_flags=SA_RESTART;
	sigaction(2,&sig,NULL);
	sigaction(3,&sig,NULL);
	char a[50]={0};
	read(0,a,sizeof(a));
//	while(1);
	printf("%s\n",a);
}
