#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // DateTime kezeles
#include <fcntl.h> // Rendszerhivasok file
#include <unistd.h> // fork
#include <sys/types.h> // pid_t típus
#include <sys/wait.h> // wait, waitpid
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

//////////////////////////////////
// IMPORTANT!!!!!!!!!!!!!!!!!!!!!!
// GEANY BUILD COMMAND (for SEMs):
// gcc -Wall -pthread -o "%e" "%f"
//////////////////////////////////


#define SEM_NAME "/mysem"

typedef enum {FALSE, TRUE} bool;


int pipe1fd[2], pipe2fd[2];
char pipeMessage[100];


////////////////
// Pipe handling
////////////////
void GetPipeMessage(int *pipefd){
	sleep(1);
	close(pipefd[1]); //Usually we close the unused write end
	//printf("%i Gyerek elkezdi olvasni a csobol az adatokat!\n", getpid());
	read(pipefd[0], pipeMessage, sizeof(pipeMessage));// reading max 100 chars
	printf("[CHILD%i] - Read pipe message: %s\n", getpid(), pipeMessage);
	close(pipefd[0]);// finally we close the used read end			
	kill(getppid(),SIGUSR2);
}

//////////////////
// Signal handling
//////////////////
pid_t pid1, pid2;
void HandlerParent(int signalNumber){
	printf("[PARENT] Parent received signal: %d\n", signalNumber);
}

void HandlerChild1(int signalNumber){
	switch(signalNumber)
	{
		case SIGUSR2:
			printf("[CHILD1] Child 1 received signal: %d\n", signalNumber);
			break;
		case SIGCONT:
			printf("[CHILD1] Child 1 received signal: %d -- ACTION: Reading from pipe!\n", signalNumber);
			GetPipeMessage(pipe1fd);
			break;
	}
}

void HandlerChild2(int signalNumber){
	switch(signalNumber)
	{
		case SIGUSR2:
			printf("[CHILD2] Child 2 received signal: %d\n", signalNumber);
			break;
		case SIGCONT:
			printf("[CHILD2] Child 2 received signal: %d -- ACTION: Reading from pipe!\n", signalNumber);
			GetPipeMessage(pipe2fd);
			break;
	}
}

/////////////////////////
// Message Queue handling
/////////////////////////

struct s_queueMessage
{
	long mtype;
	char mtext [1024];
};
typedef struct s_queueMessage queueMessage;

int SendMessageToQueue(int messageQueue)
{
	char *valami = "[QUEUE] Message";
	queueMessage qMessage;
	sprintf(qMessage.mtext, "%s", valami);

	qMessage.mtype = 8;
	int sendStatus;
	
	sendStatus = msgsnd(messageQueue, &qMessage, strlen(qMessage.mtext) + 1, 0);
	if(sendStatus < 0)
	{
		perror("[ERROR] Queue message sending!");
	}
	
	return 0;
}

int ReadMessageFromQueue(int messageQueue)
{
	queueMessage qMessage;
	int readStatus;
	
	readStatus = msgrcv(messageQueue, &qMessage, 1024, 0, 0);
	if(readStatus < 0)
	{
		perror("[ERROR] Queue message reading!");
	}
	else
	{
		printf("[CHILD%i] The received message: %ld, %s\n", getpid(), qMessage.mtype, qMessage.mtext);
	}
	
	return 0;
}

/////////////////////
// Semaphore handling
/////////////////////
sem_t *mySemaphore;
int ret,val;


