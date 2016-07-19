#include <stdio.h>
#include <stdlib.h>
int main()
{
	char* a;
	printf("%d\n",(int)a);
	a=(char*)malloc(20);
	printf("%d\n",(int)a);
	char* b=a;
	free(a);
	free(b);
}
