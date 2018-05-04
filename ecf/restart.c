/* $begin restart */
#include "csapp.h"
// 当用户在键盘上键入ctrl-c时，这个程序用信号和非本地跳转来实现软重启
sigjmp_buf buf;

void handler(int sig) 
{
    siglongjmp(buf, 1); // longjmp可以被信号处理程序使用的版本
}
// 想一想，它会输出什么？
int main() 
{
    Signal(SIGINT, handler);

    if (!sigsetjmp(buf, 1)) // setjmp可以被信号处理程序使用的版本
    	printf("starting\n");
    else 
	    printf("restarting\n");

    while(1) {
	    Sleep(1);
	    printf("processing...\n");
    }
    exit(0);
}
/* $end restart */
