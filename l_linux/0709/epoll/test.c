#include "func.h"
#include <errno.h>
int main()
{
//	int status=fcntl(0,F_GETFL);
//	status=status|O_NONBLOCK;
//	fcntl(0,F_SETFL,status);
//	char buf[50]={0};
//	sleep(2);
//	int rec=read(0,buf,50);
//	printf("%d\n",rec);
//	if(rec==EAGAIN)
//		printf("yes!\n");
	int i;
	for(i=0;i<10;i++)
	printf("|0123456789");
	printf("\n");
}
