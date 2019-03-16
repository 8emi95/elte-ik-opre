#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main()
{
    signal(SIGUSR1,SIG_IGN);
    int pipefd[2];
    int cn;
    int n;
    char ccn;

    if (pipe(pipefd) == -1){
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    pid_t  splayer=fork();
    if (splayer<0){
        perror("The fork calling was not succesful\n");
        exit(1);
    }

    if (splayer>0) {

        printf("This is first player\n");
        srand(time(0));
        n=rand()%100+1;
        printf("The number is %d\n",n);
        pause();

        printf("wait second player's number\n");
       

        close(pipefd[1]);
        read(pipefd[0],&cn,sizeof(cn));

        
        printf("second player number is: %d\n",cn);

        close(pipefd[0]);
    } else{
        printf("This is second player\n",n);
        kill(getppid(),SIGUSR1);
        printf("Show your number\n");
        srand(time(0));
        cn=rand()%100+1;
	printf("%d\n",cn);	

        close(pipefd[0]);
        write(pipefd[1],&cn,sizeof(cn));
        close(pipefd[1]);
        fflush(NULL);

    }

    return 0;
}
