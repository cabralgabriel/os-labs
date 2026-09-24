#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void){

    pid_t p = fork();

    // caso o "p" comparado seja o filho:
    if (p == 0){ 
        printf("Processo filho | pid: %d\n", getpid());
        exit(0);
    }
    // caso o "p" comparado seja o processo pai:
    else if(p > 0){
        printf("Processo pai | pid: %d\n", p);
        // como esta sem wait() aqui, o processo filho vira um processo zumbi depois do exit()
    }

    return 0;
}
