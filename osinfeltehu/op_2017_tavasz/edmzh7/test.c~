#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>


int main(int argc, char *argv[])
{
int pipefd[2];
pid_t pid;
char sz[100];
srand(time(NULL));
int r=rand()%100;

if (pipe(pipefd) == -1)
     {
      perror("Error in pipe opening");
      exit(EXIT_FAILURE);
     }

pid = fork();        // creating parent-child processes
           if (pid == -1)
           {
               perror("Fork error");
               exit(EXIT_FAILURE);
           }

           if (pid == 0)
           {   //child (SECOND PLAYER)
	       printf("Child gives a random number:\n");
               close(pipefd[1]);
               printf("Child says number%i\n", r);
               write(pipefd[1], r);
               close(pipefd[0]);
           }

           else
           {   //parent (FIRST PLAYER)
               printf("Parent gives a random number:\n");
               close(pipefd[0]);
	       printf("Parent says number %i\n", r); 
               write(pipefd[2], r);
               read(pipefd[0],sz,sizeof(sz));
               close(pipefd[1]); 
	   }





 
  
  




}
