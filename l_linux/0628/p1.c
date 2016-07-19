#include "p.h"


int main()
{
	int rw=open("pipe1",O_WRONLY);
	if(rw==-1)
	{
		perror("pipe1 open failed\n");
		return -1;
	}
	int rr=open("pipe2",O_RDONLY);
	if(rr==-1)
     {
         perror("pipe2 open failed\n");
         return -1;
     }
	char say[50]={0};
	char hear[50]={0};
	fd_set set;
	while(1)
	{
		FD_ZERO(&set);
		FD_SET(0,&set);
		FD_SET(rr,&set);
		if(select(rr+1,&set,NULL,NULL,NULL)>0)
		{
			if(FD_ISSET(0,&set))
			{
				read(0,say,sizeof(say)-1);
				write(rw,say,strlen(say)-1);
				memset(say,0,sizeof(say));
			}else if(FD_ISSET(rr,&set))
			{
				read(rr,hear,sizeof(hear));
				printf("%s\n",hear);
				memset(hear,0,sizeof(hear));
			}
		}
	}



	return 0;
}
