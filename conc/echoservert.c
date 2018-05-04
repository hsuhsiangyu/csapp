/* 
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char **argv) 
{
    int listenfd, *connfdp, port;
    socklen_t clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid; 

    if (argc != 2) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    while (1) {
	    connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc 这是为了避免这种潜在的致命竞争，必须将每个accept返回的已连接描述符分配到它自己的动态分配的存储器块
	    *connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc。这样在对等线程的赋值语句和主线程的accept语句之间引入了竞争，结果就是两个线程在同一个描述符上执行输入和输出
	    Pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* thread routine */
void *thread(void *vargp) 
{  
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach 不显式的收回线程，就必须分离每个线程，使得它终止时它的存储器资源能够被收回
    Free(vargp);                    //line:conc:echoservert:free 小心释放主线程分配的存储器块
    echo(connfd);
    Close(connfd);                  //线程运行在同一个进程中，它们都共享相同的描述符表，已连接描述表的文件表的引用计数总等于1，所以一个close就足够了
    return NULL;
}
/* $end echoservertmain */
