#include "factory.h"
int main()
{
	factory facty;
	factory_init(&facty,5,5);
//	printf("main init : the capcity is %d\n",(facty.que).capcity);
//	printf("main init : the size is %d\n",facty.size);
	factory_start(&facty,child_work);
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

	int epfd=epoll_create(1);
	struct epoll_event sock_event;
	sock_event.events=EPOLLIN;
	sock_event.data.fd=sock;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&sock_event);
	struct epoll_event events[1];
	int fd,ep;
	while(1)
	{
		memset(events,0,sizeof(events));
		ep=epoll_wait(epfd,events,1,-1);
		if(events[0].data.fd==sock)
		{
//			printf("I am waiting to recv a client!\n");
			fd=accept(sock,(struct sockaddr*)&client,&addrlen);
			queue_insertwork(&(facty.que),fd);
//			printf("come to work! the fd is %d\n",fd);
			pthread_cond_signal(&(facty.cond));
		}
	}
}
