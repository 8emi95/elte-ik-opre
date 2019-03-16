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

void createPipeFortask(int * pipeTask) {
	if (pipe(pipeTask) == -1) {
        perror("\nUnable to create pipe for the task!\n");
    }
}

void createPipeForStatus(int * pipeTask) {
	if (pipe(pipeTask) == -1) {
        perror("\nUnable to create pipe for the task!\n");
    }
}

void sendTaskToThePlayer(char * processOwner, const int * pipeTask, const char * task, const char * player) {
	printf("%s: Sending task to player %s:\n", processOwner, player);
	close(pipeTask[0]);
	write(pipeTask[1],task,strlen(task));
	close(pipeTask[1]);
}

void sendStatus(char * processOwner, const int * pipeStatus, const char * status) {
	printf("%s: Sending status to trainer\n", processOwner);
	close(pipeStatus[0]);
	write(pipeStatus[1],status,strlen(status));
	close(pipeStatus[1]);
}

int main(int argc, char *argv[]) {
	system("clear");
	char s[1024];
	char* player = argv[1];
	printf("Player name: %s\n", player);
	char * task = "Escape from the room}Escape}42";
	
	int pipeTask[2];
	int pipeStatus[2];
    char charsFromPipe[100];
	char charsFromStatusPipe[100];
	
	pid_t childProcessId;
	signal(SIGTERM,handler);
	createPipeFortask(pipeTask);

	childProcessId = fork();
	char* processOwner;
	
	if(childProcessId > 0 )	{
		processOwner = "TRAINER";
		
		printf("%s: Please enter the room!\n", processOwner);
		sendTaskToThePlayer(processOwner, pipeTask, task, player);
		pause();
		
		close(pipeStatus[1]);
		sleep(1);
		read(pipeStatus[0],charsFromStatusPipe,sizeof(charsFromStatusPipe));
		printf("%s: Status received: %s\n", processOwner, charsFromStatusPipe);
		
		kill(childProcessId,SIGTERM);
		
		int status;
		wait(&status);
	} else {
		processOwner = "PLAYER";
		
		close(pipeTask[1]);
		sleep(1);
		read(pipeTask[0],charsFromPipe,sizeof(charsFromPipe));
		
		char * task = strtok(charsFromPipe, "}");
		char * taskName = strtok(NULL, "}");
		char * taskId = strtok(NULL, "}");
		
		printf("%s: Task received!\n", processOwner);
		printf("%s: Task: %s\n", processOwner, task);
		printf("%s: Task name: %s\n", processOwner, taskName);
		printf("%s: Task id: %s\n", processOwner, taskId);
		sleep(1);
		
		int n = rand() % 100 + 1;
		char * status;
		//printf("%s: Random number: %d\n", processOwner, n);
		if(n > 50) {
			printf("%s: Escaping succesfull\n", processOwner);
			status = "success";
		} else {
			printf("%s: Escaping failed\n", processOwner);
			status = "failed";
		}
		
		sendStatus(processOwner, pipeStatus, status);
		
			
		kill(getppid(),SIGTERM);
	}
	
	return 0;
}