       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//névtelen csővezeték csak a fájlból érhető el nevesíteni kell, és akkor kívülről is elérhető
	//
       int main(int argc, char *argv[])
       {
           int pipefd_2[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
		   if (pipe(pipefd_2) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	// creating parent-child processes
           if (pid == -1) 	//ha nem sikerült gyerekfolyamatot létrehozni
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	   {		    	// child process
	       sleep(3);	// sleeping a few seconds, not necessary
		   //vár hogy a szülő küldhessen valamit, de nem kell ide!!!!! mert csak akkor megy tovább ha valami jön a vezetékből.
             //  close(pipefd[1]);  //Usually we close the unused write end
			   //illik lezárni a csővezeték végét ha nem akarom használni (pl csak írásra akarom használni)
			   // 1-es az író vég a 0 az olvasó vég
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",sz);
	       printf("\n");
		   write(pipefd[1],"hajra vasas!", 14);
               close(pipefd[0]); // finally we close the used read end
			   close(pipefd[1]);
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
              // close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Hajra Fradi!",13);
			   //ír a csővezetékre
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
			    read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
				printf("ez a valasz jott: %s", sz);
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
			   //vár a gyerek befejeződésére
				// try it without wait()
	       printf("Szulo befejezte!");	
		   close(pipefd[1]);
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
