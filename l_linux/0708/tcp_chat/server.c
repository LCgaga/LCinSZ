#include "func.h"

int main()
{
//	if(argc!=3)
//	{
//		printf("wrong argc!\n");
//		return -1;
//	}
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{
		perror("socket");
		return -1;
	}
	struct in_addr sin_addr;
	sin_addr.s_addr=inet_addr("192.168.1.95");
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(2000);
	addr.sin_addr=sin_addr;
	int b=bind(sock,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(b==-1)
	{
		perror("bind");
		return -1;
	}
	if(listen(sock,10)==-1)
	{
		perror("listen");
		return -1;
	}
	struct sockaddr_in client;
	memset(&client,0,sizeof(client));
	int addrlen=sizeof(struct sockaddr);
	printf("now I am waiting......\n");
//	int fd=accept(sock,(struct sockaddr*)&client,&addrlen);
//	printf("now I got it!\n");
//	if(fd==-1)
//	{
//		perror("accept");
//		return -1;
//	}
//	printf("the client is%s:%d!\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	char buf[50]={0};
	fd_set set;
	fd_set old;
	FD_ZERO(&old);
	FD_SET(sock,&old);
	int fd;
	int num;
	while(1)
	{
		
		memcpy(&set,&old,sizeof(old));
		if(select(14,&set,NULL,NULL,NULL)>0)
		{
			if(FD_ISSET(sock,&set))
			{
				fd=accept(sock,(struct sockaddr*)&client,&addrlen);
				FD_ZERO(&old);
				FD_SET(0,&old);
				FD_SET(fd,&old);
				printf("now I got it!\n");
				if(fd==-1)
				{
					perror("accept");
					return -1;
				}
				printf("the client is%s:%d!\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
				continue;
			}
			if(FD_ISSET(fd,&set))
			{
				memset(buf,0,sizeof(buf));
				num=recv(fd,buf,sizeof(buf),0);
				if(num==0)
				{
					printf("now the connect is over!\n");
					FD_ZERO(&old);
					FD_SET(sock,&old);
					close(fd);
					continue;
				}				
				printf("This time we recive %d chars It is: %s\n",num,buf);
//				if(FD_ISSET(fd,&set))
//				{
//					printf("fd is still setted!\n");
//				}
//				FD_CLR(fd,&set);
//				FD_SET(fd,&set);
//				if(FD_ISSET(fd,&set))
//				{
//					printf("fd is still setted!\n");
//				}
			}
			if(FD_ISSET(0,&set))
			{
				memset(buf,0,sizeof(buf));
				num=read(0,buf,sizeof(buf));
				if(num==0)
				{
					printf("now the connect is over!\n");
					FD_ZERO(&old);
					FD_SET(sock,&old);
					close(fd);
					continue;
				}				
				send(fd,buf,strlen(buf)-1,0);
//				FD_CLR(0,&set);
//				FD_SET(0,&set);
			}
		}
	}
}
