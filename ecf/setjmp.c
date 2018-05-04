/* $begin setjmp */
#include "csapp.h"

jmp_buf buf;

int error1 = 0; 
int error2 = 1;

void foo(void), bar(void);

int main() 
{
    int rc;

    rc = setjmp(buf); // 在buf缓冲区中保存当前调用环境，以供后面longjmp使用，并返回0
    if (rc == 0)
    	foo();
    else if (rc == 1) 
	    printf("Detected an error1 condition in foo\n");
    else if (rc == 2) 
	    printf("Detected an error2 condition in foo\n");
    else 
	    printf("Unknown error condition in foo\n");
    exit(0);
}

/* Deeply nested function foo */
void foo(void) 
{
    if (error1)
	    longjmp(buf, 1); // 从buf缓冲区中恢复调用环境，然后触发一个从最近一次env的setjmp调用的返回。然后setjmp返回，并带有longjmp第二个参数 1 
    bar();
}

void bar(void) 
{
    if (error2)
	longjmp(buf, 2); 
}
/* $end setjmp */
