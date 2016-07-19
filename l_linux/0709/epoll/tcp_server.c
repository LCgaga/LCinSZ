/*************************************************************************
 Author:MASnail
 Created Time: 2016年07月08日 星期五 13时55分42秒
 File Name: serve.c
 Description: 
 ************************************************************************/

#include "func.h"
#define MAX 32
void set_nonb(int fd)
{
	int set_fd=fcntl(fd,F_GETFL);
	set_fd=set_fd|O_NONBLOCK;
	int ret=fcntl(fd,F_SETFL,set_fd);
	if(-1 == ret)
	{
		perror("fcntl");
		return;
	}
}

int main()
{
	int ret;
	int sock_serve;
	sock_serve = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sock_serve)
	{
		perror("socket");
		return -1;
	}

	int cancel_port=0;//cancel port res
	setsockopt(sock_serve,SOL_SOCKET,SO_REUSEADDR,(const void*)&cancel_port,sizeof(int));
	struct sockaddr_in ip_serve;
	memset(&ip_serve,0,sizeof(ip_serve));
	ip_serve.sin_family=AF_INET;
	ip_serve.sin_port=htons(2000);
	ip_serve.sin_addr.s_addr=inet_addr("192.168.1.95");//INADDR_ANY;
	ret = bind(sock_serve,(struct sockaddr*)&ip_serve,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		close(sock_serve);
		return -1;
	}
	
	ret = listen(sock_serve,10);
	if(-1 == ret)
	{
		perror("listen");
		close(sock_serve);
		return -1;
	}

	//seting more thread array
	int ret_serve;
	int sock_client;//client sock
	char buff[MAX];
	int epfd;
	epfd = epoll_create(1);
	struct epoll_event evt;
	struct epoll_event *evts=(struct epoll_event*)malloc(3*sizeof(struct epoll_event));
	memset(&evt,0,sizeof(struct epoll_event));
	memset(evts,0,sizeof(struct epoll_event)*3);
	evt.events=EPOLLIN;
	evt.data.fd=sock_serve;
	ret_serve=epoll_ctl(epfd,EPOLL_CTL_ADD,sock_serve,&evt);
	if(-1 == ret_serve)
	{
		perror("epoll_ctl");
		return -1;
	}
	int index=0;
	while(1)
	{
ret:		ret_serve = epoll_wait(epfd,evts,3,-1);
		for(index=0;index<ret_serve;index++)
		{
			//client request
			if(evts[index].data.fd==sock_serve&&evts[index].events==EPOLLIN)
			{
				sock_client=accept(sock_serve,NULL,NULL);
				if(-1 == sock_client)
				{
					perror("accept");
					break;
				}
			evt.events=EPOLLIN;
			evt.data.fd=0;
			ret_serve=epoll_ctl(epfd,EPOLL_CTL_ADD,0,&evt);
			set_nonb(sock_client);
			evt.events=EPOLLIN|EPOLLET;
			evt.data.fd=sock_client;
			ret_serve=epoll_ctl(epfd,EPOLL_CTL_ADD,sock_client,&evt);
			}
			
			//recv
			if(evts[index].data.fd==sock_client&&evts[index].events==EPOLLIN)
			{
				while(1)
				{
				memset(&buff,0,sizeof(buff));
				ret_serve = recv(sock_client,buff,sizeof(buff)-1,0);
				if(0 < ret_serve)
				{
					printf("%s",buff);
				}
				/* client end */ 
				else if(0 == ret_serve){
				evt.events=EPOLLIN;
				evt.data.fd=0;
				ret_serve=epoll_ctl(epfd,EPOLL_CTL_DEL,0,&evt);
				evt.events=EPOLLIN;
				evt.data.fd=sock_client;
				ret_serve=epoll_ctl(epfd,EPOLL_CTL_DEL,sock_client,&evt);
				close(sock_client);
				goto ret;
					}else{
						break;
					}
				}
				printf("\n");
			}
			//send
			if(evts[index].data.fd==0&&evts[index].events==EPOLLIN)
			{
				memset(&buff,0,sizeof(buff));
				read(0,buff,sizeof(buff));
				ret_serve = send(sock_client,buff,strlen(buff)-1,0);
				
				/* client end */
				if(0 >= ret_serve)
				{
				evt.events=EPOLLIN;
				evt.data.fd=0;
				ret_serve=epoll_ctl(epfd,EPOLL_CTL_DEL,0,&evt);
				evt.events=EPOLLIN;
				evt.data.fd=sock_client;
				ret_serve=epoll_ctl(epfd,EPOLL_CTL_DEL,sock_client,&evt);
					close(sock_client);
					break;
				}
			}

		}//if
	}//while
close(sock_serve);
close(epfd);

	return 0;
}

