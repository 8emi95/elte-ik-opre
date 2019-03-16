       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	#include <poll.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
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
	       //sleep(3);	// sleeping a few seconds, not necessary
               //close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,22); // reading max 19 chars
               printf("Gyerek olvasta uzenet: %s\n",sz);
		read(pipefd[0],sz,sizeof(sz)); // felulirjuk sz-t, nullakarakter?
                write(pipefd[1],"Koszonom!",10);
		printf("Gyerek olvasta uzenet: %s\n",sz);
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
               //close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Hajra Fradi! alma",21);
	       write(pipefd[1], " korte", 7);
               close(pipefd[1]); // Closing write descriptor 
		printf("Szulo beirta az adatokat a csobe!\n");
	sleep(1);
		struct pollfd poll_fds[5];
                poll_fds[0].fd=pipefd[0];
		poll_fds[0].events=POLLIN;
		poll(poll_fds,1,5000);
		char s[50];
		if (poll_fds[0].revents&POLLIN)
		{
			read(pipefd[0],s,sizeof(s));
			printf("A gyerek válasza: %s\n",s);
		}
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
