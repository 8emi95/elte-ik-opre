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

typedef struct Messages {
	long messageType;
	char messageText[MAXMSG];
} Message;

void handler(int signumber, siginfo_t* info, void* nonused)
{
  printf("Train signaled with number %i that it arrived\n",signumber);
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	int maxNumberOfTrains = atoi(argv[1]);
	
	struct sigaction sigact;
	sigact.sa_sigaction = handler;
	sigact.sa_flags = SA_SIGINFO;
	sigemptyset(&sigact.sa_mask);
	sigaction(SIGARRIVED, &sigact, NULL);
	
	int msgQueue;
	key_t key;
	key = ftok(argv[0], 1);
	msgQueue = msgget(key, 0600|IPC_CREAT);
	
	int i = 1;
	while(i <= maxNumberOfTrains) {
		pid_t childpid;
		if((childpid = fork()) < 0){
			printf("fork error");
			exit(1);
		}
		if(childpid == 0) //child
		{
			sleep(1);
			Message childRecieved;
			//sleep(1);
			int successPercentage = rand() % 100;
			
			msgrcv(msgQueue, &childRecieved, MAXMSG, 2, 0);
			printf("Received message: %s\n", childRecieved.messageText);
			
			int myNumber = atoi(childRecieved.messageText);
			
			printf("Im the %d. train. Total number of trains: %s. Trains to come: %d.\n", myNumber, argv[1], atoi(argv[1]) - myNumber);
			if(successPercentage < 10) {
				if(rand() % 2 < 1) {
					const Message message = { 3, "I slipped on a banana..."};
					
					msgsnd(msgQueue, &message, strlen(message.messageText) + 1, 0);
				}
				else {
						const Message message = { 3, "I froze in this weather..."};
						
						msgsnd(msgQueue, &message, strlen(message.messageText) + 1, 0);
				}
			}
			else {
					const Message message = { 3, "I reached the garage!"};

					msgsnd(msgQueue, &message, strlen(message.messageText) + 1, 0);
			}
			sleep(1);
			kill(getppid(), SIGARRIVED);
			
			exit(0);
		}
	
		char nthTrainText[100] = "";
		sprintf(nthTrainText, "%d", i);		
		
		i++;
		
		Message message;
		message.messageType = 2;
		strcpy(message.messageText, nthTrainText);
		
		printf("Sent message: %s\n", message.messageText);
		msgsnd(msgQueue, &message, strlen(message.messageText) + 1, 0);
		
		pause();
		Message recieved;
		msgrcv(msgQueue, &recieved, MAXMSG, 3, 0);
		printf("Train sent: %s\n", recieved.messageText);
		
		wait(NULL);
		msgctl(msgQueue, IPC_RMID, NULL);
	}
}