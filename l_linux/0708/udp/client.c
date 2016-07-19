#include "func.h"
int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("wrong argc!\n");
		return -1;
	}
	int sock=socket(AF_INET,SOCK_DGRAM,0);
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
	int addrlen=sizeof(clisock);
	if(sendto(sock,"hello",5,0,(struct sockaddr*)&clisock,addrlen)==-1)
	{
		perror("send");
		return -1;
	}
	char buf[128]={0};
	if(recvfrom(sock,buf,sizeof(buf),0,NULL,NULL)==-1)
	{
		perror("recv");
		return -1;
	}
	printf("%s\n",buf);
	close(sock);
}
