#include "func.h"
int sendbuf(int sock,void* p,int len)
{
	int location=0;
	int ret=0;
	while((len-location))
	{
		ret=send(sock,p+location,len-location,0);
		if(ret==-1)
			return -1;
		location=location+ret;
	}
	return 0;
}
int sendfile(int sock,int file_fd)
{
	char buf[1000]={0};
	int ret,check;
	seg send_seg;
	memset(&send_seg,0,sizeof(send_seg));
	while((ret=read(file_fd,buf,sizeof(buf)))>0)
	{
//		printf("%s\n",buf);
		memset(&send_seg,0,sizeof(send_seg));
		send_seg.num=ret;
		memcpy(send_seg.cont,buf,ret);
		check=sendbuf(sock,(void*)&send_seg,4+ret);
		if(check==-1)
		{
			return -1;
			close(file_fd);
		}
//		send(sock,&send_seg,4+ret,0);
	}
	int end=-1;
	send(sock,&end,4,0);
	close(file_fd);
	return 0;
}

void conti_recvfile(int sock,char*filename)
{
	char buf[1000]={0};
	char tmpname[50]={0};
	sprintf(tmpname,"%s%s",filename,".tmp");
	int size;
	int fd=open(tmpname,O_RDWR|O_APPEND,0600);
	while(recvbuf(sock,&size,sizeof(int)),size!=-1)
	{
		memset(buf,0,sizeof(buf));
		recvbuf(sock,buf,size);
		write(fd,buf,size);
	}
	close(fd);
	rename(tmpname,filename);
}

void recvfile(int sock,char*filename)
{
	char buf[1000]={0};
	char tmpname[50]={0};
	sprintf(tmpname,"%s%s",filename,".tmp");
	int size;
	int fd=open(tmpname,O_RDWR|O_CREAT,0600);
	while(recvbuf(sock,&size,sizeof(int)),size!=-1)
	{
		memset(buf,0,sizeof(buf));
		recvbuf(sock,buf,size);
		write(fd,buf,size);
	}
	close(fd);
	rename(tmpname,filename);
}

void recvret(int sock,char* pbuf)
{
	int size;
//	printf("size is %d",size);
	recvbuf(sock,&size,sizeof(int));
	recvbuf(sock,pbuf,size);
}
void recvbuf(int sock,void* p,int len)
{
	int location=0;
	int ret=0;
	while((len-location))
	{
		ret=recv(sock,p+location,len-location,0);
		location=location+ret;
	}
}

void analyze(char* mesg,porder pord)
{
	int i=0;
	int oper_start;
	int oper_len=0;
	int para_start;
	int para_len=0;
	char* p=mesg;
	while(*p==' ')
	{
		i++;
		p++;
	}
	oper_start=i;
	while(*p!=' '&&*p!='\0')
	{
		i++;
		p++;
		oper_len++;
	}
	while(*p==' ')
	{
		i++;
		p++;
	}
	para_start=i;
	while(*p!=' '&&*p!='\0')
	{
		i++;
		p++;
		para_len++;
	}
//	printf("%d %d %d %d\n",oper_start,oper_len,para_start,para_len);
	memcpy(pord->oper,mesg+oper_start,oper_len);
	if(para_len!=0)
		memcpy(pord->para,mesg+para_start,para_len);
}
