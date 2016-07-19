#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct student
{
	int num;
	char name[20];
	float score;
}stu,*pstu;
