#include <time.h>
#include <stdio.h>
void main()
{
	time_t t=time(NULL);
	struct tm* tp=localtime(&t);
	printf("%d %d %d %d:%d:%d\n",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday+1,tp->tm_hour,tp->tm_min,tp->tm_sec);
//	time(&timep);
//	printf("%s\n",asctime(&timep));
//	printf("%s\n",ctime(&timep));
}
