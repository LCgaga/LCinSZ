#include "func.h"
void recvfile(int sock)
{
	char buf[1000]={0};
	int size;
	recvbuf(sock,&size,sizeof(int));
	recvbuf(sock,buf,size);
	int fd=open(buf,O_RDWR|O_CREAT,0600);
	while(recvbuf(sock,&size,sizeof(int)),size!=-1)
	{
		memset(buf,0,sizeof(buf));
		recvbuf(sock,buf,size);
		write(fd,buf,size);
	}
	close(fd);
}

void recvbuf(int sock,void* p,int len)
{
	int location=0;
	int ret=0;
	while((len-location))
	{
		ret=recv(sock,p+location,len-location,0);
		location=location+ret;
	}
}
