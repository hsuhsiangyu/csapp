#include "csapp.h"

int main(){
    if(Fork() == 0){
        printf("a");
    }
    else {
        printf("b");
        waitpid(-1, NULL, 0);
    }
    printf("c");
    exit(0);
}
