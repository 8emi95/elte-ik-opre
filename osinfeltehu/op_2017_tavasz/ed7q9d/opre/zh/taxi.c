#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char* lakcim[])
{	
	
	int pipefd[2]; // unnamed pipe file descriptor array
    pid_t pid;
    char kuldes[100];  // char array for reading from pipe
	char fogadas[100];
	int i = 1;
	memset(&kuldes,0,sizeof(kuldes));
	for (i;i<argc;++i)
	{
		strcat(kuldes,lakcim[i]);
		strcat(kuldes," ");
	}
	

    if (pipe(pipefd) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    pid = fork();	// creating parent-child processes
    if (pid == -1) 
	{
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) 
	{	// child process
        close(pipefd[0]);  //Usually we close the unused write end
	    write(pipefd[1], kuldes, sizeof(kuldes));
		fflush(NULL); 	// flushes all write buffers (not necessary)
        close(pipefd[0]); // finally we close the used read end
    } 
    else 
    {    // szulo process 
        close(pipefd[1]); //Usually we close unused read end
		memset(&fogadas,0,sizeof(fogadas));
        read(pipefd[0], fogadas,sizeof(fogadas));
		printf("Új fuvar érkezett: lakcím: %s \n", fogadas);
        close(pipefd[1]); // Closing write descriptor 
        wait();		// waiting for child process (not necessary)
	}
	return 0;
}