#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>
//
// unnamed pipe example
//

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char *argv[])
{
   int pipefd[2]; // unnamed pipe file descriptor array
   pid_t pid;
   char sz[100];  // char array for reading from pipe

   signal(SIGTERM,handler);
   
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
	   pause();	   
   printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
	   read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
	   printf("Gyerek olvasta uzenet: %s",sz);
   printf("\n");
		write(pipefd[1],"asd",3);
	   //close(pipefd[0]); // finally we close the used read end
   } 
   else 
   {    // szulo process 
	   printf("Szulo indul!\n");
	   //close(pipefd[0]); //Usually we close unused read end
	   write(pipefd[1], "Hajra Fradi!",13);
	   printf("Szulo beirta az adatokat a csobe!\n");
	   kill(pid,SIGTERM);
	   read(pipefd[0], sz, sizeof(sz));
	   printf("gyerek valasza: %s \n", sz);
	   //close(pipefd[1]); // Closing write descriptor 
	   fflush(NULL); 	// flushes all write buffers (not necessary)
	   wait();		// waiting for child process (not necessary)
		// try it without wait()
   printf("Szulo befejezte!");	
}
exit(EXIT_SUCCESS);	// force exit, not necessary
}