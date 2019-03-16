#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int signumber);

int main(int argc, char *argv[])
{
    
    signal(SIGTERM,handler);
    int status;
    int pipefd[2];
    
    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    pid_t child = fork();
    if (child < 0)
    {
        perror ("fork() error!");
        exit(-1);
    }
    if (0 != child)
    {
        printf("TAXITÁRSASÁG: Elindultam!\n");
        char buffer[100];
        pause();
        read(pipefd[0],buffer,sizeof(buffer)); // reading max 100 chars
        printf("TAXITÁRSASÁG: Megkaptam a címet: %s\n", buffer);
        sleep(1);
        waitpid(child, &status, 0);
        printf("TAXITÁRSASÁG: Végeztem!\n");
    }
    
    /* child process */
    else
    {
        printf("UTAS: Elindultam!\n");
        sleep(1);
        printf("UTAS: Küldöm a címet!\n");
        write(pipefd[1], argv[1], sizeof(argv[1]));
        sleep(1);
        kill(child, SIGTERM);
        printf("UTAS: Végeztem!\n");
    }
    
    return 0;
}

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
}
