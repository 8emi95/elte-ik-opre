       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd[3]; // unnamed pipe file descriptor array
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
	       sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
			   close(pipefd[2]);
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
			   read(pipefd[1], sz, sizeof(sz));
               printf("Gyerek olvasta uzenet: %s",sz);
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Hajra Fradi!",13);
			   write(pipefd[2], "Hajra Magyarország!", 50);
               close(pipefd[1]); // Closing write descriptor 
			   close(pipefd[2]);
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 