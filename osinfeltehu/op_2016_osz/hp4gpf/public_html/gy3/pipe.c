       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd[2]; // unnamed pipe file descriptor array (egyik a cső eleje, a másik a vége) 1 író vég, 0 az olvasó vég
           int pipedf[2];
           pid_t pid;
           char sz[100];  // char array for reading from pipe
            //Ha valami hiba van akkor -1 
           if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           if (pipe(pipedf) == -1) 
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
	       //sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
              printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
              write(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
              printf("Gyerek olvasta uzenet: %s",sz);
	       printf("\n");
         write(pipedf[1], "Hajra Vasas!",14);
         //close(pipedf[1]);
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
         sleep(3); //ha itt alszik, akkor a gyerek próbál belőle olvasni.
               printf("Szulo indul!\n");
               //close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Hajra Fradi!",13);
               read(pipedf[0], sz, sizeof(sz));
               printf("Ez a valasz %s\n", sz);
               //close(pipedf[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
