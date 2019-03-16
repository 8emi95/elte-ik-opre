#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int pipe1[2]; 
    pid_t pid;
    char* message = argv[1];

    if (pipe(pipefd) == -1)
{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    pid = fork();	
    if (pid == -1)
{
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
{		    	// child process
		sleep(1);
        close(pipe1[0]);  
        printf("utas erkezett\n");
        write(pipe1[1],message,sizeof(message));
        printf("utas uzenete: %s\n",message);
        printf("\n");
        close(pipe1[1]); 
    }
    else
    {    // szulo process
        printf("Kozpont varakozik!\n");
        close(pipe1[1]); 
        read(pipe1[0],message,sizeof(message));
		close(pipe1[0]);
        printf("Kozpont cimet kapott: ");
        printf("%s\n",message);
        fflush(NULL); 	// flushes all write buffers (not necessary)
        wait();		// waiting for child process (not necessary)
 // try it without wait()
  printf("Szulo befejezte!");
}
exit(EXIT_SUCCESS);
}