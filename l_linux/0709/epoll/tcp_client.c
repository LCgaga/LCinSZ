#include "func.h"
//通过socket实现tcp的客户端通信
int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);//将点分十进制转成32位网络字节序
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	fd_set rdset;
	char buf[128]={0};
	int i;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(sfd,&rdset);
		i=select(sfd+1,&rdset,NULL,NULL,NULL);
		if(i>0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=read(0,buf,sizeof(buf));					
				if(ret<=0)
				{
					break;
				}
				ret=send(sfd,buf,strlen(buf)-1,0);
				if(ret<=0)
				{
					perror("send");
					return -1;
				}
			}
			if(FD_ISSET(sfd,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);
				if(ret==0)
				{
					break;
				}
				else if(ret <0)
				{
					perror("recv");
					return -1;
				}
				printf("%s\n",buf);
			}
		}
	}
	close(sfd);
	return 0;
}

