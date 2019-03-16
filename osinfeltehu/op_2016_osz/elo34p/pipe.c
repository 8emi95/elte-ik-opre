       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1) //ha nem jó visszaad -1-et
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	// creating parent-child processes - gyerekfolyamat
           if (pid == -1) //gyerek folyamat létrejöttének ellenőrzése
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
			   write()
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
			//sleep(3);
               printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Hajra Fradi!",13); //írni a csőbe (melyik csőbe, mit akarunk írni, hossz)
			   read(pipefd[0],sz,sizeof(sz));
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary) //mindenképpen kiír mindent a képernyőre!!!
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }
//íróvég 1, olvasóvég 0 !!! ha írni akarok lezárjuk az olvasóvéget, ha olvasni akkor lezárjuk az íróvéget
//csővezetékkel tudok szinkronizálni -> nem kell bele sleep()-et írni, mert attól csak lelassul
//válaszoljunk a csővezetéken keresztül, gyerek válaszoljon a szülő üzenetére
