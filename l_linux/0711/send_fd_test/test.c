#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
//发送描述符控制信息
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
	ret=recvmsg(fdr,&msg,0);
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}
	*fd=*(int*)CMSG_DATA(cmsg);
}



int main()
{
	int fds[2];
	int ret;
	ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
	if(-1==ret)
	{
		perror("socketpair");
		return -1;
	}
	if(!fork())
	{
//		close(fds[1]);
		int fd =open("test",O_RDWR|O_CREAT,0600);
		send_fd(fds[0],fd);
//		printf("I am child,fds[0]=%d\n",fds[0]);
		//read(fds[0],&fd,4);
		recv_fd(fds[1],&fd);
		printf("I am child,fd1=%d\n",fd);
		send_fd(fds[1],fd);
		recv_fd(fds[1],&fd);
		printf("I am child,fd2=%d\n",fd);
		char buf[128]={0};
//		read(fd,buf,sizeof(buf));
//		printf("buf=%s\n",buf);
		exit(0);
	}else{
//		close(fds[0]);
//		int fd=open("test",O_RDONLY);
//		printf("fd=%d\n",fd);
		//write(fds[1],&fd,sizeof(int));
//		send_fd(fds[1],fd);
		wait(NULL);
		return 0;
	}
}	

