#include <stdio.h>
#include <stdlib.h>
// rand函数的可重入版本
/* $begin rand_r */
/* rand_r - a reentrant pseudo-random integer on 0..32767 */
int rand_r(unsigned int *nextp)  //关键思想是用一个调用者传递进来的指针取代了静态的next变量
{
    *nextp = *nextp * 1103515245 + 12345;
    return (unsigned int)(*nextp / 65536) % 32768;
}
/* $end rand_r */

int main()
{
    unsigned int next = 1;

    printf("%d\n", rand_r(&next));
    printf("%d\n", rand_r(&next));
    printf("%d\n", rand_r(&next));
    exit(0);
}

