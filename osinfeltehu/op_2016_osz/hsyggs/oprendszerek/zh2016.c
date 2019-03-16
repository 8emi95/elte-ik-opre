#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAXMSG 1024
#define MEMSIZE 1024
#define DOWN -1
#define UP 1

typedef struct Messages {
	long messageType;
	char messageText[MAXMSG];
} Message;

int create_semaphore(const char* pathname, int semaphor_value){
	int semid;
	key_t key;
	
	key = ftok(pathname, 1);
	if((semid = semget(key, 1, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0)
		perror("semget");
	
	if(semctl(semid, 0, SETVAL, semaphor_value) < 0)
		perror("semctl");
	
	return semid;
}

int semaphore_operation(int semid, int op) {
	struct sembuf operation;
	
	operation.sem_num = 0;
	operation.sem_op = op; // 1 => up | -1 => down
	operation.sem_flg = 0;
	
	if(semop(semid, &operation, 1) < 0)
		perror("semop");
}

void delete_semaphore(int semid){
      semctl(semid, 0, IPC_RMID);
}

int main(int argc, char* argv[]){	
	pid_t childpid;
	
	int msgQueue;
	key_t key;
	key = ftok(argv[0], 1);
	msgQueue = msgget(key, 0600|IPC_CREAT);
	
	int pipe_fd[2];
	pipe(pipe_fd);
	
	int semid = create_semaphore(argv[0], 0);
	
	printf("Semid: %d\n", semid);
	
	char *shared_memory;
	int sh_mem_id;
	key_t shmem_key;
	shmem_key = ftok(argv[0], 1);
	sh_mem_id = shmget(shmem_key, MEMSIZE, IPC_CREAT|S_IRUSR|S_IWUSR);
	shared_memory = shmat(sh_mem_id, NULL, 0);
	strcpy(shared_memory, "0");
	
	if(msgQueue < 0) {
		perror("msgget");
		exit(1);
	}
	
	if((childpid = fork()) == -1) {
		printf("Fork\n");
		exit(1);
	}
	
	if(childpid == 0) {	//Child#1
		srand(time(NULL) ^ (getpid() << 16));
	
		close(pipe_fd[0]);
		
		printf("First student arrived\n");
		Message message;
		
		int status = msgrcv(msgQueue, &message, MAXMSG, 5, 0);
		
		printf("Recvd msg: %s\n", message.messageText);
		
		char answer[3] = "";
		sprintf(answer, "%d", rand() % 4 + 1);
		
		write(pipe_fd[1], answer, strlen(answer) + 1);
		
		if(rand() % 2 == 1)
		{
			printf("#1 Does not understand, writing answer to shmem\n");
			char *question;
			
			printf("#1 Waiting for semafor opening\n");
			semaphore_operation(semid, DOWN);
			printf("#1 Semaphor opened\n");
			
			strcpy(question, shared_memory);
			int modify = atoi(question) | 1;
			strcpy(shared_memory, question);
			
			semaphore_operation(semid, UP);
		}
		shmdt(shared_memory);
		exit(0);
	}
	
	if((childpid = fork()) == -1) {
		close(pipe_fd[0]);
		
		printf("Fork\n");
		exit(2);
	}
		
	if(childpid == 0) { // Child#2
		srand(time(NULL) ^ (getpid() << 16));
		
		printf("Second student arrived\n");
		Message message;
		int status = msgrcv(msgQueue, &message, MAXMSG, 5, 0);
		
		printf("Recvd msg: %s\n", message.messageText);
		
		char answer[3] = "";
		sprintf(answer, "%d", rand() % 4 + 1);
		
		write(pipe_fd[1], answer, strlen(answer) + 1);
		
		if(rand() % 2 == 1)
		{
			printf("#2 Does not understand, writing answer to shmem\n");
			
			char *question;
			printf("#2 Waiting for semafor opening\n");
			semaphore_operation(semid, DOWN);
			printf("#2 Semaphor opened\n");
			
			strcpy(question, shared_memory);
			int modify = atoi(question) | 2;
			strcpy(shared_memory, question);
			
			semaphore_operation(semid, UP);
		}
		
		shmdt(shared_memory);
		exit(0);
	} else { // Parent
		char readBuffer[MAXMSG];
		close(pipe_fd[1]);
		
		Message question = { 5, "What is the meaning of life?" };
		
		printf("Sending questions\n");
		msgsnd(msgQueue, &question, strlen(question.messageText) + 1, 0);
		printf("Question sent for first student\n");
		msgsnd(msgQueue, &question, strlen(question.messageText) + 1, 0);
		printf("Question sent for second student\n");
		
		read(pipe_fd[0], readBuffer, MAXMSG);
		printf("Answer #1: %s\n", readBuffer);
		
		read(pipe_fd[0], readBuffer, MAXMSG);
		printf("Answer #2: %s\n", readBuffer);
		
		char received[MAXMSG];
		
		printf("Waiting for semaphor to open up\n");
		semaphore_operation(semid, DOWN);
		strcpy(received, shared_memory);
		semaphore_operation(semid, UP);
		printf("Sharedmemcpy %s\n", received);
		
		wait(NULL);
		
		delete_semaphore(semid);
		shmctl(sh_mem_id,IPC_RMID,NULL);
	}
	
}