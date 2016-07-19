#include "func.h"

//循环发送
void send_n(int new_fd,char* buf,int len)
{
	int ret;
	int total=0;
	while(total<len)
	{
		ret=send(new_fd,buf+total,len-total,0);
		total=total+ret;	
	}
}

//循环接收
void recv_n(int new_fd,char* buf,int len)
{
	int ret;
	int total=0;
	while(total<len)
	{
		ret=recv(new_fd,buf+total,len-total,0);
		total=total+ret;
	}
}
