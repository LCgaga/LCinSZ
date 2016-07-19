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
	int fd=accept(sock,(struct sockaddr*)&client,&addrlen);
	printf("now I got it!\n");
	if(fd==-1)
	{
		perror("accept");
		return -1;
	}
	printf("the client is%s:%d!\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	char buf[50]={0};
	if(recv(fd,buf,sizeof(buf),0)==-1)
	{
		perror("recv");
		return -1;
	}
	printf("I hear that:%s!\n",buf);
	if(send(fd,"hello",6,0)==-1)
	{
		perror("send");
		return -1;
	}
	close(sock);
	close(fd);
}
