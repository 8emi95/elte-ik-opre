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
    char* uzenet;  // char array for reading from pipe

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
		uzenet = argv[1];
        close(pipefd[0]); 
        printf("utas erkezett !\n");
        //strcpy(uzenet,argv[2]);
        write(pipefd[1],uzenet,sizeof(uzenet)); //
        printf("utas cimet mondja: %s\n",uzenet);
        printf("\n");
        close(pipefd[1]); 
    }
    else
    {    // szulo process
        printf("Kozpont varakozik!\n");
        close(pipefd[1]); 
        read(pipefd[0],uzenet,sizeof(uzenet));
        printf("Kozpont olvasta cim: ");
        printf("%s\n",uzenet);
        fflush(NULL); 	
        wait();		
  printf("Szulo befejezte!");
}
exit(EXIT_SUCCESS);	// force exit, not necessary
}
