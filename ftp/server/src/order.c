#include "order.h"
void order_recv(int sock,porder pord)
{
	recvbuf(sock,pord,sizeof(order)); 
}

void order_handle(order ord,char* path,char* home,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)//1、在这里要传进来一把锁，并且要锁上后将锁传到下面的函数中 2、要传入当前的路径值
{
//	printf("we recive the oper to be %s and para %s\n",ord.oper,ord.para);
	if(!strcmp(ord.oper,"cd"))
		cd_handle(ord.para,path,home,name,pmutex,fd,log_fd);
	if(!strcmp(ord.oper,"ls"))
		ls_handle(ord.para,path,name,pmutex,fd,log_fd);
	if(!strcmp(ord.oper,"mkdir"))
		mkdir_handle(ord.para,path,name,pmutex,fd,log_fd);
	if(!strcmp(ord.oper,"puts"))
		puts_handle(ord.para,path,name,pmutex,fd,log_fd);
	if(!strcmp(ord.oper,"gets"))
		gets_handle(ord,path,name,pmutex,fd,log_fd);
	if(!strcmp(ord.oper,"remove"))
		remv_handle(ord.para,path,name,pmutex,fd,log_fd);
	if(!strcmp(ord.oper,"pwd"))
		pwd_handle(path,name,pmutex,fd,log_fd);
}

void mkdir_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)
{
//	printf("the former path is %s\n",path);
	char log[200]={0};
	char now[50]={0};
	char buf[50]={"dictory created.\n"};
	chdir(path);
	memset(path,0,50);
	getcwd(path,50);
	if(mkdir(para,0777)!=0)
	{
		memset(buf,0,sizeof(buf));
		strcpy(buf,"failed to create dictory.\n");
	}
//	printf("%s\n",path);
//	printf("check_pwd\n");
	pthread_mutex_unlock(pmutex);
	gettime(now);
	sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"mkdir",para);
	write(log_fd,log,strlen(log));
	sendret(fd,buf);
}
void cd_handle(char* para,char* path,char* home,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)//在这里传入保存的路径：1、跳到指定路径值，拼接路径，解锁
{
	char log[200]={0};
	char buf[100]={0};
	char now[50]={0};
	strcpy(buf,path);
	chdir(path);
	chdir(para);
	memset(path,0,100);
	getcwd(path,100);
	if(strncmp(path,home,strlen(home))!=0)
		chdir(buf);
	memset(path,0,100);
	getcwd(path,100);
//	printf("%s\n",path);
	pthread_mutex_unlock(pmutex);
	gettime(now);
	sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"cd",path);
	write(log_fd,log,strlen(log));
	sendret(fd,path);
}

void ls_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)
{
	char buff[500]={0};
	char tmp[100]={0};
	char log[200]={0};
	char now[50]={0};
	chdir(path);
	DIR *dir;
	dir=opendir(path);
	if(NULL==dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	struct stat buf;
	char s[100];
	char time[100];
	char u_t[100];
	char g_t[100];
	char mode[11];
	int ret;
	while((p=readdir(dir))!=NULL)
	{
		memset(s,0,sizeof(s));
		memset(&buf,0,sizeof(buf));
		memset(mode,0,sizeof(mode));
		sprintf(s,"%s/%s",path,p->d_name);
		ret=stat(s,&buf);
		if(ret==-1)
		{
			perror("stat");
			return ;
		}
		if(p->d_name[0]!='.')
		{
			memset(time,0,sizeof(time));
			time_show(time,&buf.st_mtime);
			mode_show(buf.st_mode,mode);
			mode[1]='\0';
			sprintf(tmp,"%s %12ld %s %s\n",mode,buf.st_size,time,p->d_name);
			strcat(buff,tmp);
			memset(tmp,0,sizeof(tmp));
		}
	}
//	printf("check_ls\n");
	pthread_mutex_unlock(pmutex);
	gettime(now);
	sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"ls",path);
	write(log_fd,log,strlen(log));
	sendret(fd,buff);
}

void puts_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)//保护到open文件完成得到描述符，然后解锁，然后开始传送，这样即使被打断也没关系
{
	chdir(path);
	char log[200]={0};
	char now[50]={0};
	char buf[50]="translation completed\n";
	int file_fd=open(para,O_RDWR|O_CREAT,0600);
	if(file_fd==-1)
	{
		printf("fail to open\n");
	}
	pthread_mutex_unlock(pmutex);
	gettime(now);
	sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"puts",para);
	write(log_fd,log,strlen(log));
	recvfile(fd,file_fd);
	sendret(fd,buf);
}

