#include "factory.h"
void* child_work(void* p)
{
	 pfactory pfy=(pfactory)p;
	int fd,se;
	while(1)
	{
//		printf("now I am crossing the mutex!\n");
		pthread_mutex_lock(&(pfy->cond_mutex));
//		printf("now I have crossed the mutex!\n");
		fd=queue_get(&(pfy->que));
//		printf("now the fd is %d!\n",fd);
		if(fd==-1)
		{
//			printf("now I am slepping!\n");
			pthread_cond_wait(&(pfy->cond),&(pfy->cond_mutex));
//			printf("awaken!\n");
		}
//		pthread_cond_wait(&(pfy->cond),&(pfy->cond_mutex));
		else{
			pthread_mutex_unlock(&(pfy->cond_mutex));
			se=sendfile(fd,FILENAME);
			if(se==-1){
				printf("mission failed!\n");
			}
			else{
				printf("mission completed!\n");
			}
			close(fd);
			pthread_mutex_lock(&(pfy->cond_mutex));
		}
		pthread_mutex_unlock(&(pfy->cond_mutex));
//		printf("here!\n");
	}
}
int sendfile(int sock,char* filename)
{
	int file_fd=open(filename,O_RDONLY);
	char buf[1000];
	int ret,check;
	seg send_seg;
	memset(&send_seg,0,sizeof(send_seg));
	send_seg.num=strlen(filename);
	memcpy(send_seg.cont,filename,strlen(filename));
	check=sendbuf(sock,&send_seg,4+strlen(filename));
	if(check==-1)
		return -1;
//	send(sock,&send_seg,4+strlen(filename),0);
	memset(&send_seg,0,sizeof(send_seg));
	while((ret=read(file_fd,buf,sizeof(buf)))>0)
	{
//		printf("%s\n",buf);
		memset(&send_seg,0,sizeof(send_seg));
		send_seg.num=ret;
		memcpy(send_seg.cont,buf,ret);
		check=sendbuf(sock,(void*)&send_seg,4+ret);
		if(check==-1)
		{
			return -1;
			close(file_fd);
		}
//		send(sock,&send_seg,4+ret,0);
	}
	int end=-1;
	send(sock,&end,4,0);
	close(file_fd);
	return 0;
}

int sendbuf(int sock,void* p,int len)
{
	int location=0;
	int ret=0;
	while((len-location))
	{
		ret=send(sock,p+location,len-location,0);
		if(ret==-1)
			return -1;
		location=location+ret;
	}
	return 0;
}
