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

void send_fd(int fdw,int fd);
void recv_fd(int fdr,int* fd);
void makeprocess(int num,int* pairs);
int getpair_r(int* pairs,int num);
int getpair_w(int* pairs,int num);
void sendfile(int sock,int file_fd);
void recvfile(int);
void recvbuf(int sock,void* p,int len);
