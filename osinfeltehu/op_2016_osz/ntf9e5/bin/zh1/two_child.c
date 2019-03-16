#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{  
pid_t child1 = fork();

  char message[50];
  char message_read[50];

	int firstpipe[2];
	int secondpipe[2];
 if (pipe(firstpipe) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
 if (pipe(secondpipe) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
 
  if(child1 < 0)
  {
    printf("Error.\n");
  }
  else if (child1 > 0)
  {
    pid_t child2 = fork();
    if (child2 < 0)
    { 
       printf("Error\n");
    }
    else if (child2 > 0)
    {
		   printf("Parent Started\n");
		   sleep(3);
/*		   strcpy(message,"55");
		   close(firstpipe[0]);
		   close(secondpipe[0]);
		   write(firstpipe[1], message, sizeof(message));
		   strcpy(message,"60");
		   write(secondpipe[1], message, sizeof(message));
		   close(firstpipe[1]);
		   close(secondpipe[1]);
		   close(pipefd[0][0]);
		   write(pipefd[0][1], message, sizeof(message));
		   close(pipefd[0][1]);
			fflush(NULL);
			strcpy(message,"something else message");
		close(pipefd[1][0]);
		     write(pipefd[1][1], message, sizeof(message));
		    close(pipefd[1][1]);*/
			printf("Parent Ended\n");
		   //sleep(1);
		
    }
    else
    {
		sleep(1);
	   printf("Child 2 Started\n");
/*	   close(secondpipe[1]);
	   read(secondpipe[0], message_read, sizeof(message_read));
	   close(secondpipe[0]);
	   close(pipefd[1][1]);
	   read(pipefd[1][0], message_read, sizeof(message_read));
	   close(pipefd[1][0]); 
	   printf("Child 2: %s\n", message_read);
	   fflush(NULL);
	    close(pipefd[1]);
		read(pipefd[0], message_read, sizeof(message_read));
		close(pipefd[0]);
		printf("Child 2: %i\n", atoi(message_read) + 1 );*/
		printf("Child 2 Ended\n");
	   //sleep(1);
    }
  }
  else
  {
		sleep(1);
     printf("Child 1 Started\n");
	 printf("Child 1 Ended\n");
	 //sleep(1);
   }
  return 0;
}
