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

void handler(int signumber);

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
	
	struct sigaction sig1;
	//sig1.sa_handler = handler; //ez a handler fog lefutni
	
	sigemptyset(&sig1.sa_mask);
	sig1.sa_flags = 0; 
	sigaction(SIGUSR1,&sig1,NULL);
	
	int pid, pTC, cTP;
	int pipefd[2]; // névtelen cső
	char msg[1024];
	
	
	
	
	//így születik a gyerek:
	pid = fork();
	
	//actual work starts ---------------------------------------------------------------------------
	if (pid > 0) //parent 
	{		
		//--------------------------
		printf("parent: Task = %s\n", task);
		printf("parent: name = %s\n", name);
		printf("parent: id = %s\n", id);
		
		
		
		//--------------------------
		strcpy(msg, task);
		write(pTC, msg, sizeof(msg));
		
		strcpy(msg, name);
		write(pipefd[1], msg, sizeof(msg));
		
		strcpy(msg, id);
		write(pipefd[1], msg, sizeof(msg));
		
		pause();
		read(pipefd[0], msg, sizeof(msg));
		
		exit(0);
	} 
	else // child
	{
		char myTask[2];
		char myName[1024];
		char myId[2];
		
		read(pipefd[0], myTask, sizeof(myTask));
		read(pipefd[0], myName, sizeof(myName));
		read(pipefd[0], myId, sizeof(myId));
		
		printf("Nevem: %s\n", myName);
		printf("Sorszámom: %s\n", myId);
		printf("Feladatom: %s\n", myTask);
		
		int random = rand() % 2;
		
		if (random){
			strcpy(msg, "Megoldottam a feladatomat!\n");
		}
		else{
			strcpy(msg, "Sajnos nem sikerült megoldani\n");
		}


		printf("%s", msg);
		write(pipefd[1], msg, sizeof(msg));
		kill(getppid(), SIGUSR1);
		
		exit(0);
	}
	
	//actual word ends -----------------------------------------------------------------------------
	return 0;
}
