/* $begin tfgetsmain */
#include "csapp.h"

char *tfgets(char *s, int size, FILE *stream);

int main() 
{
    char buf[MAXLINE];

    if (tfgets(buf, MAXLINE, stdin) == NULL)
    	printf("BOOM!\n");
    else 
	    printf("%s", buf);

    exit(0);
}

char *tfgets(char *s, int size, FILE *stream){
    register int c;
    register char *cs;
    cs = s;
    while(--size>0 &&(c = getc(stream)) != EOF)
        if((*cs++ = c)=='\n')
            break;
    *cs ='\0';
    return (c==EOF && cs == s) ?NULL :s;
}



/* $end tfgetsmain */
