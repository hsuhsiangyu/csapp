/* 
 * A thread-safe version of echo that counts the total number
 * of bytes received from clients. 这个程序展示了一个从线程例程调用的初始化程序包的一般技术。
 */
/* $begin echo_cnt */
#include "csapp.h"
// 需要初始化byte_cnt 计数器和mutex 信号量，本程序使用Pthread_once去调用初始化函数
static int byte_cnt;  /* byte counter */
static sem_t mutex;   /* and the mutex that protects it */

static void init_echo_cnt(void)
{
    Sem_init(&mutex, 0, 1);
    byte_cnt = 0;
}

void echo_cnt(int connfd) 
{
    int n; 
    char buf[MAXLINE]; 
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    Pthread_once(&once, init_echo_cnt); //line:conc:pre:pthreadonce  
    Rio_readinitb(&rio, connfd);        //line:conc:pre:rioinitb
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
	P(&mutex);
	byte_cnt += n; //line:conc:pre:cntaccess1
	printf("thread %d received %d (%d total) bytes on fd %d\n", 
	       (int) pthread_self(), n, byte_cnt, connfd); //line:conc:pre:cntaccess2
	V(&mutex);
	Rio_writen(connfd, buf, n);
    }
}
/* $end echo_cnt */
