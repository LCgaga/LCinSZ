#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main()
{
	int fd=open("file",O_RDWR|O_CREAT,0600);
	if(fd==-1)
	{
	perror("open failed!\n");
	return -1;
	}
	char a[10]="world";
	char *b=mmap(NULL,5,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if((int)b==-1)
	{
		perror("mmap failed!\n");
		return -1;
	}
	memcpy(b,a,5);
	if(munmap(b,5)==-1)
		perror("munmap failed!\n");
	return 0;
}
