#include "func.h"
void sig_handle(int sig)
{
	printf("sig comes\n");
}
void makeprocess(int num,int* pairs)
{
	int i,fd;
	int ret;

	for(i=0;i<num;i++)
	{//父进程
		ret=socketpair(AF_LOCAL,SOCK_STREAM,0,pairs+2*i);
		if(-1==ret)
		{
			perror("socketpair");
			return ;
		}
	}
	for(i=0;i<num;i++)
	{
		if(fork())
		{//父进程
			
		}else
		{
//			signal(SIGPIPE,sig_handle);
			while(1)
			{
//				printf("I am the %dth child\n",i);
				recv_fd(getpair_r(pairs,i),&fd);
//				printf("now I get the %dmission!\n",fd);
				int se=sendfile(fd,FILENAME);
				if(se==-1)
					printf("mission %d failed!\n",fd);
//				send_fd(getpair_r(pairs,i),fd);
				send(getpair_r(pairs,i),&fd,4,0);
//				exit(0);
			}
		}
	}
}

int getpair_r(int* pairs,int num)
{
	return *(pairs+num*2);
}


int getpair_w(int* pairs,int num)
{
	return *(pairs+num*2+1);
}





