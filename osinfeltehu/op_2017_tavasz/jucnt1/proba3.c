#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int signumber);

int countLines(char *filename);

int main(int argc, char *argv[])
{

    signal(SIGTERM,handler);
    int status;
    /* create pipe */
    int pipefd[2];
    int pipefd2[2];
    if (pipe(pipefd) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
     if (pipe(pipefd2) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }


    /* create child */
    pid_t child = fork();
   pid_t child2;
    if (child < 0) 
    {
        perror ("fork() error!");
        exit(-1);
    }

    /* parent process */
    if (0 != child)    
    {         
        pause();
        child2=fork();
        pause();
        if(child2==0)
        {
            kill(getppid(), SIGTERM);
            char buffer[15];
            pause();
            /* Kiolvasom a csovezetekbol az adatot */
            //close(pipefd[1]);  //Usually we close the unused write end
            printf("Gyerek elkezdi kiolvasni a dolgokat!\n");
            read(pipefd2[0],buffer,sizeof(buffer)); // reading max 100 chars
            printf("Gyerek olvasta uzenet: %s\n\n", buffer);
            sleep(1);
            kill(getppid(), SIGTERM);

        }
        char buf[]="Hajra Fradi";
        printf("Szulo indul!\n");
        //close(pipefd[0]); //Usually we close unused read end
       write(pipefd[1], buf, sizeof(buf));
       sleep(1);
       kill(child, SIGTERM);
       pause();
               printf("Szulo indul!\n");
        //close(pipefd[0]); //Usually we close unused read end
       write(pipefd2[1], buf, sizeof(buf));
       sleep(1);
       kill(child2, SIGTERM);
       pause();
       while(!wait(NULL));
      //waitpid(child, &status, 0); 
    }

    /* child process */
    else
    {
            sleep(1);
            kill(getppid(), SIGTERM);
            char buffer[15];
            pause();
            /* Kiolvasom a csovezetekbol az adatot */
            //close(pipefd[1]);  //Usually we close the unused write end
            printf("Gyerek elkezdi kiolvasni a dolgokat!\n");
            read(pipefd[0],buffer,sizeof(buffer)); // reading max 100 chars
            printf("Gyerek olvasta uzenet: %s\n\n", buffer);
            sleep(1);
            kill(getppid(), SIGTERM);
    }

    return 0;
}

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
}

