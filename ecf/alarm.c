/* $begin alarm */
#include "csapp.h"

void handler(int sig) 
{
    static int beeps = 0;

    printf("BEEP\n");
    if (++beeps < 5)  
	Alarm(1); /* Next SIGALRM will be delivered in 1 second */
    else {
	printf("BOOM!\n");
	exit(0);
    }
} // handle返回时，控制传递回main函数，从当初被信号到达时中断了的地方继续执行

int main() 
{
    Signal(SIGALRM, handler); /* install SIGALRM handler 使用signal函数设置一个信号处理函数，只要进程收到一个SIGALRM信号，就异步的调用该函数，中断main程序中的无限while循环*/
    Alarm(1); /* Next SIGALRM will be delivered in 1s */

    while (1) {
	;  /* Signal handler returns control here each time */
    }
    exit(0);
}
/* $end alarm */
