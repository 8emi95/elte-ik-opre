       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <signal.h>
	#include <sys/types.h>
	//
	// unnamed pipe example
	//
		void handler(int signumber){};
	
	
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
	      	// sleeping a few seconds, not necessary
               //close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",sz);
	       printf("\n");
			   close(pipefd[0]); // finally we close the used read end
			   
			   write(pipefd[1],"Hajra Vasas!",13);
			   close(pipefd[1]);
			   
			   printf("Gyerek beirta az adatokat a csobe!\n");
			   kill(getppid(),SIGTERM);
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
			   
               write(pipefd[1], "Hajra Fradi!",13);
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
			   pause();//while(szamlalo ==0){uspeep(1)};
			   //wait();
			   printf("Szulo elkezdi olvasni a csobol az adatokat!\n");
			   read(pipefd[0],sz,sizeof(sz));
			   printf("Szülő olvasta uzenet: %s\n",sz);
			   close(pipefd[0]);
			    	// flushes all write buffers (not necessary)
               		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
