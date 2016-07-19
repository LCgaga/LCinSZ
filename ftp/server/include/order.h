#ifndef __ORDER_H__
#define __ORDER_H__
#include "factory.h"
typedef struct order_t{
	char oper[10];
	char para[30];
	int pos;
}order,*porder;

void order_handle(order ord,char* path,char* home,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void order_send(order ord);
void order_recv(int sock,porder pord);
void mkdir_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void cd_handle(char* para,char* path,char* home,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void ls_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void puts_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void gets_handle(order ord,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void remv_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void pwd_handle(char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd);
void mode_show(int mode,char *a);//ls相关
void time_show(char time_str[],const time_t *time);//ls相关
void gettime(char* now);
int authentify(order ord);
void get_salt(char* salt,char* passwd);
int getfsize(char* filename);
#endif
