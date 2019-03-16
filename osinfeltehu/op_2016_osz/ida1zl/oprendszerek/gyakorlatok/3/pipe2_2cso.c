       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd1[2]; // unnamed pipe file descriptor array, író-olvasó vég
           int pipefd2[2]; // unnamed pipe file descriptor array, író-olvasó vég
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd1) == -1) //generálja a csővezetéket
			{
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
            }
		   if (pipe(pipefd2) == -1) //generálja a csővezetéket
			{
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
            }
		   
           pid = fork();	// creating parent-child processes
           if (pid == -1)  //létrejött-e
			{
               perror("Fork hiba");
               exit(EXIT_FAILURE);
            }

           if (pid == 0) 
		   {		    	// child process
	       //sleep(3);	// sleeping a few seconds, not necessary
               //close(pipefd[1]);  //Usually we close the unused write end, ha olvasunk legyen lezárva az író vége
			   printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd1[0],sz,sizeof(sz)); // reading max 100 chars, olvasó vég mindig 0
               printf("Gyerek olvasta uzenet: %s",sz);
			   printf("\n");
			   write(pipefd2[1], "Hajra vasas!", 13); // nem ez lesz a válasza a gyereknek, az ütemezőtől függ meddig engedi futni a kódot, vagy 1 újabb csővezeték, vagy szignálok küldése
               close(pipefd2[0]); // finally we close the used read end, végén mindent lezárunk
           } 
           else 
           {    // szulo process 
			  //sleep(3); //ide tesszük a sleepet
               printf("Szulo indul!\n");
               //close(pipefd[0]); //Usually we close unused read end, ha írunk legyen lezárva az olvasó
               write(pipefd1[1], "Hajra Fradi!",13); //csővezetékbe írás, 1.:melyik végbe akarunk írni, 2.:mit, 3.:hossza, az író vég mindig 1
               read(pipefd2[0], sz, sizeof(sz));
			   printf("gyerek valasza %s\n", sz); //fflush(stdout)
			   close(pipefd1[1]); // Closing write descriptor 
               //printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
			}
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
