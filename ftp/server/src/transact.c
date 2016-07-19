#include "order.h"
void sendret(int sock,char* pbuf)
{
	seg send_seg;
	memset(&send_seg,0,sizeof(send_seg));
	send_seg.num=strlen(pbuf);
	memcpy(send_seg.cont,pbuf,strlen(pbuf));
	sendbuf(sock,&send_seg,4+strlen(pbuf));
}

void recvfile(int sock,int fd)
{
	char buf[1000]={0};
	int size;
//	int fd=open(filename,O_RDWR|O_CREAT,0600);
	while(recvbuf(sock,&size,sizeof(int)),size!=-1)
	{
		memset(buf,0,sizeof(buf));
		recvbuf(sock,buf,size);
		write(fd,buf,size);
	}
	close(fd);
}

int sendfile_map(int sock,char* pos,int len)
{
	int resi=len%1000;
	int times=len/1000;
	int i;
	int check;
	seg send_seg;
	for(i=0;i<times;i++)
	{
		memset(&send_seg,0,sizeof(send_seg));
		send_seg.num=1000;
		memcpy(send_seg.cont,pos,1000);
		check=sendbuf(sock,(void*)&send_seg,4+1000);
		if(check==-1)
		{
			printf("nothing serious.\n");
			return -1;
		}
		pos=pos+1000;
	}
	memset(&send_seg,0,sizeof(send_seg));
	send_seg.num=resi;
	memcpy(send_seg.cont,pos,resi);
	check=sendbuf(sock,(void*)&send_seg,4+resi);
	if(check==-1)
	{
		printf("nothing serious.\n");
		return -1;
	}
	int end=-1;
	send(sock,&end,4,0);
	printf("all things right.\n");
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
			printf("nothing serious.\n");
			close(file_fd);
			return -1;
		}
//		send(sock,&send_seg,4+ret,0);
	}
	int end=-1;
	send(sock,&end,4,0);
	printf("all things right.\n");
	close(file_fd);
	return 0;
}

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

