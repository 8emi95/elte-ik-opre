#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#include "projhead.h"

void handler(int sgn){;}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    time_t t;
    srand(time(&t));

    pid_t pid1, pid2;
    int pfds[2]; // 0: read end, 1: write end
    pipe(pfds);
    switch(pid1 = fork()) {
    case -1:
        perror("fork"); 
        exit(1); 
    case 0: // child
        printf("ELLENORZO: This is the ELLENORZO process!\n");
        ellenorzo_main(pfds);        
        printf("ELLENORZO: I'm outta here!\n");
        exit(0);
    default: //parent
        printf("SZULO: This is the SZULO process!\n");
        int rv;
        switch(pid2 = fork()) {
            case -1:
                perror("fork");  
                exit(1);         
            case 0: // child
                printf("PECSETELO: This is the PECSETELO process!\n");
                kill(getppid(), SIGUSR2);
                printf("PECSETELO: I'm outta here!\n");
                exit(0);
            default: //parent
                break;
        }
        szulo_main(argc, argv, pfds);
        waitpid(pid1, &rv, 0);
        printf("SZULO: ELLENORZO's exit status is: %d\n", WEXITSTATUS(rv));
        waitpid(pid2, &rv, 0);
        printf("SZULO: PECSETELO's exit status is: %d\n", WEXITSTATUS(rv));
                
        printf("SZULO: I'm outta here!\n");
}
return 0; }