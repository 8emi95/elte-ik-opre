#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>

void handler(int signum)
{
    printf("A gyerek (PID: %i) bejelentkezett a szulohoz.\n", signum);
}

void interruptHandler()
{
    printf("\n");
    exit(1);
}

int main(int argc, char** argv)
{
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGINT, interruptHandler);

    int pipefd[2];
    if(pipe(pipefd) == -1)
    {
        perror("Nem sikerult a cso letrehozasa.\n");
        exit(1);
    }
    
    pid_t child1;
    pid_t child2;
    
    child1 = fork();
    if(child1 < 0) { perror("Nem sikerult a gyerek letrehozasa.\n"); exit(1); } 
    if(child1 == 0) // child1 process
    {
        printf("Child1 process indul.\n");
        sleep(1);
        kill(getppid(), SIGUSR1);

        close(pipefd[1]);
        char reader[100];
        read(pipefd[0], &reader, 14);
        close(pipefd[0]);
        
        printf("Child1 process olvasta: %s\n", reader);      
        printf("Child1 process vege.\n");
    }else
    {
        child2 = fork();
        if(child2 < 0) { perror("Nem sikerult a gyerek letrehozasa.\n"); exit(1); }
        if(child2 == 0) // child2 process
        {
            printf("Child2 process indul.\n");
            sleep(2);
            kill(getppid(), SIGUSR2);
            printf("Child2 process vege.\n");
        }else // parent process
        {
             printf("Parent process indul.\n");
             pause();
             pause();

             close(pipefd[0]);
             write(pipefd[1], "Turbek turbek!", 14);
             close(pipefd[1]);

             while(wait(NULL) > 0);
             printf("Parent process vege.\n");
        }
    }
    return 0;
}
