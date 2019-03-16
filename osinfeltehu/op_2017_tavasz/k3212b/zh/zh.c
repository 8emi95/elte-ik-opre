       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	
       int main(int argc, char *argv[])
       {
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[sizeof argv[1]];  // char array for reading from pipe
			if(argc == 1){
				perror("Üres argumentum!");
               exit(-1);
			}
           if (pipe(pipefd) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
               exit(-1);
			}
           pid = fork();	// creating parent-child processes
           if (pid == -1) 
			{
               perror("Fork hiba");
               exit(-1);
			}
		//utas
        if (pid == 0) 
			{
               close(pipefd[0]); //Usually we close unused read end
			   printf("Utas küldi a cimet! %s \n", argv[1]);
               write(pipefd[1], argv[1],sizeof argv[1]);
               close(pipefd[1]); // Closing write descriptor 
               printf("Az utas elmondta a cimét!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Központ vége!");	
			} 
		else //központ
			{    // szulo process 
            close(pipefd[1]);  			
            read(pipefd[0],sz,sizeof(sz)); 
            printf("Központ megkapta a cimet! %s",sz);
	        printf("\n");
            close(pipefd[0]); // finally we close the used read end
			
               
	   }
	   exit(1);	// force exit, not necessary
       }

 
