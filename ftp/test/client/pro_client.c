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
	int len;
	char buf[1000];
	//接收文件名
	ret=recv(sfd,&len,sizeof(len),0);
	if(ret<0)
	{
		perror("recv");
		return -1;
	}
	memset(buf,0,sizeof(buf));
	recv(sfd,buf,len,0);
	int fd;
	fd=open(buf,O_RDWR|O_CREAT,0666);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	//接收文件内容
	while(1)
	{
		recv_n(sfd,&len,sizeof(len));//先接收火车头，长度
		memset(buf,0,sizeof(buf));
		if(len >0)
		{
			recv_n(sfd,buf,len);//再接内容
			write(fd,buf,len);
		}else{
			break;
		}
	}
	close(fd);
	close(sfd);
	return 0;
}
		
		
		
