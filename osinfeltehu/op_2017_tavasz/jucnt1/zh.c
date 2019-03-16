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

    if (pipe(pipefd) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    /* create child */
    pid_t child = fork();
    if (child < 0) 
    {
        perror ("fork() error!");
        exit(-1);
    }

    /* parent process */
    if (0 != child)    
    {         
        char buffer[100];
		pause();
		read(pipefd[0],buffer,sizeof(buffer)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s\n\n", buffer);
        sleep(1);
        kill(getppid(), SIGTERM);

      waitpid(child, &status, 0); 
    }

    /* child process */
    else
    {
        sleep(2);
		char buf[100];
		strcpy(buf,argv[1]);
        printf("Szulo indul!\n");
        write(pipefd[1], buf, sizeof(buf));
        sleep(1);
        kill(child, SIGTERM);
    }

    return 0;
}

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
}

int countLines(char *filename)
{
    FILE *fp = fopen(filename, "r");

    char c;
    int lines = 1;

    while ((c = fgetc(fp)))
    {
        if (c == EOF)
            break;
        if (c == '\n')
        {
            lines++;
        }
    }
    fclose(fp);

    return lines;
}