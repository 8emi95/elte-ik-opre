#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

int main(int argc, char *argv[])
{
    signal(SIGUSR1,handler);
    int pipefd[2]; // unnamed pipe file descriptor array
    char sz[100];  // char array for reading from pipe

    szavazokszama=atoi(argv[1]);
    if (pipe(pipefd) == -1) 
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    pid_t ellen = fork();	// creating parent-child processes
    if (ellen == -1) 
    {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if( ellen>0 )
    {
      pid_t pecstelo = fork();
      if( pecsetelo>0 )
      {
        while( szamlalo != 2) {;} // pause helyett
        elnok();
        wait(NULL); wait(NULL);
      }
      else
      {
        pecset();
      }
    }
    else 
    {
      ellenorzo();
    }
    //exit(EXIT_SUCCESS);	// force exit, not necessary
    return 0;
}
