/* $begin sigint1 */
#include "csapp.h"
/*当输入ctrl-c时外壳发送的SIGINT信号，其默认行为时立即终止该进程，在此实例中，我们将默认行为修改为捕获信号，输出一条信息，然后终止该进程。*/
void handler(int sig) /* SIGINT handler */   //line:ecf:sigint1:beginhandler
{
    printf("Caught SIGINT\n");               //line:ecf:sigint1:printhandler
    exit(0);                                 //line:ecf:sigint1:exithandler
}                                            //line:ecf:sigint1:endhandler

int main() // 信号处理程序就是并发的一个实例，它与main函数并发的运行
{
    /* Install the SIGINT handler 设置处理程序，然后进入休眠状态，直到接收到一个信号 */         
    if (signal(SIGINT, handler) == SIG_ERR)  //line:ecf:sigint1:begininstall
    	unix_error("signal error");          //line:ecf:sigint1:endinstall
    
    pause(); /* Wait for the receipt of a signal */  //line:ecf:sigint1:pause
    
    exit(0);
}
/* $end sigint1 */
