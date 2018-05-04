/* $begin signal2 */
#include "csapp.h"

void handler2(int sig) 
{
    pid_t pid;
  
    while ((pid = waitpid(-1, NULL, 0)) > 0) // 修改的地方 >0
	    printf("Handler reaped child %d\n", (int)pid);
    if (errno != ECHILD)// 被回收时，errno==ECHILD
    	unix_error("waitpid error");
    Sleep(2);
    return;
}
// 但还存在问题，它不具有移植性，在特定的Solaris系统上，诸如read这样的慢速系统调用在信号发送中断后，是不会自动重启的。所以必须考虑系统调用过早返回的可能性，然后当它发生时手动重启它们。
int main() 
{
    int i, n;
    char buf[MAXBUF];

    if (signal(SIGCHLD, handler2) == SIG_ERR)
	    unix_error("signal error");

    /* Parent creates children */
    for (i = 0; i < 3; i++) {
	    if (Fork() == 0) {
	        printf("Hello from child %d\n", (int)getpid());
	        Sleep(1);
	        exit(0);
	    }
    }

    /* Parent waits for terminal input and then processes it */
    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
	    unix_error("read error");

    printf("Parent processing input\n");
    while (1)
	;

    exit(0);
}
/* $end signal2 */
