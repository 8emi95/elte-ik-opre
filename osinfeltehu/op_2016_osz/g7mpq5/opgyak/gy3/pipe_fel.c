       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>

       #include <signal.h>

       void handler(int signumber) {
	//TODO itt vmi valtozo allitas
        //TODO vagy ket csovel
       }
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           signal(SIGTERM,handler);

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
	       sleep(3);	// sleeping a few seconds, not necessary
               //close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               kill(getppid(),SIGTERM);
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",sz);
	       printf("\n");
               write(pipefd[1], "Hajra Vasas!",13);
               close(pipefd[0]); // finally we close the used read end
               close(pipefd[1]);  //Usually we close the unused write end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               //close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Hajra Fradi!",13);
               pause();
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Ez a valasz: %s\n", sz);
               close(pipefd[1]); // Closing write descriptor 
               close(pipefd[0]); //Usually we close unused read end
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
