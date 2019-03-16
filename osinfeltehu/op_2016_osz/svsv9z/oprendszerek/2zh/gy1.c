#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <time.h>

#include<sys/sem.h>
#include<semaphore.h>

#include <sys/wait.h> //waitpids  
#include <string.h>
#include <errno.h>

#include <sys/ipc.h> 
#include <sys/msg.h> 

#include <mqueue.h>

void sighandler(int signum) {
	printf("Megerkezett!\n");
}

void sighandlerForChild(int signum) {
	printf("Megkapva Child!\n");
}

void endhandler(int signum) {
	printf("Vege!\n");
}

int main( int argc, const char* argv[] )
{
	srand(time(NULL));

	pid_t child1;
	pid_t child2;

	printf("MUKODIK\n");
	
	child1 = fork();

	int fd[2];

	pipe(fd);

	


	if (child1 == 0) {
		 /* Child 1 code */
		int d;

		 printf("CHILD1\n");
		sleep(1);

		kill(getppid(), SIGUSR1);




	}
	else {
		child2 = fork();

		if (child2 == 0) {
			sleep(0.5f);
			printf("CHILD2\n");
			kill(getppid(), SIGUSR1);
			sleep(1);
			}
	
		else {
	 		/* Parent Code */
			 int visNum;
			 sscanf(argv[1], "%i", &visNum);

			signal(SIGUSR1, sighandler);
			
			pause();
			pause();

			printf("Folytatodik!\n");

			pause();
		}
	}	
	
	
}












