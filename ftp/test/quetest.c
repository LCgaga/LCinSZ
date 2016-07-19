#include "factory.h"
int main()
{
	que queue;
	queue.head=NULL;
	queue.tail=NULL;
	queue.size=0;
	queue.capcity=10;
	queue_insertwork(&queue,1);
	queue_insertwork(&queue,2);
	queue_insertwork(&queue,3);
	int i=0;
	for(i=0;i<4;i++)
		printf("%d\n",queue_get(&queue));
}
