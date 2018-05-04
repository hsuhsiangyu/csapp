/* 
 * echoservert_pre.c - A prethreaded concurrent echo server 使用生产者-消费者模型来降低（为每一个客户端创建一个新线程的）开销。
 * 服务器是一个主线程和一组工作者线程构成的，主线程不断的接受来自客户端的连接请求，并将得到的连接描述符放在一个有限缓冲区中，每一个工作者线程反复的从共享缓冲区中
 * 取出描述符，为客户端服务，然后等待下一个描述符。
 */
/* $begin echoservertpremain */
#include "csapp.h"
#include "sbuf.h"
#define NTHREADS  4
#define SBUFSIZE  16

void echo_cnt(int connfd);
void *thread(void *vargp);

sbuf_t sbuf; /* shared buffer of connected descriptors */

int main(int argc, char **argv) 
{
    int i, listenfd, connfd, port;
    socklen_t clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid; 

    if (argc != 2) {
    	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(0);
    }
    port = atoi(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE); //line:conc:pre:initsbuf
    listenfd = Open_listenfd(port);

    for (i = 0; i < NTHREADS; i++)  /* Create worker threads */ //line:conc:pre:begincreate
	    Pthread_create(&tid, NULL, thread, NULL);               //line:conc:pre:endcreate 

    while (1) { 
	connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
	sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }
}

void *thread(void *vargp) 
{  
    Pthread_detach(pthread_self()); 
    while (1) { 
	    int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */ //line:conc:pre:removeconnfd 等待直到它能从缓冲区中取出一个已连接描述符
	    echo_cnt(connfd);                /* Service client */
	    Close(connfd);
    }
}
/* $end echoservertpremain */
