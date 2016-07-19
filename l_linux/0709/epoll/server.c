#include "func.h"

int main()
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{
		perror("socket");
		return -1;
	}
	struct in_addr sin_addr;
	sin_addr.s_addr=inet_addr("192.168.1.95");
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(2000);
	addr.sin_addr=sin_addr;
	int b=bind(sock,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(b==-1)
	{
		perror("bind");
		return -1;
	}
	if(listen(sock,10)==-1)
	{
		perror("listen");
		return -1;
	}
	struct sockaddr_in client;
	memset(&client,0,sizeof(client));
	int addrlen=sizeof(struct sockaddr);
	printf("now I am waiting......\n");
	char buf[10]={0};

	int fd;
	struct epoll_event stdi_event;
	stdi_event.events=EPOLLIN|EPOLLET;
	stdi_event.data.fd=0;
	struct epoll_event sock_event;
	sock_event.events=EPOLLIN|EPOLLET;
	sock_event.data.fd=sock;
	int epfd=epoll_create(1);
	epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&sock_event);
	struct epoll_event fd_event;
	struct epoll_event events[3];
	int num;
	int recvnum;
	int i;
	while(1)
	{
		memset(events,0,sizeof(events));
		num=epoll_wait(epfd,events,3,-1);
		for(i=0;i<num;i++)
		{
			if(events[i].data.fd==sock)
			{
				fd=accept(sock,(struct sockaddr*)&client,&addrlen);
				int status=fcntl(fd,F_GETFL);
				status=status|O_NONBLOCK;
				fcntl(fd,F_SETFL,status);
				fd_event.events=EPOLLIN|EPOLLET;
				fd_event.data.fd=fd;
				epoll_ctl(epfd,EPOLL_CTL_DEL,sock,&sock_event);
				epoll_ctl(epfd,EPOLL_CTL_ADD,0,&stdi_event);
				epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&fd_event);
				printf("now I got it!\n");
				if(fd==-1)
				{
					perror("accept");
					return -1;
				}
				printf("the client is%s:%d!\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
				break;
			}
			if(events[i].data.fd==fd)
			{
				memset(buf,0,sizeof(buf));
				while((recvnum=recv(fd,buf,sizeof(buf)-1,0))>0)
				{
					printf("|||%s",buf);
					memset(buf,0,sizeof(buf));
				}
				if(recvnum==-1)
				{
					printf("!!!\n");
				}
				if(recvnum==0)
				{
					printf("now the connect is over!\n");
					epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&sock_event);
					epoll_ctl(epfd,EPOLL_CTL_DEL,0,&stdi_event);
					epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&fd_event);
					close(fd);
					break;
				}				
			}
			if(events[i].data.fd==0)
			{
				memset(buf,0,sizeof(buf));
				recvnum=read(0,buf,sizeof(buf));
				if(recvnum==0)
				{
					printf("now the connect is over!\n");
					epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&sock_event);
					epoll_ctl(epfd,EPOLL_CTL_DEL,0,&stdi_event);
					epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&fd_event);
					close(fd);
					break;
				}				
				send(fd,buf,strlen(buf)-1,0);
			}
		}
	}
}
