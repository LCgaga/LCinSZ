#include "func.h"
int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("wrong argc!\n");
		return -1;
	}
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in clisock;
	memset(&clisock,0,sizeof(clisock));
	clisock.sin_family=AF_INET;
	clisock.sin_port=htons(atoi(argv[2]));
	clisock.sin_addr.s_addr=inet_addr(argv[1]);
	if(connect(sock,(struct sockaddr*)&clisock,sizeof(struct sockaddr))==-1)
	{
		perror("connect");
		return -1;
	}
	printf("the server is%s:%d!\n",inet_ntoa(clisock.sin_addr),ntohs(clisock.sin_port));
	char buf[50]={0};
	if(send(sock,"hello",6,0)==-1)
	{
		perror("send");
		return -1;
	}
	recv(sock,buf,sizeof(buf),0);
	printf("I hear that %s!\n",buf);
	close(sock);
	return -1;
}
