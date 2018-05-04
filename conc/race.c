/* 
 * race.c - demonstrates a race condition
 */
/* $begin race */
#include "csapp.h"
#define N 4

void *thread(void *vargp);

int main() 
{
    pthread_t tid[N];
    int i;

    for (i = 0; i < N; i++) 
    	Pthread_create(&tid[i], NULL, thread, &i); //line:conc:race:createthread 竞争出现在下一次在调用Pthread_create函数和第25行参数的简接引用和赋值之间，如果对等线程在主线程执行Pthread_create之前就执行了第25行，那么myid变量就得到正确的ID
    for (i = 0; i < N; i++) 
	    Pthread_join(tid[i], NULL);
    exit(0);
}

/* thread routine */
void *thread(void *vargp) 
{
    int myid = *((int *)vargp);  //line:conc:race:derefarg
    printf("Hello from thread %d\n", myid);
    return NULL;
}
/* $end race */
