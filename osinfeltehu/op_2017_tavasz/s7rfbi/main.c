#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>



int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t pid;
    char sz[100];

    srand(time(NULL)); //the starting value of random number generation
    int r=rand()%100; //number between 0-99
    printf("%i\n",r);

    if (pipe(pipefd) == -1)
    {
        perror("Opening error!");
        exit(EXIT_FAILURE);
    }
    pid = fork();	// creating parent-child processes
    if (pid == -1)
    {
        perror("Fork error");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // child process
        sleep(3);	// sleeping a few seconds, not necessary
        close(pipefd[1]);  //Usually we close the unused write end
        printf("Child starts to read from the pipe!\n");
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Child read the message: %s\n",sz);
        printf("\n");
        close(pipefd[0]);
    }
    else
    {

        printf("Parent starts!\n");
        close(pipefd[0]);
        write(pipefd[1], "Hurray Fradi!",13);
        close(pipefd[1]);
        printf("Parent wrote the message to the pipe!\n");
        fflush(NULL);
        wait();
        printf("Parent finished!\n");
    }
    exit(EXIT_SUCCESS);
}