int main(int argc, char *argv[])
{
	int waitStatus;
	
	// Parent prep 1 code
	printf("[PARENT] Parent started! PID: %d\n", getpid());
	
	// Signals
	signal(SIGCONT, HandlerParent);
	signal(SIGUSR2, HandlerParent);
	
	// Pipes
	if (pipe(pipe1fd) == -1) 
	{
		perror("[ERROR] Pipe1 is not created!\n");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipe2fd) == -1) 
	{
		perror("[ERROR] Pipe2 is not created!\n");
		exit(EXIT_FAILURE);
	}
	
	// Message Queue
	int messageQueue, queueStatus;
	key_t queueKey;
	
	queueKey = ftok(argv[0], 1);
	
	messageQueue = msgget(queueKey, 0700 | IPC_CREAT);
	if(messageQueue < 0)
	{
		perror("[QUEUE] msgget error!");
		return 1;
	}
	
	// Semaphore
	if ((mySemaphore = sem_open(SEM_NAME, O_CREAT , 0600, 1)) == SEM_FAILED)
	{
		perror("sem_open");
		return -1;
	}
	
	// Get the current semaphore value
	sem_getvalue(mySemaphore, &val);
	printf("[SEMAPHORE] Semaphore value = %d\n", val);
	
	
	// Main Code
	printf("[PARENT] Child1 is starting!\n");
	SendMessageToQueue(messageQueue);
	pid1 = fork();
	if(pid1 == -1)
	{
		perror("[PARENT] Child1: Fork error!\n");
	    exit(EXIT_FAILURE);
	}
	else if(pid1 == 0)
	{
		// Child1 code
		printf("[CHILD1] Child1 is running! PID: %d\n", getpid());
		sleep(1);
		signal(SIGCONT, HandlerChild1);
		signal(SIGUSR2, HandlerChild1);
		ReadMessageFromQueue(messageQueue);
		printf("[CHILD1] Sending signal to parent: %d\n", getppid());
		kill(getppid(), SIGUSR2);
		pause();
	}
	
	if(pid1 > 0)
	{
		// Parent prep 2 code
		printf("[PARENT] Waiting for Child1 signal.\n");
		pause();
		
		printf("[PARENT] Child2 is starting!\n");
		pid2 = fork();
		if(pid2 == -1)
		{
			perror("[PARENT] Child2: Fork error!\n");
			exit(EXIT_FAILURE);
		}
		else if(pid2 == 0)
		{
			// Child2 code
			printf("[CHILD2] Child2 is running! PID: %d\n", getpid());
			sleep(1);
			signal(SIGCONT, HandlerChild2);
			signal(SIGUSR2, HandlerChild2);
			printf("[CHILD2] Sending signal to parent: %d\n", getppid());
			kill(getppid(), SIGUSR2);
			pause();
		}
		
		if(pid2 > 0)
		{
			// Parent main code
			printf("[PARENT] Waiting for Child2 signal.\n");
			pause();
			
			char pMessage[30] = "[PIPE] This is a pipe message!";
			close(pipe1fd[0]);
			write(pipe1fd[1], pMessage, sizeof(pMessage));
			close(pipe1fd[1]);
			fflush(NULL);			
			printf("[PARENT] Message sent to the pipe!\n");
			printf("[PARENT] Message sent to the queue!\n");
			kill(pid1, SIGCONT);
			pause();
			waitpid(pid1, &waitStatus, 0); // Várakozás a gyermekfolyamat befejeződésére.
			if (WIFEXITED(waitStatus))
			{
				printf("[PARENT] Child1: Normal terminaton\n");
			}
			else
			{
				printf("[PARENT] Child1: Error in termination\n");
			}
			
			close(pipe2fd[0]);
			write(pipe2fd[1], pMessage, sizeof(pMessage));
			close(pipe2fd[1]);
			fflush(NULL);			
			printf("[PARENT] Message sent to the pipe!\n");
			kill(pid2, SIGCONT);
			pause();
			waitpid(pid2, &waitStatus, 0); // Várakozás a gyermekfolyamat befejeződésére.
			if (WIFEXITED(waitStatus))
			{
				printf("[PARENT] Child2: Normal termination\n");
			}
			else
			{
				printf("[PARENT] Child2: Error in termination\n");
			}
			
			queueStatus = msgctl( messageQueue, IPC_RMID, NULL );
			if(queueStatus < 0)
			{
				perror("msgctl");
			}
			printf("[PARENT] Queue is removed!\n");
			
			//printf("[PARENT] Child1 is killed!\n");
			//kill(pid1, SIGKILL);
			//printf("[PARENT] Child2 is killed!\n");
			//kill(pid2, SIGKILL);
			
			if (sem_post(mySemaphore) != 0)
			{
				perror("post error");
			}
			
			sem_close(mySemaphore);
			sem_unlink(SEM_NAME);
		}
	}
	
	
	return 0;
}
