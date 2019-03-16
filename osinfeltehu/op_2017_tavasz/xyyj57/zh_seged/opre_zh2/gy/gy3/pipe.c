       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <signal.h>
	#include <sys/types.h>

	//
	// unnamed pipe example
	//
		void handler(int signumber){
		  printf("Signal with number %i has arrived\n",signumber);
		}
	
       int main(int argc, char *argv[])
       {
		   signal(SIGUSR1, handler);
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[100];  // char array for reading from pipe

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
	      
             //  
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               
			   read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
			   printf("Gyerek olvasta uzenet: %s\n",sz);
			   
			   kill(getppid(), SIGUSR1);
			  
			  write(pipefd[1], "Hajra !",13);
			   printf("Gyerek beirta az adatokat a csobe!\n");
               close(pipefd[1]);  //Usually we close the unused write end
			   
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
              // 
               
			   write(pipefd[1], "Hajra Fradi! \n",13);
			   printf("Szulo beirta az adatokat a csobe!\n");
			   pause();
			   read(pipefd[0], sz, sizeof(sz));
			   printf("Szulo olvasta az uzenet: %s\n",sz);
			   close(pipefd[0]); //Usually we close unused read end
			   
               close(pipefd[1]); // Closing write descriptor 
              
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!\n");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
