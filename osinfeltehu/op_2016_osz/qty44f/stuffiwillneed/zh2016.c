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

#define SIGUNDERSTAND SIGRTMIN+1
#define MAXMSG 1024
#define MEMSIZE 1024
#define DOWN -1
#define UP 1

// Message
typedef struct Messages {
	long messageType;
	char messageText[MAXMSG];
} Message;

//Signal
void handler(int signumber, siginfo_t* info, void* nonused)
{
  printf("Signal with number %i has arrived\n",signumber);
  if (info->si_code == SI_QUEUE) 
    printf("Do you understand it now? %s\n", info->si_value.sival_int ? "Yepp" : "Nope");
}
//Semaphore
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
//Semaphor END

int main(int argc, char* argv[]){
	//fork
	pid_t childpid_1, childpid_2;
	
	//messages
	int msgQueue;
	key_t key;
	key = ftok(argv[0], 1);
	msgQueue = msgget(key, 0600|IPC_CREAT); //message queue open
	
	//pipe
	int pipe_fd[2]; //pipe open
	pipe(pipe_fd);
	
	//semaphore
	int semid = create_semaphore(argv[0], 0); //open semaphore
	printf("Semid: %d\n", semid);
	
	//shared memory
	char *shared_memory;
	int sh_mem_id;
	key_t shmem_key;
	shmem_key = ftok(argv[0], 1); //generate key
	sh_mem_id = shmget(shmem_key, MEMSIZE, IPC_CREAT|S_IRUSR|S_IWUSR); //open shared memory
	shared_memory = shmat(sh_mem_id, NULL, 0); //connect to shared memory
	strcpy(shared_memory, "0"); //write to shared memory
	
	//signal
	struct sigaction sigact;

	sigact.sa_sigaction = handler; //set handler
	sigact.sa_flags = SA_SIGINFO; //set flag
	sigemptyset(&sigact.sa_mask); //empty signal set
	sigaction(SIGUNDERSTAND, &sigact, NULL); //add sigunderstand to set
	
	semaphore_operation(semid, UP); //set semaphore to be opened
	
	if(msgQueue < 0) {	//msgqueue opening failed
		perror("msgget");
		exit(1);
	}
	
	if((childpid_1 = fork()) == -1) { //forking failed
		printf("Fork\n");
		exit(1);
	}
	
	if(childpid_1 == 0) {	//Child#1
		union sigval s_value_int;	//signal value
		
		srand(time(NULL) ^ (getpid() << 16));	//seeding random
	
		close(pipe_fd[0]);	//close read end
		
		printf("First student arrived\n");
		Message message;
		
		int status = msgrcv(msgQueue, &message, MAXMSG, 5, 0); // recieve message
		
		printf("Recvd msg: %s\n", message.messageText);
		
		char answer[3] = "";
		sprintf(answer, "%d", rand() % 4 + 1);
		
		write(pipe_fd[1], answer, strlen(answer) + 1);	//write to pipe file descriptor
		
		if(rand() % 2 == 1)	//generate random number
		{
			printf("#1 Does not understand it, writing question to shmem\n");
			
			char *question;
			
			printf("#1 Waiting for semafor opening\n");
			semaphore_operation(semid, DOWN);	//close semaphor
			printf("#1 Semaphor opened, now I closed it\n");
			
			sprintf(shared_memory, "%d", atoi(shared_memory) | 1);	//read n write shared memory
			
			semaphore_operation(semid, UP);	//open semaphor
			printf("#1 reopened Semaphor\n");
			pause();	//wait for signal	
			
			s_value_int.sival_int = rand() % 2;	//set recieved signal value
			sigqueue(getppid(), SIGUNDERSTAND, s_value_int);	//send signal value through queue
		}
		shmdt(shared_memory);	//release shared memory
		
		exit(0);
	}
	
	if((childpid_2 = fork()) == -1) { //forking failed
		close(pipe_fd[0]);
		
		printf("Fork\n");
		exit(2);
	}
		
	if(childpid_2 == 0) { // Child#2
		union sigval s_value_int;
		
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
			printf("#2 Does not understand it, writing question to shmem\n");
			
			char *question;
			
			printf("#2 Waiting for semafor opening\n");
			semaphore_operation(semid, DOWN);
			printf("#2 Semaphor opened, now I closed it\n");
			
			sprintf(shared_memory, "%d", atoi(shared_memory) | 2);
			
			semaphore_operation(semid, UP);
			printf("#2 reopened Semaphor\n");
			
			pause();
			s_value_int.sival_int = rand() % 2;
			sigqueue(getppid(), SIGUNDERSTAND, s_value_int);
		}
		shmdt(shared_memory);
		
		exit(0);
	}
	// Parent
	char readBuffer[MAXMSG];
	close(pipe_fd[1]);	//close write end
	
	Message question = { 5, "What is the meaning of life?" };
	
	printf("Sending questions\n");
	msgsnd(msgQueue, &question, strlen(question.messageText) + 1, 0); //send message
	printf("Question sent for first student\n");
	msgsnd(msgQueue, &question, strlen(question.messageText) + 1, 0); //send message
	printf("Question sent for second student\n");
	
	read(pipe_fd[0], readBuffer, MAXMSG);  //read from pipe read end
	printf("Answer #1: %s\n", readBuffer);
	
	read(pipe_fd[0], readBuffer, MAXMSG);  //read from pipe read end
	printf("Answer #2: %s\n", readBuffer);
	
	char received[MAXMSG];
	
	sleep(1); 
	printf("Waiting for semaphor to open up\n");
	semaphore_operation(semid, DOWN);
	printf("Semaphore opened\n");
	semaphore_operation(semid, UP);
	
	sleep(1);
	
	if(atoi(shared_memory) & 1) {
		printf("#1 Child does not get it...\n");
		
		kill(childpid_1, SIGUNDERSTAND); //send signal
		pause();
	}
	
	if(atoi(shared_memory) & 2) {
		printf("#2 Child does not get it...\n");
		
		kill(childpid_2, SIGUNDERSTAND); //send signal
		pause();
	}
	
	wait(NULL);
	
	delete_semaphore(semid);	//remove semaphore
	msgctl(msgQueue, IPC_RMID, NULL);	//remove messagequeue
	shmctl(sh_mem_id,IPC_RMID,NULL);	 //remove and erase shared memory
}