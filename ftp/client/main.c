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
	char a[50]={0};
	char ret[200]={0};
	
	printf("please enter your account and password on a line.\n");
	fgets(a,sizeof(a),stdin);
	memset(ret,0,sizeof(ret));
	a[strlen(a)-1]='\0';
	accountsend(sock,a,ret);
	if(strcmp(ret,"user denied!\n")!=0)
	{
		printf("%s\n",ret);
		while(1)
		{
			memset(a,0,sizeof(a));
			printf("[ftp ~/]$>");
			fgets(a,sizeof(a),stdin);
			a[strlen(a)-1]='\0';
			ordersend(sock,a);
		}
	}
	printf("%s\n",ret);
}
