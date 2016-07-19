#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
//一个窗口用来设置票数，2个窗口用来卖票

pthread_mutex_t mutex;
pthread_cond_t cond;
int ticket=10;
void* setticket(void* p)
{
	pthread_mutex_lock(&mutex);
	if(ticket>0)
	{
		pthread_cond_wait(&cond,&mutex);
	}
	ticket=10;
	printf("set ticket success\n");
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
void* salewins(void* p)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(ticket>0)
		{
			printf("I am windows %d,the ticket is %d\n",(int)p,ticket);
			ticket--;
			if(ticket==0)
			{
				pthread_cond_signal(&cond);//唤醒设置票的线程去放票
			}
		}else{
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}
int main()
{
	pthread_t pset,psale1,psale2;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_create(&pset,NULL,setticket,NULL);
	pthread_create(&psale1,NULL,salewins,(void*)1);
	pthread_create(&psale2,NULL,salewins,(void*)2);
	pthread_join(pset,NULL);
	pthread_join(psale1,NULL);
	pthread_join(psale2,NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}
	
	
