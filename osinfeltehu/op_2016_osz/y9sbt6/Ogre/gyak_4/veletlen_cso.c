       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
       #include <time.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd[2]; // unnamed pipe file descriptor array, read:[0], write:[1]
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
	       int pipe_data;      
	       sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
	       
	       while(read(pipefd[0], &pipe_data, sizeof(pipe_data)) )
	       {
                 printf("Gyerek olvasta uzenet: %i",pipe_data);
	         printf("\n");
	       }  
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               srand(time(NULL));
               int data_num = rand() / 10 + 1;
               
               close(pipefd[0]); //Usually we close unused read end
               //write(pipefd[1], "Hajra Fradi!",13);
               int i;
               for(i = 0; i < data_num; ++i)
               {
                 write(pipefd[1], &i, sizeof(int));
               }
               
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
