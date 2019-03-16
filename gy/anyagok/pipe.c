#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

int main(int argc, char *argv[]) // char** argv?
{
    int pipefd[2]; //ez lesz az iro-olvaso vegu cso; O: olvaso veg, 1: iro veg
    pid_t pid;                  //egyszerre csak egyik veget hasznaljuk
    char buffer[100]; //ebbe olvasunk;

    if (pipe(pipefd) == -1) //megprobaljuk megnyitni a pipe-ot
    {
        perror("COULD NOT OPEN PIPE");
        exit(EXIT_FAILURE);
    }

    pid = fork(); //letrehozzuk a gyerekfolyamatot
    if (pid == -1)
    {
        perror("FORK ERROR");
        exit(EXIT_FAILURE);
    }

    if (pid != 0) // szulo process
    {
        puts("Parent process started...");
        close(pipefd[0]); //most nem hasznaljuk az olvaso veget, ezert bezarjuk
        write(pipefd[1], "CICA", 5); //5: kuldott uzenet merete (4 char + '\0') 
        close(pipefd[1]); //bezarjuk a csovet
        puts("[PARENT] Message sent through pipe.");
        fflush(NULL); // flushes all write buffers (not necessary)
        wait();       // waiting for child process to terminate (not necessary)
        puts("Parent process over.");
    }
    else // child process
    {
        puts("Child process started...");
        sleep(3);         //not necessary
        close(pipefd[1]); //olvasni fogunk, ezert a write endet csukjuk be
        puts("[CHILD] Child is up to read a message.");
        read(pipefd[0], buffer, sizeof(buffer)); // a bufferbe olvas
        printf("[CHILD]: Message received: %s\n", buffer);
        close(pipefd[0]); // bezarjuk az olvaso veget is
        puts("Child process over.");
    }
    exit(EXIT_SUCCESS); // force exit, not necessary

    return 0;
}