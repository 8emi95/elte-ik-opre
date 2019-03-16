		#include <stdio.h>
		#include <stdlib.h>
		#include <unistd.h> // for pipe()
		#include <string.h>
		//
		// unnamed pipe example
		//
		int main(int argc, char *argv[])
		{
			int pipefd[2];
			pid_t pid;
			char sz[100];  // char array for reading from pipe
			char buffer[1024];
			
			
			char par[200];
			int i=0;
			if(argc<1) {
				printf("Keves argumentum\n");
				exit(EXIT_FAILURE);
			} else {
				strcpy(par, argv[1]);
				for(i=2; i<argc; i++) {
					strcat(par," ");
					strcat(par,argv[i]);
				}
			}
			
			
			//printf("%s\n",par);
			
			if (pipe(pipefd) == -1) 
			{
				perror("Hiba a pipe nyitaskor!");
				exit(EXIT_FAILURE);
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
			{		    	// child process
				//sleep(2);	// sleeping a few seconds, not necessary
				close(pipefd[0]);
				
				

				strcpy(buffer,par);
				sleep(1);
				write(pipefd[1],buffer,sizeof(buffer));
				printf("Gyerek irta: %s\n", buffer);
				sleep(1);

				close(pipefd[1]); // Closing write descriptor
				
				printf("Gyerek befejezte!\n");	
			} 
			else 
			{    // szulo process 
				printf("Szulo indul!\n");
				close(pipefd[1]);
				
				
				
				sleep(1);
				read(pipefd[0], buffer, sizeof(buffer));
				printf("Szulo olvasta: %s\n", buffer);
				
				
				
				close(pipefd[0]); // Closing write descriptor 
				
				fflush(NULL); 	// flushes all write buffers (not necessary)
				wait();
				printf("Szulo befejezte!\n");	
			}
			exit(EXIT_SUCCESS);	// force exit, not necessary
		}


