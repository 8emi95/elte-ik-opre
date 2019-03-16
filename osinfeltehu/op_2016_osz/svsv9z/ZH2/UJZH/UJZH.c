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
	pid_t child_a, child_b;

	child_a = fork();

	if (child_a == 0) {
		/* Child A code */
		printf("CHILD1");
		sleep(1);
		kill(getppid(), SIGUSR1);

	} else {
		child_b = fork();

		if (child_b == 0) {
			/* Child B code */
			printf("CHILD2");
			sleep(0.5f);
			kill(getppid(), SIGUSR1);
			

		} else {
			/* Parent Code */
			printf("PARENT");

			signal(SIGUSR1, sighandler);
			pause();
			pause();

			printf("FUT TOVABB");

			int numOfVis;
			sscanf(argv[1], "%i", &numOfVis);
			printf("\nLatogatok szama: %i", numOfVis );
		}
	}
	
}









//signal(SIGUSR1, sighandler);
//kill(getppid(), SIGUSR1);
