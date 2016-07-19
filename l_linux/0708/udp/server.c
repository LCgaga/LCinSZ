#include "func.h"
int main()
{
	int sfd=socket(AF_INET,SOCK_DGRAM,0);
	struct in_addr sin_addr;
	sin_addr.s_addr=inet_addr("192.168.1.95");
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(2000);
	addr.sin_addr=sin_addr;
	int addrlen=sizeof(addr.sin_addr);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1)
	{
		perror("bind");
		return -1;
	}
	struct sockaddr_in cli_addr;
	char buf[128]={0};
	if(recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&addr,&addrlen)==-1)
	{
		perror("recv");
		return -1;
	}
	printf("%s\n",buf);
	if(sendto(sfd,"hello",5,0,(struct sockaddr*)&addr,addrlen)==-1)
	{
		perror("send");
		return -1;
	}
	close(sfd);
}
