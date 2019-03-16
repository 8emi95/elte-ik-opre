		#include <stdio.h>
		#include <stdlib.h>
		#include <unistd.h> // for pipe()
		#include <string.h>
		//
		// unnamed pipe example
		//
		int main(int argc, char *argv[])
		{
			int pipefdCtP[2]; // unnamed pipe file descriptor array
			int pipefdPtC[2]; // unnamed pipe file descriptor array
			pid_t pid;
			char sz[100];  // char array for reading from pipe
			char buffergy[512];
			char buffersz[512];
			
			
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
			
			
			printf("%s\n",par);
			//strcpy(buffersz,par);
			if (pipe(pipefdPtC) == -1) 
			{
				perror("Hiba a pipe nyitaskor!");
				exit(EXIT_FAILURE);
			}
			if (pipe(pipefdCtP) == -1) 
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
				//close(pipefdPtC[0]);
				//close(pipefdCtP[1]);
				
				
				strcpy(buffergy, "");
				write(pipefdCtP[1],buffergy,sizeof(buffergy));
				strcpy(buffergy,par);
				sleep(1);
				write(pipefdCtP[1],buffergy,sizeof(buffergy));
				printf("Gyerek irta: %s\n", buffergy);
				
				sleep(1);
				read(pipefdPtC[0], buffergy, sizeof(buffergy));
				printf("Gyerek olvasta: %s\n", buffergy);
				

				close(pipefdPtC[1]); // Closing write descriptor 
				close(pipefdCtP[0]);
				printf("Szulo befejezte!\n");	
			} 
			else 
			{    // szulo process 
				printf("Szulo indul!\n");
				//close(pipefdPtC[0]);
				//close(pipefdCtP[1]);
				
				read(pipefdCtP[0], buffersz, sizeof(buffersz));
				
				sleep(2);
				read(pipefdCtP[0], buffersz, sizeof(buffersz));
				printf("Szulo olvasta: %s\n", buffersz);
				sleep(1);
				strcpy(buffersz, "Masodik szoveg");
				write(pipefdPtC[1],buffersz,sizeof(buffersz));
				
				printf("Szulo irta: %s\n", buffersz);
				printf("Szulo irta: %s\n", buffersz);
				printf("Szulo irta: %s\n", buffersz);
				
				
				
				close(pipefdPtC[1]); // Closing write descriptor 
				close(pipefdCtP[0]);
				
				fflush(NULL); 	// flushes all write buffers (not necessary)
				printf("Szulo befejezte!\n");	
			}
			exit(EXIT_SUCCESS);	// force exit, not necessary
		}


