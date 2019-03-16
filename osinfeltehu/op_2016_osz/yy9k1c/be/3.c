#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <time.h>

#define MAXMSG 1024
#define SIGARRIVED SIGRTMIN+1

pid_t childpid;
int msgQueue;

typedef struct Messages {
	long messageType;
	char messageText[MAXMSG];
} Message;

void sendMessage(int queue, char *text, int type) {
	Message message;
	message.messageType = type;
	strcpy(message.messageText, text);
	
	msgsnd(queue, &message, strlen(message.messageText) + 1, 0);
}

void handler(int signumber, siginfo_t* info, void* nonused)
{
  printf("Train signaled with signal #%i that it arrived\n",signumber);
}

void launchTrain(int totalNumberOfTrains){
	if((childpid = fork()) < 0){
		printf("fork error");
		exit(1);
	}
	if(childpid == 0) //child
	{
		Message recieved;
		sleep(1);
		msgrcv(msgQueue, &recieved, MAXMSG, 2, 0);
		
		int successPercentage;
		if((successPercentage = (rand() % 100)) < 10){
			printf("Oh no!.. Slipped on a banana!.. or froze in this harsh weather.\n");
		}
		
		printf("Im the %d. train. Total number of trains: %d. Trains to come: %d.\n", recieved.messageText, totalNumberOfTrains, totalNumberOfTrains - atoi(recieved.messageText));
		if(successPercentage < 10) {
			printf("I failed at fulfilling the task.\n");
			if(rand() % 2 < 1)
				sendMessage(msgQueue, "I slipped on a banana...", 3);
			else
				sendMessage(msgQueue, "I froze in the harsh weather...", 3);
		} else {
			printf("I successfully fullfilled the task.\n");
			sendMessage(msgQueue, "I reached the garage!", 3);
		}
		kill(getppid(), SIGARRIVED);
		
		exit(0);
	}
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	int totalNumberOfTrains = atoi(argv[1]);
	sigaction(SIGARRIVED, &sigact, NULL);
	
	key_t key;
	key = ftok(argv[0], 1);
	msgQueue = msgget(key, 0600|IPC_CREAT);
	
	launchTrain(totalNumberOfTrains);
	
	//parent
	printf("Im the not-a-train\n");
	char nthTrainText[100] = "";
	sprintf(nthTrainText, "%d", );
	sendMessage(msgQueue, nthTrainText, trainToLaunch);
	
	pause();
	Message recieved;
	msgrcv(msgQueue, &recieved, MAXMSG, 0);
	printf("Train #%d sent: %d\n", recieved.messageType, recieved.messageText);
	
	wait(NULL);
	msgctl(msgQueue, IPC_RMID, NULL);
}