#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <crypt.h>
#include <errno.h>
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
int main()
{
	struct spwd *sp;
	char name[50]={0};
	char passwd[50]={0};
	char salt[512]={0};
	while(1)
	{
		memset(name,0,sizeof(name));
		memset(passwd,0,sizeof(passwd));
		scanf("%s",name);
		scanf("%s",passwd);
		sp=getspnam(name);
		if(sp==NULL)
		{
			printf("no user\n");
			break;
		}
		get_salt(salt,sp->sp_pwdp);
		if(strcmp(sp->sp_pwdp,crypt(passwd,salt))==0)
			printf("passed!\n");
		else
			printf("denied!\n");
	}
}
