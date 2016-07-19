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
	
	fd_set set;
	char buf[50]={0};
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(0,&set);
        FD_SET(sock,&set);
        if(select(sock+1,&set,NULL,NULL,NULL)>0)
        {
            if(FD_ISSET(0,&set))
                {
                    memset(buf,0,sizeof(buf));
                    int num1=read(0,buf,sizeof(buf));
					if(num1==0)
						break;
                    send(sock,buf,strlen(buf)-1,0);
                }
            if(FD_ISSET(sock,&set))
                {
					printf("begin to recv!\n");
					recvfile(sock);
					break;
//					int size=0;
//					recv(sock,&size,sizeof(int),0);
//					printf("the 1th size is %d\n",size);
//              	memset(buf,0,sizeof(buf));
//					recv(sock,buf,size,0);
//                  int num=recv(sock,buf,sizeof(buf),0);
//					if(num==0)
//						break;
//                    printf("%s\n",buf);
//					recv(sock,&size,sizeof(int),0);
//					printf("the 2th size is %d\n",size);
//              	memset(buf,0,sizeof(buf));
//					recv(sock,buf,size,0);
//                  int num=recv(sock,buf,sizeof(buf),0);
//					if(num==0)
//						break;
//                  printf("%s\n",buf);
//					recv(sock,&size,sizeof(int),0);
//					printf("the last num is %d\n",size);
                }
        }
    }
	close(sock);
	return -1;
}
