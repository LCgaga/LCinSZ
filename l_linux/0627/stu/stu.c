#include "stuh.h"

int main()
{
	stu s[3]={{1,"zhangsan",90.5},{2,"lisi",80.5},{3,"wangwu",70.5}};
//	printf("name is%s\n%s\n%s\n",s[0].name,s[1].name,s[2].name);
	int fd=open("file",O_RDWR|O_CREAT|O_TRUNC,0600);
	if(fd==-1)
	{
		perror("open failed!\n");
		return -1;
	}
	stu n;
	int wr=write(fd,s,sizeof(s));
	if(wr==-1)
	{
		perror("write failed!\n");
		return -1;
	}
	int wl=lseek(fd,0,SEEK_SET);
	if(wl==-1)
	{
		perror("seek failed!\n");
		return -1;
	}
	int rr;
	int i;
	for(i=0;i<3;i++)
	{
		rr=read(fd,&n,sizeof(n));
		if(rr==-1)
		{
			perror("read failed!\n");
			return -1;
		}
		printf("num:%d,name:%s,score:%f\n",n.num,n.name,n.score);
	}
	return 0;
}
