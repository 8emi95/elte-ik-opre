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
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t child;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           child = fork();	// creating parent-child processes
           if (child == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (child>0)
	   {		    	// child process

			   	   
               printf("Szulo indul!\n");
			   pause();
               write(pipefd[1], "Hajra Fradi!",13);
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
			   			   printf("Szulo elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Szulo olvasta uzenet: %s",sz);
               close(pipefd[0]); //Usually we close unused read end
               fflush(NULL); 	// flushes all write buffers (not necessary)
              //wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
           } 
           else 
           { 
	       //sleep(3);	// sleeping a few seconds, not necessary
			 kill(getppid(),SIGCONT); 
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",sz);
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
			   
			   	write(pipefd[1], "Hajra Frodo!",13);
               close(pipefd[1]); // Closing write descriptor 
			   printf("Gyerek beirta az adatokat a csobe!\n");
			  
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
