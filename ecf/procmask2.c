#include "csapp.h"

void initjobs()
{
}

void addjob(int pid)
{
}

void deletejob(int pid)
{
}

/* $begin procmask2 */
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
    sigset_t mask;

    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
    	Sigemptyset(&mask);
	    Sigaddset(&mask, SIGCHLD); 
	    Sigprocmask(SIG_BLOCK, &mask, NULL); /* Block SIGCHLD */
    //在调用fork之前，阻塞SIGCHILD信号
	    /* Child process */
	    if ((pid = Fork()) == 0) {
    // 注意，子进程继承了父进程的被阻塞集合，在Execve之前，小心的解除子进程中阻塞的SIGCHLD信号
	        Sigprocmask(SIG_UNBLOCK, &mask, NULL); /* Unblock SIGCHLD */
	        Execve("/bin/date", argv, NULL);
	    }

	/* Parent process */
	addjob(pid);  /* Add the child to the job list */
    //调用addjob函数后，取消阻塞这些信号
	Sigprocmask(SIG_UNBLOCK, &mask, NULL);  /* Unblock SIGCHLD */
    }
    exit(0);
}
/* $end procmask2 */
