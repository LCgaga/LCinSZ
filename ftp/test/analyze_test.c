#include<stdio.h>
#include<string.h>
void analyze(char* mesg)
{
	int i=0;
	int oper_start;
	int oper_len=0;
	int para_start;
	int para_len=0;
	char* p=mesg;
	while(*p==' ')
	{
		i++;
		p++;
	}
	oper_start=i;
	while(*p!=' ')
	{
		i++;
		p++;
		oper_len++;
	}
	while(*p==' ')
	{
		i++;
		p++;
	}
	para_start=i;
	while(*p!='\0'&&*p!=' ')
	{
		i++;
		p++;
		para_len++;
	}
//	para_len--;
	printf("%d\n",oper_start);
	printf("%d\n",oper_len);
	printf("%d\n",para_start);
	printf("%d\n",para_len);
//	memcpy(pord->oper,mesg+oper_start,oper_len);
//	memcpy(pord->para,mesg+para_start,para_len);
}
int main()
{
	while(1)
	{
		char a[50];
		memset(a,0,50);
		fgets(a,sizeof(a),stdin);
		a[strlen(a)-1]='\0';
		analyze(a);
	}
}
