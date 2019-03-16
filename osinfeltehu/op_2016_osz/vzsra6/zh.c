#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>
#include <unistd.h> //pipe
#include <sys/msg.h> 
#include <signal.h>    


void handler(int signumber){
    
}


int main(int argc, char *argv[]) {
	//************* ArgC Check *************
	if (argc != 2) {
		printf("Egy parancssori argumentumot kerek.\n");
		exit(1);
	}
	const char* childName = argv[1];
	
	
	//************* Pipe *************
	int pipefd1[2];
	int pipefd2[2];
    char sz[1024] = {'\0'};
	if (pipe (pipefd1) == -1) {	
		perror("Nem skerult a pipeot megnyitni.\n");
		exit(2);
    }
	if (pipe (pipefd2) == -1) {	
		perror("Nem skerult a pipeot megnyitni.\n");
		exit(2);
    }
	
	//********
	struct sigaction sigact;
    sigact.sa_handler=handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(SIGUSR1,&sigact,NULL);


	//************* Multifork *************
	
	pid_t pid = fork();	// creating parent-child processes
	if (pid == -1) {
	   perror("Fork hiba");
	   exit(EXIT_FAILURE);
	}


	if (pid == 0) 	// child process
	{		    	
		sleep (3);	// sleeping a few seconds, not necessary
		close (pipefd1[1]);  //Usually we close the unused write end
		read (pipefd1[0], sz, sizeof(sz)); // reading max 1024 chars
		printf ("Gyerek olvasta uzenet: %s",sz);
		printf ("\n");
		close (pipefd1[0]); // finally we close the used read end
		
		srand(time(NULL));
		int shot = (rand() % 2);
		
		char message[1024] = {'\0'};
		
		close(pipefd2[0]); //Usually we close unused read end
		if (shot == 0) {
			printf ("Nem sikertült a feladatot megoldani.\n");
			strcpy (message, "Nem sikertült a feladatot megoldani.\n");
			write (pipefd2[1], message, 1024);
		} else {
			printf ("Sikertült a feladatot megoldani.\n");
			strcpy (message, "Sikerult a feladatot megoldani.\n");
			write (pipefd2[1], message, 1024);
		}
		close (pipefd2[1]); // Closing write descriptor 
		fflush(NULL); 	// flushes all write buffers (not necessary)
		
		kill(getppid(), SIGUSR1);
		exit(0);
   } 
   else 	// szulo process 
   {    
		printf("Szulo indul!\n");
		close(pipefd1[0]); //Usually we close unused read end
		char message[1024] = {'\0'};
		strcpy (message, "Szabadulos Jatek feladat:  ");
		strcat (message, childName);
		strcat (message, " ");
		strcat (message, "0");
		write (pipefd1[1], message, 1024);
		close (pipefd1[1]); // Closing write descriptor 
		fflush(NULL); 	// flushes all write buffers (not necessary)
		wait(NULL);		// waiting for child process (not necessary)
		
		sigset_t sigset;                    
        sigfillset(&sigset);                
        sigdelset(&sigset, SIGUSR1);            
        sigsuspend(&sigset);
	
		close (pipefd2[1]);  //Usually we close the unused write end
		read (pipefd2[0], sz, sizeof(sz)); // reading max 1024 chars
		printf("Eredmeny: %s", sz);	
		printf("Szulo befejezte!");	
		close (pipefd2[0]); // finally we close the used read end
		exit (0);
;

	}




}


