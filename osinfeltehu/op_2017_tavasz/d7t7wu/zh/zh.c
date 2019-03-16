
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

typedef struct
{
unsigned int t;
unsigned int nr;
}mess;
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Nincs megadva a lakcim");
    }
    else
    {
        int pipefd[2]; // unnamed pipe file descriptor array
        pid_t pid;
        char sz[100];  // char array for reading from pipe

        if (pipe(pipefd) == -1)
        {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
        }
        int i = 0;
            pid = fork();	// creating parent-child processes
            if (pid == -1)
            {
                perror("Fork hiba");
                exit(EXIT_FAILURE);
            }

            if (pid > 0)
            {

                sleep(3);	// sleeping a few seconds, not necessary
                close(pipefd[1]);  //Usually we close the unused write end
                printf("A szulo elkezdi olvasni a csobol az adatokat!\n");
                read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
                printf("A lakcim: %s",sz);
                printf("\n");
                close(pipefd[0]); // finally we close the used read end

                pid_t child2 = fork();
                if (pid == -1)
                {
                    perror("Fork hiba");
                    exit(EXIT_FAILURE);
                }
                if(child2>0)
                {
                    //szulo

                    int status;
                    while( wait(&status)!=-1) {}
                    printf("Szulo vege\n");
                }
                else
                {
                    printf("%d az auto\n",getpid());
                }

            }
            else
            {
                //gyerek

                close(pipefd[0]); //Usually we close unused read end
                write(pipefd[1], argv[1],sizeof(argv[1]));
                close(pipefd[1]); // Closing write descriptor
            }

        return 0;
    }
}
