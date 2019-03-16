       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[]) {
           int pipefd[2]; // unnamed pipe file descriptor array
           // 0 - olvas, 1 - ír
           pid_t pid;
           char sz[100];  // char array for reading from pipe
           // pipe fv hívásnak a fork előtt kell lenni

           if (pipe(pipefd) == -1) {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }

           pid = fork();	// creating parent-child processes
           if (pid == -1) {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0)
           {		    	// child process
    	       sleep(3);	// sleeping a few seconds, not necessary, mert magától is addig vár, amíg adat nem érkezik, és nincs lezárva a másik a csővezetéknek; ezért akár szinkronizációra is lehet használni. Nem csak első írásnál, mindig.
               close(pipefd[1]);  //Usually we close the unused write end

    	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0], sz, sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s", sz);
    	       printf("\n");

               close(pipefd[0]); // finally we close the used read end
           }
           else
           {    // szulo process 
               printf("Szulo indul!\n");
               close(pipefd[0]); // Usually we close unused read end
               write(pipefd[1], "Hajra Fradi!", 13); // Fontos, hogy 13, méretnél egyel több, különben nem írja bele a végén a nullát, és nem lesz vége.
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary) // akkor kell, ha azt akarom, hogy mindenképp mindent kiírjon, ami az ideiglenes tárolóban van.
               wait();		// waiting for child process (not necessary)
				// try it without wait()
               printf("Szulo befejezte!");
           }

    	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
