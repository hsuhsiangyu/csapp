/* 
 * hello.c - Pthreads "hello, world" program 
 */
/* $begin hello */
#include "csapp.h"
void *thread(void *vargp);                    //line:conc:hello:prototype

int main()                                    //line:conc:hello:main
{
    pthread_t tid;                            //line:conc:hello:tid 用来存放对等线程的线程ID
    Pthread_create(&tid, NULL, thread, NULL); //line:conc:hello:create 返回时，主线程和新创建的对等线程同时运行，tid包含新创建线程的ID
    Pthread_join(tid, NULL);                  //line:conc:hello:join 主线程等待对等线程终止。Pthread_join函数会阻塞，直到线程tid终止，将线程例程返回的(void*)指针赋值为thread_return指定的位置，然后回收已终止线程占用的所有存储器资源
    exit(0);                                  //line:conc:hello:exit
}

void *thread(void *vargp) /* thread routine */  //line:conc:hello:beginthread 线程的代码和本地数据被封装在一个线程例程中
{                           
    printf("Hello, world!\n");                 
    return NULL;                               //line:conc:hello:return
}                                              //line:conc:hello:endthread
/* $end hello */
