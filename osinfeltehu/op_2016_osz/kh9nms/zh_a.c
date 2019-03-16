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

#define SIGARRIVED SIGRTMIN+1

typedef struct Messages {
	long messageType;
	char messageText[1024];
} Message;

void handler(int signumber, siginfo_t* info, void* nonused)
{
  printf("A %i szamu szerelveny jelzett, hogyvisszaert a kozpontba\n",signumber);
}

void sendMessage(int queue, char *text, int type) {
	Message message;
	message.messageType = type;
	strcpy(message.messageText, text);
	
	msgsnd(queue, &message, strlen(message.messageText) + 1, 0);
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	pid_t childpid;
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
	
	if((childpid = fork()) < 0){
		printf("fork error");
		exit(1);
	}
	if(childpid == 0) // szerelveny / gyerek
	{
		Message recieved;
		//sleep(1);
		msgrcv(msgQueue, &recieved, 1024, 2, 0);
		
		int successPercentage;
		if((successPercentage = (rand() % 100)) < 10){
			printf("Baj van: bananhej vagy zord ido.\n");
		}
		
		printf("Ez a %s. szerelveny. Hatra van: %d. \nMai Osszes szerelveny: %s.\n", recieved.messageText, atoi(argv[1]) - atoi(recieved.messageText), argv[1]);
		if(successPercentage < 10) {
			printf("Nem teljesitettem a muszakom :'(\n");
			if(rand() % 2 < 1)
				sendMessage(msgQueue, "Ok: nem megfelelo palyatisztantartas ( banan a sinen ... )", 3);
			else
				sendMessage(msgQueue, "Ok: Ilyen hidegben nincs kedvem kimenni a hidegbe, maradj Te is otthon", 3);
		}
		else {
			printf("Minden bajtol mentes utam volt :)\n");
			sendMessage(msgQueue, "Megerkeztem a kozpontba", 3);
		}
		kill(getppid(), SIGARRIVED);
		
		exit(0);
	}
	
	// szulo
	printf("BMF megnyitott\n");
	char nthTrainText[100] = "";
	sprintf(nthTrainText, "%d", rand() % maxNumberOfTrains + 1);
	sendMessage(msgQueue, nthTrainText, 2);
	
	pause();
	Message recieved;
	msgrcv(msgQueue, &recieved, 1024, 3, 0);
	printf("Szerelveny uzenete: %s\n\nBoldog Karacsony es Kellemes Uj Esztendot Kivan Onnek es Csaladjanak a BMF!\n\n", recieved.messageText);
	
	wait(NULL);
	msgctl(msgQueue, IPC_RMID, NULL);
}
