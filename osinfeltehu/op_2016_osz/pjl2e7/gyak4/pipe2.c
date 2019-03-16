       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd1[2]; // szulo irja, gyerek olvassa
	   int pipefd2[2]; //gyerek irja, szulo olvassa
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd1) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
	   if (pipe(pipefd2) == -1) 
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
                //close(pipefd[1]);  //Usually we close the unused write end
                printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
                close(pipefd1[1]);
                read(pipefd1[0],sz,sizeof(sz)); // reading max 100 chars
                close(pipefd1[0]);
                printf("Gyerek olvasta uzenet: %s",sz);
                printf("\n");
                close(pipefd2[0]);
                write(pipefd2[1], "Hajra Vasas!", 13);
                close(pipefd2[1]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               //sleep(3);
		       printf("Szulo indul!\n");
               close(pipefd1[0]); //Usually we close unused read end
               write(pipefd1[1], "Hajra Fradi!",13);
               close(pipefd1[1]);
	           close(pipefd2[1]);
               read(pipefd2[0], sz, sizeof(sz));
               close(pipefd2[0]);
		       printf("gyerek valasza %s\n", sz);//fflush(stdout);
		//close(pipefd[1]); // Closing write descriptor 
               //printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	           printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
