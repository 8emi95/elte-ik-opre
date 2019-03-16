	#include <stdlib.h>  
	#include <stdio.h>

	#include <sys/stat.h>

	#include <string.h> //strlen
	#include <fcntl.h> //lock
	#include <errno.h> //perror, errno
	#include <sys/types.h> //open
	#include <sys/stat.h>
	#include <time.h>


	#define MAX_INPUT 256
	   
       int main(int argc, char *argv[])
       {
		   
		   char userAddress[MAX_INPUT];
		   strcpy(userAddress, argv[1]);
		   
           int pipefd[2];
           pid_t pid;
           char sz[MAX_INPUT];

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
	   {	
				close(pipefd[0]); 
				
				char address[MAX_INPUT];
				// printf("Please provide your address.:\n");
				// fgets(address, MAX_INPUT, stdin);
				// printf("The address is : %s\n", address);
				
				strcpy(sz, userAddress);
				//strcpy(sz, address);
				write(pipefd[1], sz, sizeof(sz));
				
				sleep(2);
			
				printf("\n");
				close(pipefd[1]);
				
				printf("Child is done.\n");
           } 
           else 
           {    // szulo process 
				printf("Szulo indul!\n");
				close(pipefd[1]);
				
				read(pipefd[0], sz, sizeof(sz));
				printf("Address of user is: %s\n", sz);
				
				close(pipefd[0]);
				fflush(NULL);
				wait();		// megvárja a gyerek folyamatot

				printf("Parent is done.\n");	
	   }
	   
	   exit(EXIT_SUCCESS);	// force exit, not necessary
	}
