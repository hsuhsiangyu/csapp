/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
    	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	    exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd); 

    while (Fgets(buf, MAXLINE, stdin) != NULL) { // 反复从标准输入读取文本行
	    Rio_writen(clientfd, buf, strlen(buf));// 发送文本行给服务器
	    Rio_readlineb(&rio, buf, MAXLINE);     // 将从服务器读取回送的行
	    Fputs(buf, stdout);             // 并输出结果到标准输出
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
