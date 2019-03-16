#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h> // for pipe()

void handler(int signumber){}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	signal(SIGUSR1,handler);
   
   int pipefd[2]; // unnamed pipe file descriptor array
   pid_t pid;
   char sz[100];  // char array for reading from pipe

   if (pipe(pipefd) == -1){
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
   }
   pid = fork();	// creating parent-child processes
   if (pid == -1) {
	   perror("Fork hiba");
	   exit(EXIT_FAILURE);
   }

   if (pid == 0) {		    	// child process
	    sleep(1);
		int i;
		int max;

		do{
			
			read(pipefd[0], &i,sizeof(i));
			read(pipefd[0], &max,sizeof(max));
			
			int r = rand()%100;
			int beert = 0;
			if(r < 5){
				write(pipefd[1], "Elcsuszott egy bananhejon", 26);
			}else if (r < 10){
				write(pipefd[1], "Lefagyott a szerelveny.", 24);
			}else{
				write(pipefd[1], "Sikeresen beert az allomasra", 29);
				beert = 1;
			}

			printf("Szerelveny szama: %i\n", i+1);
			printf("Hatralevo szerelvenyek: %i\n", max-(i+1));
			if(beert == 1){
				printf("Teljesitettem az uzemet.\n");
			}else{
				printf("Nem sikerult teljesiteni az uzemet\n");
			}
			
			kill(getppid(),SIGUSR1);
			pause();
		}while(i < max);
		
	    close(pipefd[0]);
		close(pipefd[1]);
   }
   else {    // szulo process 
	   
	   //argv[1] - maxszám
	   int i;
	   int max = atoi(argv[1]);
	   for(i=0; i < max; i++){
			write(pipefd[1], &i, sizeof(i));
			write(pipefd[1], &max, sizeof(max));
			kill(pid,SIGUSR1);
			pause();
			read(pipefd[0], sz,sizeof(sz));
			printf("Allomas uzenete: %s\n", sz);
	   }
	   
	   close(pipefd[0]);
	   close(pipefd[1]);
	   fflush(NULL);
	}
	
	exit(EXIT_SUCCESS);
}