void gets_handle(order ord,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)
{
	chdir(path);
	char log[200]={0};
	char now[50]={0};
	char buf[200]="translation completed\n";
	printf("check_gets\n");
	int size;
	int flie_fd=open(ord.para,O_RDONLY);
	if(flie_fd==-1)//文件不存在
	{
		memset(buf,0,sizeof(buf));
		pthread_mutex_unlock(pmutex);
		strcpy(buf,"failed to open file ,please check it the name is right.\n");
		sendret(fd,buf);
		gettime(now);
		sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"gets",ord.para);
		write(log_fd,log,strlen(log));
		return;
	}
	if(ord.pos!=0)//断点续传
	{
		printf("pos is %d\n",ord.pos);
		pthread_mutex_unlock(pmutex);
		lseek(flie_fd,ord.pos,SEEK_SET);
		sendfile(fd,flie_fd);
		pthread_mutex_unlock(pmutex);
		gettime(now);
		sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"gets",ord.para);
		write(log_fd,log,strlen(log));
		return;
	}
	if((size=getfsize(ord.para))>100*1024*1024)//大文件发送
	{
		printf("what a big one!\n");
		pthread_mutex_unlock(pmutex);
		sendret(fd,buf);
		char* pos=mmap(NULL,size,PROT_READ,MAP_SHARED,flie_fd,0);
		sendfile_map(fd,pos,size);
		munmap(pos,size);
		gettime(now);
		sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"gets",ord.para);
		write(log_fd,log,strlen(log));
		return ;
	}
	//下面为普通发送
	pthread_mutex_unlock(pmutex);
	gettime(now);
	sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"gets",ord.para);
	write(log_fd,log,strlen(log));
	sendret(fd,buf);
	if(flie_fd!=-1)
		sendfile(fd,flie_fd);
}

void remv_handle(char* para,char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)
{
	chdir(path);
	int ret=remove(para);
	if(ret!=0)
	{
		ret=rmdir(para);
	}
	char buf[200]={0};
	char log[200]={0};
	char now[50]={0};
	if(ret==0)
		strcpy(buf,"delete successfully\n");
	else
		strcpy(buf,"delete failed, please check the name again.\n");
//	printf("check_remv\n");
	pthread_mutex_unlock(pmutex);
	gettime(now);
	sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"remv",para);
	write(log_fd,log,strlen(log));
	sendret(fd,buf);
}

void pwd_handle(char* path,char* name,pthread_mutex_t* pmutex,int fd,int log_fd)
{
//	printf("the former path is %s\n",path);
	char log[200]={0};
	char now[50]={0};
	chdir(path);
	memset(path,0,50);
	getcwd(path,50);
	printf("%s\n",path);
//	printf("check_pwd\n");
	pthread_mutex_unlock(pmutex);
	gettime(now);
	sprintf(log,"%s %-6s %-8s %-30s\n",now,name,"pwd",path);
	write(log_fd,log,strlen(log));
	sendret(fd,path);
}

int getfsize(char* filename)
{
	struct stat statbuf;
	stat(filename,&statbuf);
	int size=statbuf.st_size;
	return size;
}

void mode_show(int mode,char *a)//ls相关
{
	char b[]="-rwxrwxrwx";
 	int c[11]={0};
	int d[11]={0};
	int i,j,k,tmp;
	k=mode/pow(2,14);
	for(i=1;i<11;i++)
	{
		d[i]=b[i]-'a';
	}
	for(i=0;i<11;i++)
	{
		c[i]=mode%2;
		mode=mode/2;
	}
	for(i=0,j=9;i<=j;i++,j--)
	{
		tmp=c[i];
		c[i]=c[j];
		c[j]=tmp;
	}
	for(i=0;i<11;i++)
	{
		c[i]=c[i]*d[i];
	}
	for(i=0;i<11;i++)
	{
		a[i]=c[i]+'a';
	}
	a[i-1]='\0';
	for(i=0;a[i]!='\0';i++)
	{
		if(a[i]=='a')
		{
			a[i]='-';
		}
	}
	if(k==1)
	{
		a[0]='d';
	}
}

void time_show(char time_str[],const time_t *time)//ls相关
{
	strcpy(time_str,ctime(time)+4);
	time_str[12]='\0';
}
