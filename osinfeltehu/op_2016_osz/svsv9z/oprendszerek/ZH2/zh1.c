#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h>
#include <string.h>
#include <errno.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>

void sighandler(int signum) {
	printf("Megerkezett!\n", signum);
}

void sighandlerForChild(int signum) {
	printf("Megkaptam a neveket!\n", signum);
}


void waitingForVisitors(int signum) {
}

void visitorsArrived(int signum) {
	printf("Megerkeztek a vendegek!\n");
}

int main( int argc, const char* argv[] )
{
	pid_t child_a, child_b;

	child_a = fork();

	if (child_a == 0) {
		/* Child A code */
		printf("CHILD_A READY!");
		kill(getppid(), SIGUSR1);

	} else {
		child_b = fork();

		if (child_b == 0) {
			/* Child B code */
			printf("CHILD_B READY!");
		} else {
			/* Parent Code */

			printf("PARENT READY!");

						
			signal(SIGUSR1, sighandler);
			pause();

		}
	}
}
