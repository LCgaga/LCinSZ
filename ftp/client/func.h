#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
	int num;
	char cont[1000];
}seg,*pseg;
typedef struct order_t{
	char oper[10];
	char para[30];
	int pos;
//	int paranum;
}order,*porder;
int sendfile(int sock,int file_fd);
void conti_recvfile(int sock,char*filename);
int getfsize(char* filename);
int isfile(char* filename);
void recvfile(int,char*);
void recvret(int sock,char* pbuf);
void recvbuf(int sock,void* p,int len);
void ordersend(int sock,char* mesg);
void analyze(char* mesg,porder pord);
int sendbuf(int sock,void* p,int len);
void orderhandle(int sock,order ord);
void accountsend(int sock,char* mesg,char* ret);
