#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h> //waitpid
#include <unistd.h> //includes fork, pipe()
#include <fcntl.h> // manipulate file descriptor
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // for errno, the number of last error
#include <string.h>
#include <signal.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char *argv[])
{
   int pipefd[2]; // unnamed pipe file descriptor array
   int eredmenyfd[2]; // unnamed pipe file descriptor array
   pid_t pid;
   char sz[100];  // char array for reading from pipe
    signal(SIGUSR1,handler);


   if (pipe(pipefd) == -1) 
{
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
   }
   
     if (pipe(eredmenyfd) == -1) 
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
   sleep(3);	// sleeping a few seconds, not necessary
	   close(pipefd[1]);  //Usually we close the unused write end
   printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
	  // read(pipefd[0],sz,100); // reading max 100 chars
	  
	   while(read(pipefd[0], sz, sizeof(sz))) {
           printf("Gyerek olvasta: %s\n", sz);
    }
	  
	 //  printf("Gyerek olvasta uzenet: %s",sz);
   printf("\n");
	   close(pipefd[0]); // finally we close the used read end
	   sleep(3);
	   
	   int i=rand()%100;
	    char dontes[100];
	   if(i>50){
		    strcpy(dontes,"sikerult megoldani a feladatot"); 
	   }else{
		  strcpy(dontes,"NEM sikerult megoldania feladatot");  
	   }
	   printf("Gyerek: %s\n", dontes);
	   
	    close(eredmenyfd[0]); //Usually we close unused read end
	    write(eredmenyfd[1], dontes,100);
	    close(eredmenyfd[1]); // Closing write descriptor 
		
		 kill(getppid(),SIGUSR1);

	   exit(0);
   } 
   else 
   {    // szulo process 
	   printf("Szulo indul!\n");
	   
	   close(pipefd[0]); //Usually we close unused read end
	    write(pipefd[1], "feladat",100);
	   write(pipefd[1], argv[1],100);//write(pipefd[1], "Hajra Fradi!",13);
	     write(pipefd[1], "sorszam",100);
	   close(pipefd[1]); // Closing write descriptor 
	   
	   printf("Szulo beirta az adatokat a csobe!\n");
	   fflush(NULL); 	// flushes all write buffers (not necessary)
	   pause(); //waits till a signal arrive
	   
	    char e[100];  
	   close(eredmenyfd[1]);
	   read(eredmenyfd[0], e, sizeof(e));
	    close(eredmenyfd[0]);
		 printf("Szulo olvasta: %s\n", e);
	   
	   wait(NULL);		// waiting for child process (not necessary)
		// try it without wait()
   printf("Szulo befejezte!");	
}
exit(EXIT_SUCCESS);	// force exit, not necessary
}


