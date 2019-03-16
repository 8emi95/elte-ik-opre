       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>
	//
	// unnamed pipe example
	//
	void signal_handler(int sing_number) {
	printf("\nSignal with number %i has arrived\n", sing_number);
}
       int main(int argc, char *argv[])
       {
		   signal(SIGTERM, signal_handler);
           int pipefd[2]; // unnamed pipe file descriptor array
		   int pipefd2[2];
           pid_t pid;
           char sz[100];  // char array for reading from pipe
	   int a = 2;
	   int b = 3;
           char y[500];
		   char h[500];
		   int z = 0;

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
	       sleep(1);	// sleeping a few seconds, not necessary
               //close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],&y,sizeof(y)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",y);
			   sleep(2);
			   strcpy(y,"Itt vagyok");
			   write(pipefd[1], &y,sizeof(y));
			   //kill(getppid(), SIGTERM);
			   //pause();
			   sleep(2);
			   
			   strcpy(y,"");
			   read(pipefd[0],&y,sizeof(y));
               printf("Gyerek olvasta uzenet: %s",y);
			   int randomka = rand()%100;
			   write(pipefd[1], &randomka,sizeof(randomka));
			   //kill(getppid(), SIGTERM);
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
			   close(pipefd[1]);
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               //close(pipefd[0]); //Usually we close unused read end
               //write(pipefd[1], "Hajra Fradi!\n",13);
	       //a = a + b;
	       write(pipefd[1], "Budapest",sizeof("Budapest"));
               //close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
			   sleep(5);
			   //pause();
			   read(pipefd[0],&h,sizeof(h));
				printf("ez jött: %s\n",h);
				write(pipefd[1], "Ukk",sizeof("Ukk"));
				fflush(NULL);
				//kill(0, SIGTERM);
				sleep(5);
				//pause();
				read(pipefd[0],&z,sizeof(z));
				printf("ez jött: %d\n",z);
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
