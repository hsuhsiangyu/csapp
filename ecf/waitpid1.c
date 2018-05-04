/* $begin waitpid1 */
#include "csapp.h"
#define N 2

int main() 
{
    int status, i;
    pid_t pid;

    /* Parent creates N children */
    for (i = 0; i < N; i++)                       //line:ecf:waitpid1:for
	if ((pid = Fork()) == 0)  /* child */     //line:ecf:waitpid1:fork
	    exit(100+i);                          //line:ecf:waitpid1:exit

    /* Parent reaps N children in no particular order 在这里顺序是不确定的 */
    while ((pid = waitpid(-1, &status, 0)) > 0) { //line:ecf:waitpid1:waitpid 第一个参数为-1，所以对waitpid的调用会阻塞，直到任意一个子进程终止。
	if (WIFEXITED(status))                    //line:ecf:waitpid1:wifexited 检查子进程的退出状态
	    printf("child %d terminated normally with exit status=%d\n",
		   pid, WEXITSTATUS(status));     //line:ecf:waitpid1:wexitstatus 父进程提取出退出状态
	else
	    printf("child %d terminated abnormally\n", pid);
    }
/*当回收所有的子进程后，再调用waitpid就返回-1，并且设置errno为ECHILD，所以下面一行是检查waitpid函数是否正常终止 */
    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)                          //line:ecf:waitpid1:errno
	unix_error("waitpid error");

    exit(0);
}
/* $end waitpid1 */
