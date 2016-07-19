#include "func.h"
void send_fd(int fdw,int fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	char buf1[10]="hello ";
	char buf2[10]="world";
	struct iovec iov[2];
	iov[0].iov_base=buf1;
	iov[0].iov_len=6;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr* cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg)=fd;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	ret=sendmsg(fdw,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
}
//接收描述符控制信息	
void recv_fd(int fdr,int* fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	char buf1[10]={0};
	char buf2[10]={0};
	struct iovec iov[2];
	iov[0].iov_base=buf1;
	iov[0].iov_len=6;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr* cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
//	printf("卡不住\n");
	ret=recvmsg(fdr,&msg,0);
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}
	*fd=*(int*)CMSG_DATA(cmsg);
}

int sendfile(int sock,char* filename)
{
	int file_fd=open(filename,O_RDONLY);
	char buf[1000];
	int ret,check;
	seg send_seg;
	memset(&send_seg,0,sizeof(send_seg));
	send_seg.num=strlen(filename);
	memcpy(send_seg.cont,filename,strlen(filename));
	check=sendbuf(sock,&send_seg,4+strlen(filename));
	if(check==-1)
		return -1;
//	send(sock,&send_seg,4+strlen(filename),0);
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
