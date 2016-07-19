#include "func.h"
void orderhandle(int sock,order ord)
{
	char buf[500]={0};
	if(!strcmp(ord.oper,"mkdir"))
	{
		sendbuf(sock,&ord,sizeof(ord));
		recvret(sock,buf);
		printf("%s\n",buf);
	}


	if(!strcmp(ord.oper,"exit"))
	{
		sendbuf(sock,&ord,sizeof(ord));
		printf("exiting in three seconds\n");
		sleep(3);
		exit(0);
	}
	if(!strcmp(ord.oper,"cd"))
	{
		sendbuf(sock,&ord,sizeof(ord));
		recvret(sock,buf);
		printf("%s\n",buf);
	}
	if(!strcmp(ord.oper,"ls"))
	{
		sendbuf(sock,&ord,sizeof(ord));
		recvret(sock,buf);
		printf("%s\n",buf);
	}
	if(!strcmp(ord.oper,"puts"))
	{
		sendbuf(sock,&ord,sizeof(ord));
		int file_fd=open(ord.para,O_RDONLY);
		if(file_fd==-1)
		{
			printf("failed to open file ,please check it the name is right.\n");
			return;
		}
//		printf("ready to send\n");
		sendfile(sock,file_fd);
//		printf("send is over\n");
		recvret(sock,buf);
		printf("%s\n",buf);
	}
	if(!strcmp(ord.oper,"gets"))
	{
		if(isfile(ord.para)==0)
		{
			printf("this file has already exited\n");
			return;
		}
		char tmpname[50]={0};
		sprintf(tmpname,"%s%s",ord.para,".tmp");
		if(isfile(tmpname)==0)
		{
			ord.pos=getfsize(tmpname);
			sendbuf(sock,&ord,sizeof(ord));
			conti_recvfile(sock,ord.para);
			printf("file has been continued to be recived\n");
			return;
			//获取order的第三个成员变量发过去，忽略对面已经没有此文件的情况
		}
		sendbuf(sock,&ord,sizeof(ord));
		recvret(sock,buf);
		if(strcmp(buf,"failed to open file ,please check it the name is right.\n"))
		{
			recvfile(sock,ord.para);
		}
		printf("%s\n",buf);
	
	}
	if(!strcmp(ord.oper,"remove"))
	{
		sendbuf(sock,&ord,sizeof(ord));
		recvret(sock,buf);
		printf("%s\n",buf);
	}
	if(!strcmp(ord.oper,"pwd"))
	{
		sendbuf(sock,&ord,sizeof(ord));
		recvret(sock,buf);
		printf("%s\n",buf);
	}

}

int getfsize(char* filename)
{
	struct stat statbuf;
	stat(filename,&statbuf);
	int size=statbuf.st_size;
	return size;
}

int isfile(char* filename)
{
	int ret=open(filename,O_RDONLY);
	if(ret<=0)
		return -1;
	close(ret);
	return 0;
}

void ordersend(int sock,char* mesg)
{
	order ord;
	memset(&ord,0,sizeof(ord));
	analyze(mesg,&ord);
	orderhandle(sock,ord);
}

void accountsend(int sock,char* mesg,char* ret)
{
	order ord;
	memset(&ord,0,sizeof(ord));
	analyze(mesg,&ord);
	sendbuf(sock,&ord,sizeof(ord));
	recvret(sock,ret);
}
