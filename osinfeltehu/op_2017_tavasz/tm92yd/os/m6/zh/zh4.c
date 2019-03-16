#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

struct messageBuffer
{
	long messageType;
	char messageText[256];
};

int main()
{
	pid_t firstStudent;
	pid_t secondStudent;

	key_t key = 2012;
	int messageID;
	int status = 0;
	int firstStatus = 0;
	int secondStatus = 0;
	struct messageBuffer messageQueue;
	
	int pfdFirst[2];
	int pfdSecond[2];

	//üzenetsor létrehozása
	messageID = msgget(key, IPC_CREAT | 0600);
	if (messageID < 0)
	{
		perror("\nUzenetsor: A kérdés elküldése sikertelen!\n");
		exit(1);
	}
	
	if(pipe(pfdFirst) < 0)
	{
		perror("\nPipe: Csővezeték létrehozása sikertelen!\n");
		exit(1);
	}

	if(pipe(pfdSecond) < 0)
	{
		perror("\nPipe: Csővezeték létrehozása sikertelen!\n");
		exit(1);
	}
	
	//első gyerek
	firstStudent = fork();
	if(firstStudent < 0)
	{
		perror("\nFork: Az első hallgató létrehozása sikertelen!\n");
		exit(1);
	}
	if(firstStudent == 0)
	{
		char question[256];
		status = msgrcv(messageID, &messageQueue, 256, 100, 0);
		strcpy(question, messageQueue.messageText);

		printf("1. hallgató: A feltett kérdés: %s\n", question);
		
		sleep(1);
		
		close(pfdFirst[0]);
		
		srand(time(NULL));
		int r = rand() % 4 + 1;
		
		printf("1. hallgato: %d", r);
		write(pfdFirst[1], &r, sizeof(r));
		
		close(pfdFirst[1]);
		
		_exit(0);
	}

	//második gyerek
	secondStudent = fork();
	if(firstStudent < 0)
	{
		perror("\nFork: A masodik hallgató létrehozása sikertelen!\n");
		exit(1);
	}
	if(secondStudent == 0)
	{
		char question[256];
		status = msgrcv(messageID, &messageQueue, 256, 100, 0);
		strcpy(question, messageQueue.messageText);

		printf("2. hallgató: A feltett kérdés: %s\n", question);
		
		sleep(2);
		
		close(pfdSecond[0]);
		
		srand(time(NULL));
		int r = rand() % 4 + 1;
		printf("2. hallgato: %d", r);
		write(pfdSecond[1], &r, sizeof(r));
		
		close(pfdSecond[1]);
		
		_exit(0);
	}
	
	//szülő
	if(firstStudent != 0 && secondStudent != 0)
	{
		messageQueue.messageType = 100;
		strcpy(messageQueue.messageText, "Mennyi 2 * 2?");
		
		status = msgsnd(messageID, &messageQueue, sizeof(messageQueue.messageText), 0);
		if(status < 0)
		{
			perror("\nHiba a kérdés küldése során!\n");
		}
		
		status = msgsnd(messageID, &messageQueue, sizeof(messageQueue.messageText), 0);
		if(status < 0)
		{
			perror("\nHiba a kérdés küldése során!\n");
		}
		
		int firstOpinion;
		int secondOpinion;
		
		close(pfdFirst[1]); 
		read(pfdFirst[0], &firstOpinion, sizeof(int));
		close(pfdFirst[0]);
		
		close(pfdSecond[1]); 
		read(pfdSecond[0], &secondOpinion, sizeof(int));
		close(pfdSecond[0]);
		
		printf("Oktato: Az 1. hallgató válasza %d. A 2. hallgató válasza %d.", firstOpinion, secondOpinion);
		
		int close(int pfdFirst);
		int close(int pfdSecond);
		
		sleep(2);
		_exit(0);
	}
	
	return 0;
}