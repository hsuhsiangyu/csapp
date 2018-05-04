/* $begin waitpid2 */
#include "csapp.h"
#define N 2

int main() 
{
    int status, i;
    pid_t pid[N], retpid;  // 数组存储 pid 号， 

    /* Parent creates N children */
    for (i = 0; i < N; i++) 
	if ((pid[i] = Fork()) == 0)  /* Child */          //line:ecf:waitpid2:fork  父进程按顺序存储子进程的pid  
	    exit(100+i);

    /* Parent reaps N children in order 消除了不确定性，按照父进程创建子进程的相同顺序来回收这些子进程 */
    i = 0;
    while ((retpid = waitpid(pid[i++], &status, 0)) > 0) { //line:ecf:waitpid2:waitpid 父进程通过用适当的pid作为第一个参数来调用waitpid
	if (WIFEXITED(status))  
	    printf("child %d terminated normally with exit status=%d\n",
		   retpid, WEXITSTATUS(status));
	else
	    printf("child %d terminated abnormally\n", retpid);
    }
    
    /* The only normal termination is if there are no more children */
    if (errno != ECHILD) 
	unix_error("waitpid error");

    exit(0);
}
/* $end waitpid2 */
