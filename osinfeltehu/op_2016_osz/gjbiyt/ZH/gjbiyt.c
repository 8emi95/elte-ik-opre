#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

void handler(int signumber){
  printf("Signal érkezett\n");
}

const char* parentPipe = "parentToChild";
const char* childPipe = "childToParent";

int main(int argc,char * argv[]) {

	if (argc < 2){
		printf("Parancssori argumentumként adja meg a versenyző nevét!\n");
		exit(0);
	}
	
	int childToParent = mkfifo(childPipe, S_IRUSR | S_IWUSR);
	int parentToChild = mkfifo(parentPipe, S_IRUSR | S_IWUSR);
	
	if (childToParent == -1) {
		printf("Error number(childToParent): %i", errno);
		exit(EXIT_FAILURE);
	}
	
	if (parentToChild == -1) {
		printf("Error number(parentToChild): %i", errno);
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));
	
	
	char name[1024];
	strcpy(name, argv[1]);
	
	char id[2] = "0";
	char task[2] = "5";
	
	struct sigaction sigact;
	sigact.sa_handler=handler; 
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags=0;
	sigaction(SIGTERM,&sigact,NULL);
	
	int pid, pTC, cTP;
	int pipefd[2]; // névtelen cső
	char msg[1024];
	
	
	
	
	//így születik a gyerek:
	pid = fork();
	
	//actual work starts ---------------------------------------------------------------------------
	if (pid > 0) //parent 
	{		
		/*--------------------------
		printf("parent: Task = %s\n", task);
		printf("parent: name = %s\n", name);
		printf("parent: id = %s\n", id);
		
		
		
		*/
		
		pTC = open(parentPipe, O_WRONLY);
		cTP = open(childPipe, O_RDONLY);

		strcpy(msg, task);
		write(pTC, msg, sizeof(msg));
		
		strcpy(msg, name);
		write(pTC, msg, sizeof(msg));
		
		strcpy(msg, id);
		write(pTC, msg, sizeof(msg));
		
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset,SIGTERM);
		
		int status;
		waitpid(pid,&status,0);
		
		read(cTP, msg, sizeof(msg));
		printf("parent:%s\n", msg);
		
		unlink(parentPipe);
		unlink(childPipe);
		
		exit(0);
	} 
	else // child
	{
		char rcv[1024];
		char myTask[2];
		char myName[1024];
		char myId[2];
		
		pTC = open(parentPipe, O_RDONLY);
		cTP = open(childPipe, O_WRONLY);
		
		read(pTC, rcv, sizeof(rcv));
		strcpy(myTask, rcv);
		
		read(pTC, rcv, sizeof(rcv));
		strcpy(myName, rcv);
		
		read(pTC, rcv, sizeof(rcv));
		strcpy(myId, rcv);
		
		printf("Nevem: %s, Sorszámom: %s\n", myName, myId);
		//printf("Sorszámom: %s\n", myId);
		printf("Feladatom: %s\n", myTask);
		
		int random = rand() % 2;
		
		if (random){
			strcpy(msg, "Megoldottam a feladatomat!\n");
		}
		else{
			strcpy(msg, "Sajnos nem sikerült megoldani\n");
		}


		printf("%s", msg);
		write(cTP, msg, sizeof(msg));
		kill(getppid(), SIGTERM);
		
		exit(0);
	}
	
	//actual word ends -----------------------------------------------------------------------------
	return 0;
}
