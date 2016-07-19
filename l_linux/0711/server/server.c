#include "func.h"
int main()
{
	int num=5;
//	int file_fd=open("test",O_RDONLY);
//	printf("It is truely %d\n",file_fd);
	int* pairs=(int*)calloc(num*2,sizeof(int));
	int* busy=(int*)calloc(num,sizeof(int));
	makeprocess(num,pairs);
//	wait(0);
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
	struct epoll_event child_events[num];
	int i,j;
	int ep;
	int fd;
	for(i=0;i<num;i++)
	{
		child_events[i].events=EPOLLIN;
		child_events[i].data.fd=getpair_w(pairs,i);
		epoll_ctl(epfd,EPOLL_CTL_ADD,getpair_w(pairs,i),child_events+i);
	}
	struct epoll_event events[num+1];
//	printf("check1\n");
	while(1)
	{
		memset(events,0,sizeof(events));
		ep=epoll_wait(epfd,events,num+1,-1);
		int k;
//		for(k=0;k<10;k++)
//			printf("now the pairs is %d\n",pairs[k]);
		for(i=0;i<ep;i++)
		{
//			printf("check2\n");
			if(events[i].data.fd==sock)
			{
//				printf("and it is me!\n");
				fd=accept(sock,(struct sockaddr*)&client,&addrlen);
//				printf("check3\n");
				for(j=0;j<num;j++)
				{
					if(busy[j]==0)
					{//	printf("check4\n");
						printf("the %dth child is not busy now!\n",j);
						send_fd(getpair_w(pairs,j),fd);
						busy[j]=1;
						break;
					}
				}
			}
			for(j=0;j<num;j++)
			{	
				if(events[i].data.fd==getpair_w(pairs,j))
				{
					busy[j]=0;
					int work;
//					recv_fd(getpair_r(pairs,i),&work);
					recv(getpair_w(pairs,j),&work,4,0);
					printf("the %dth child has finished his work%d!\n",j,work);
				}
			}
		}
	}
}
