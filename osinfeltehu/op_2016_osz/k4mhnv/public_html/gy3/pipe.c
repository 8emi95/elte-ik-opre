       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <sys/time.h>
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {

            srand(time(NULL)); //the starting value of random number generation
            int r[5];
            int j = 0;
             while(j < 4){
                r[j] = rand()%100; //number between 0-99
                j++;
             }
             
           int pipefd[2]; // unnamed pipe file descriptor array
           int pipefdalt[2];
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
	       //sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",sz);
               write(pipefd[1], "Hajra Vasas!", 13);
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
               int k = 0;
               while(k < 4){
                write(pipefd[1], "uzenet" , 7);
                k++;
               }
               read(pipefd[0], sz, sizeof(sz));
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
