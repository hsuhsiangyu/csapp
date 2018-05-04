/*
 * rfork.c - Wrapper for fork() that introduces non-determinism
 *           in the order that the parent and child are executed
 * /基本思想是每次调用fork之后，父进程和子进程扔一枚硬币决定谁会休眠一会儿
 * 可以测试到父子进程执行的两种顺序，无论这个特定内核的调度策略是什么样的。
/* $begin rfork */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

/* Sleep for a random period between [0, MAX_SLEEP] us */
#define MAX_SLEEP 100000

/* Macro that maps val into the range [0, RAND_MAX] */
#define CONVERT(val) (((double)val)/(double)RAND_MAX)

pid_t Fork(void)
{
    static struct timeval time;
    unsigned bool, secs;
    pid_t pid;
    
    /* Generate a different seed each time the function is called */
    gettimeofday(&time, NULL);
    srand(time.tv_usec); 

    /* Determine whether to sleep in parent of child and for how long */
    bool = (unsigned)(CONVERT(rand()) + 0.5);
    secs = (unsigned)(CONVERT(rand()) * MAX_SLEEP);

    /* Call the real fork function */
    if ((pid = fork()) < 0) 
	return pid;

    /* Randomly decide to sleep in the parent or the child */
    if (pid == 0) { /* Child */
	if(bool) {
	    usleep(secs);
	}
    }
    else { /* Parent */
	if (!bool) {
	    usleep(secs);
	}
    }

    /* Return the PID like a normal fork call */
    return pid;
}
/* $end rfork */
