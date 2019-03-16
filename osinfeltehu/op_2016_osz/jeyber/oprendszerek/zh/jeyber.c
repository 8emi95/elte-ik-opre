#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void handler(int signumber) {
  signal(SIGTERM,SIG_DFL);
}

void createPipeForTask(int* pipeTask, char* player) {
	if (pipe(pipeTask) == -1) {
        perror("Unable to create pipe for the task!\n");
    }
}

void sendTaskToThePlayer(char * processOwner, char* player, const int *pipeTask, const char* task) {
	printf("%s: Send task to player: %s\n", processOwner, player);
	close(pipeTask[0]);
	write(pipeTask[1],task,strlen(task));
	close(pipeTask[1]);
}

int main(int argc, char *argv[] ) {
	char* processOwner;
	char* player = argv[1];
	char* task = "Ez lesz itt a feladatod, jo?";
	int pipeTask[2];
	char charsFromPipe[100];
	const char * pipeFileName = "pipe.txt";
	
	printf("Player name: %s\n", player);
	
	pid_t childProcessId = fork();
	signal(SIGTERM,handler);
	mkfifo(pipeFileName, S_IRUSR | S_IWUSR );
	
	system("clear");
	
	createPipeForTask(pipeTask, player);
	
	if(childProcessId > 0 )	{
		processOwner = "TRAINER";
		
		printf("%s: Please enter the room!\n", processOwner);
		
		sendTaskToThePlayer(processOwner, player, pipeTask, task);
		pause();
		
		sleep(1);
		kill(childProcessId,SIGTERM);
		
		int status;
		wait(&status);
	} else {
		processOwner = "PLAYER";
		
		close(pipeTask[1]);
		sleep(1);
		read(pipeTask[0],charsFromPipe,sizeof(charsFromPipe));
		printf("%s: Task received: %s\n", processOwner, charsFromPipe);	
		sleep(1);
		kill(getppid(),SIGTERM);
		pause();
	}
	
	return 0;
}