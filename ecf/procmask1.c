#include "csapp.h"

void initjobs()
{
}

void addjob(int pid) // 对于父进程的main函数流和信号处理流的某些交错，可能会在addjob之前调用deletejob，导致作业列表中出现一个不正确的条目。这就是竞争，好好理解下代码
{
}

void deletejob(int pid)
{
}

/* $begin procmask1 */
void handler(int sig)
{
    pid_t pid;
    while ((pid = waitpid(-1, NULL, 0)) > 0) /* Reap a zombie child */
	deletejob(pid); /* Delete the child from the job list */
    if (errno != ECHILD)
	unix_error("waitpid error");
}
    
int main(int argc, char **argv)
{
    int pid;

    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
	/* Child process */
	if ((pid = Fork()) == 0) {
	    Execve("/bin/date", argv, NULL);
	}

	/* Parent process */
	addjob(pid);  /* Add the child to the job list */
    }
    exit(0);
}
/* $end procmask1 */
