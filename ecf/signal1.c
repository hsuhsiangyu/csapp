/* $begin signal1 */
#include "csapp.h"

void handler1(int sig) 
{
    pid_t pid;

    if ((pid = waitpid(-1, NULL, 0)) < 0) // 回收子进程
    	unix_error("waitpid error");
    printf("Handler reaped child %d\n", (int)pid);
    Sleep(2);
    return;
}
// 这个程序有缺陷，它无法处理信号阻塞、信号不排队等待和系统调用被中断等情况
/* 因次父进程只回收了两个子进程，有个子进程成为僵死进程
 * 父进程接收并捕获第一个信号。当处理程序还在处理第一个信号时，第二个信号就传送并添加到了待处理信号集合里。第三个信号到达了，因为已经有了一个待处理的的SIGCHLD,第三个SIGCHLD信号会被丢弃。一段时间后处理程序返回，内核注意到有个待处理信号，就迫使父进程接收这个信号。
 * 教训是：不可以用信号来对其他进程中发生的事件计数
 *
 * */
int main() 
{
    int i, n;
    char buf[MAXBUF];

    if (signal(SIGCHLD, handler1) == SIG_ERR)
	unix_error("signal error");

    /* Parent creates children */
    for (i = 0; i < 3; i++) {
	    if (Fork() == 0) { 
	        printf("Hello from child %d\n", (int)getpid());
	        Sleep(1);
	        exit(0); // 当子进程终止时，内核通过发送一个SIGCHLD信号通知父进程。
	    }
    }

    /* Parent waits for terminal input and then processes it */
    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
	unix_error("read");

    printf("Parent processing input\n");
    while (1)
	; 

    exit(0);
}
/* $end signal1 */
