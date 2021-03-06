#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <math.h>
void mode_show(int mode,char *a)
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

void time_show(char time_str[],const time_t *time)
{
	strcpy(time_str,ctime(time)+4);
	time_str[12]='\0';
}

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	DIR *dir;
	dir=opendir(argv[1]);
	if(NULL==dir)
	{
		perror("opendir");
		return -1;
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
		sprintf(s,"%s/%s",argv[1],p->d_name);
		ret=stat(s,&buf);
		if(ret==-1)
		{
			perror("stat");
			return -1;
		}
		if(p->d_name[0]!='.')
		{
			memset(time,0,sizeof(time));
			time_show(time,&buf.st_mtime);
			mode_show(buf.st_mode,mode);
			mode[1]='\0';
			printf("%s %5ld %s %s\n",mode,buf.st_size,time,p->d_name);
		}
	}
	return 0;
}

