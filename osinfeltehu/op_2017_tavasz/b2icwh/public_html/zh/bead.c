#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
//
// unnamed pipe example
//
int main(int argc, char *argv[])
{
    int pipefd[2]; // unnamed pipe file descriptor array
    pid_t pid;
    char* uzenet = argv[1];  // char array for reading from pipe

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
{		    	// child process

        close(pipefd[0]);  //close read
        printf("utas jott !\n");
        //strcpy(uzenet,argv[2]);
        write(pipefd[0],uzenet,sizeof(uzenet)); //
        printf("utas uzenete: %s\n",uzenet);
        printf("\n");
        close(pipefd[1]); // finally we close the used read end
    }
    else
    {    // szulo process
        printf("Kozpont varja az utasok erdeklodeset!\n");
        close(pipefd[1]); //Usually we close unused read end
        read(pipefd[0],uzenet,sizeof(uzenet));
        printf("Kozpont olvasta cim: ");
        printf("%s\n",uzenet);
        fflush(NULL); 	// flushes all write buffers (not necessary)
        wait();		// waiting for child process (not necessary)
 // try it without wait()
  printf("Szulo befejezte!");
}
exit(EXIT_SUCCESS);	// force exit, not necessary
}
