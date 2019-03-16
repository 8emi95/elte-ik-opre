       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <signal.h>
	//
	// unnamed pipe example
	//
	
	void handler(int sign){
		szamlalo++;
	}
	
	
       int main(int argc, char *argv[])
       {   int szamlalo =0;
		   int randm; 
		   int ciklusSzam;
		   signal(SIGTERM, handler);
           int pipefd[2]; // unnamed pipe file descriptor array 2 elem, (két vég?)
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1)  //létrehozás
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	// creating parent-child processes mindig fork elott!!
           if (pid == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	   {		    	// child process
	       sleep(3);	// sleeping a few seconds, not necessary
             //  close(pipefd[1]);  //Usually we close the unused write end
	
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
            int x;
		while(read(pipefd[0],x,sizeof(x))) // reading max 100 chars
               
			   printf("Gyerek olvasta uzenet: %s",sz);
			  //  write(pipefd[1], "Hajra Fradi!",13);		
			//kill(getpid(), SIGTERM);//
			printf("\n");
               close(pipefd[0]); // finally we close the used read end
           
		   } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
          	   randm = rand()%10;
			   int y;
			for (ciklusSzam=0; ciklusSzam<randm; ciklusSzam++){
		    	y= rand()%10;
		     write(pipefd[1],&y,sizeof(int));
		   
		   
	   } 
		  
		
//		while (szamlalo==0){sleep(1);}	// pause();   helyett
//			   read(pipefd[0],sz,sizeof(sz)); 
            //
		//	printf("A valasz:%s\n",sz);   
			   close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
