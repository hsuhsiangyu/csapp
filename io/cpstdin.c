/* $begin cpstdin */
#include "csapp.h"

int main(void) 
{
    char c;
//read函数从描述符为STDIN_FILENO的当前文件位置拷贝最多1个字节到存储器位置c
//返回只 -1 表示一个错误，0表示EOF，其他表示实际传送的字节数量。write类似
    while(read(STDIN_FILENO, &c, 1) != 0) 
	write(STDOUT_FILENO, &c, 1);
    exit(0);
}
/* $end cpstdin */
