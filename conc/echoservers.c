/* 
 * echoservers.c - A concurrent echo server based on select
 */
/* $begin echoserversmain */
#include "csapp.h"
//活动客户端的集合维护在一个pool池结构里
typedef struct { /* represents a pool of connected descriptors */ //line:conc:echoservers:beginpool
    int maxfd;        /* largest descriptor in read_set */   
    fd_set read_set;  /* set of all active descriptors */
    fd_set ready_set; /* subset of descriptors ready for reading  */
    int nready;       /* number of ready descriptors from select */   
    int maxi;         /* highwater index into client array */
    int clientfd[FD_SETSIZE];    /* set of active descriptors */
    rio_t clientrio[FD_SETSIZE]; /* set of active read buffers */
} pool; //line:conc:echoservers:endpool

/* $end echoserversmain */
void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);
void check_clients(pool *p);
/* $begin echoserversmain */

int byte_cnt = 0; /* counts total bytes received by server */

int main(int argc, char **argv)
{
    int listenfd, connfd, port; 
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    static pool pool; 

    if (argc != 2) {
    	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    init_pool(listenfd, &pool); //line:conc:echoservers:initpool
    while (1) {
	/* Wait for listening/connected descriptor(s) to become ready */
	    pool.ready_set = pool.read_set; //pool.read_set 既作为输入参数也作为输出参数，所以在每一次调用时select之前都重新初始化它
	    pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL); // select函数来检测两种不同类型的输入事件，1）来自一个新客户端的连接请求到达，2）一个已存在的客户端的已连接描述符准备好可以读了。

	/* If listening descriptor ready, add new client to pool */
	    if (FD_ISSET(listenfd, &pool.ready_set)) { //line:conc:echoservers:listenfdready
	        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:conc:echoservers:accept
	        add_client(connfd, &pool); //line:conc:echoservers:addclient 将该客户端添加到池中
	    }
	
	/* Echo a text line from each ready connected descriptor */ 
	    check_clients(&pool); //line:conc:echoservers:checkclients 把来自每个准备好的已连接描述符的一个文本行回送回去。
    }
}
/* $end echoserversmain */

/* $begin init_pool */
void init_pool(int listenfd, pool *p) 
{
    /* Initially, there are no connected descriptors */
    int i;
    p->maxi = -1;                   //line:conc:echoservers:beginempty
    for (i=0; i< FD_SETSIZE; i++)   //clientfd数组表示已连接描述符的集合，-1表示一个可用的槽位
    	p->clientfd[i] = -1;        //line:conc:echoservers:endempty

    /* Initially, listenfd is only member of select read set */
    p->maxfd = listenfd;            //line:conc:echoservers:begininit 监听描述符是select读集合中唯一的描述符
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set); //line:conc:echoservers:endinit
}
/* $end init_pool */

/* $begin add_client */
void add_client(int connfd, pool *p) 
{
    int i;
    p->nready--;
    for (i = 0; i < FD_SETSIZE; i++)  /* Find an available slot */
	if (p->clientfd[i] < 0) { 
	    /* Add connected descriptor to the pool */
	    p->clientfd[i] = connfd;                 //line:conc:echoservers:beginaddclient
	    Rio_readinitb(&p->clientrio[i], connfd); //line:conc:echoservers:endaddclient 初始化相应的RIO读缓冲区，这样就能够对这个描述符调用rio_readlineb

	    /* Add the descriptor to descriptor set */
	    FD_SET(connfd, &p->read_set); //line:conc:echoservers:addconnfd 

	    /* Update max descriptor and pool highwater mark 这样check_clients函数就不需要搜索整个数组了 */
	    if (connfd > p->maxfd) //line:conc:echoservers:beginmaxfd
		    p->maxfd = connfd; //line:conc:echoservers:endmaxfd
	    if (i > p->maxi)       //line:conc:echoservers:beginmaxi
		    p->maxi = i;       //line:conc:echoservers:endmaxi
	    break;
	}
    if (i == FD_SETSIZE) /* Couldn't find an empty slot */
	app_error("add_client error: Too many clients");
}
/* $end add_client */

/* $begin check_clients */
void check_clients(pool *p) 
{
    int i, connfd, n;
    char buf[MAXLINE]; 
    rio_t rio;

    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
	connfd = p->clientfd[i];
	rio = p->clientrio[i];

	/* If the descriptor is ready, echo a text line from it */
	if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) { 
	    p->nready--;
	    if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //如果成功的从描述符读取了一个文本行，
		    byte_cnt += n; //line:conc:echoservers:beginecho 
		    printf("Server received %d (%d total) bytes on fd %d\n", 
                n, byte_cnt, connfd);
		    Rio_writen(connfd, buf, n); //line:conc:echoservers:endecho 将该文本行回送到客户端
	    }

	    /* EOF detected, remove descriptor from pool */
	    else { 
    		Close(connfd); //line:conc:echoservers:closeconnfd
	    	FD_CLR(connfd, &p->read_set); //line:conc:echoservers:beginremove 从池中清除掉这个描述符
		    p->clientfd[i] = -1;          //line:conc:echoservers:endremove
	    }
	}
    }
}
/* $end check_clients */

