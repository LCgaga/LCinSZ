#include "order.h"
void* child_work(void* p)
{
	pfactory pfy=(pfactory)p;
	int fd,se;
	char path[100];
	char home[100];
	while(1)
	{
		pthread_mutex_lock(&(pfy->cond_mutex));
		fd=queue_get(&(pfy->que));
		if(fd==-1)
		{
			pthread_cond_wait(&(pfy->cond),&(pfy->cond_mutex));
		}
		else
		{
			pthread_mutex_unlock(&(pfy->cond_mutex));
			order ord;
			char buf[200]={"passed!\n"};
			char name[50]={0};
			char log[200]={0};
			char now[50]={0};
			order_recv(fd,&ord);
			int au=authentify(ord);
			if(au==0)
			{
				sendret(fd,buf);
				memset(path,0,sizeof(path));
				memset(home,0,sizeof(home));
				memset(name,0,sizeof(name));
				memset(now,0,sizeof(now));
				strcpy(name,ord.oper);
				strcpy(path,"/home/lc/server/base/");
	
				gettime(now);
				sprintf(log,"%s %-6s %-8s\n",now,name,"login");
				write(pfy->log_fd,log,strlen(log));
	
				pthread_mutex_lock(&(pfy->path_mutex));
				chdir(path);
				if(chdir(ord.oper)!=0)
				{
					mkdir(ord.oper,0700);
					chdir(ord.oper);
				}
				getcwd(path,100);
				getcwd(home,100);
//				printf("home is %s path is %s\n",home,path);
				pthread_mutex_unlock(&(pfy->path_mutex));
				printf("a thread has work\n");
				while(1)//服务器处理
				{
					order_recv(fd,&ord);
					if(!strcmp(ord.oper,"exit"))
						break;
					pthread_mutex_lock(&(pfy->path_mutex));
					order_handle(ord,path,home,name,&(pfy->path_mutex),fd,pfy->log_fd);
				}
			}
			else
			{
				strcpy(buf,"user denied!\n");
				sendret(fd,buf);
			}
			pthread_mutex_lock(&(pfy->cond_mutex));
		}
		pthread_mutex_unlock(&(pfy->cond_mutex));
	}
}

void gettime(char* now)
{
//	char now[100]={0};
	time_t t=time(NULL);
	struct tm* tp=localtime(&t);
//	printf("size of now is%d\n",sizeof(now));
	sprintf(now,"%d %d %d %02d:%02d:%02d",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday+1,tp->tm_hour,tp->tm_min,tp->tm_sec);
//	return now;
}

int authentify(order ord)
{
//	printf("the name is %s,the passwd is %s\n",ord.oper,ord.para);
	struct spwd *sp;
	char salt[512]={0};
	sp=getspnam(ord.oper);
	if(sp==NULL)
	{
		return -1;
//		printf("no user\n");
//		break;
	}
	get_salt(salt,sp->sp_pwdp);
	if(strcmp(sp->sp_pwdp,crypt(ord.para,salt))==0)
		return 0;
	else
		return -1;
}

void get_salt(char* salt,char* passwd)
{
	int i,j;
	for(i=0,j=0;passwd[i]&&j!=3;i++)
	{
		if(passwd[i]=='$')
			j++;
	}
	strncpy(salt,passwd,i-1);
}
