#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <semaphore.h>

//**
//Compile it using -pthread (Because of POSIX Semaphore)
//**


//Global constants
#define SHARED_MEMORY_SIZE 1024
#define MESSAGE_SIZE 1024


struct message
{
	long mtype; 		  //Number to distinguish messages(message type)
	char mtext[ MESSAGE_SIZE ];   //The message itself
};

//  messageq_id: the ID of the desired message queue
//  message_type: with messagetype message_type
int send_message( int messageq_id, long message_type, const char *message)
{
	struct message msg;
	msg.mtype = message_type;
	strcpy(msg.mtext, message);

	int status;

	status = msgsnd( messageq_id, &msg, sizeof(msg.mtext) , 0 );

	if ( status < 0 )
		perror("Error while sending message through message queue");
	return 0;
}

int receive_message( int messageq_id, long message_type, char *message_buffer)
{
	struct message msg;
	int status;

	// If message_type == 0 then the first message is received
	// If message_type  > 0 then the first message with mtype == message_type is received
	// Last parameter: message flag
	// If the received message has size greater than MESSAGE_SIZE:
	//   If the last parameter is 0, then the call fails, the message stays in the queue and the call returns -1
	//   If the last parameter is 1, then the message is truncated to MESSAGE_SIZE and the call succeeds

	status = msgrcv(messageq_id, &msg, MESSAGE_SIZE, message_type, 0);

	if ( status < 0 ){
		perror("Error while receiving message through message queue");
		return -1;
	}
	else{
		//printf( "Message with code %ld:  %s\n", msg.mtype, msg.mtext );
		strcpy(message_buffer, msg.mtext);
	}

	return 0;
}

sem_t *create_semaphore(char *semaphore_name, int initial_value)
{
    sem_t *semid = sem_open(semaphore_name, O_CREAT, S_IRUSR | S_IWUSR, initial_value);
	if (semid == SEM_FAILED){
		perror("Could not create semaphore.");
	}
    return semid;
}

void delete_semaphore(char *semaphore_name)
{
    sem_unlink(semaphore_name);
}

void handler(int sig) {}

int main (int argc,char* argv[]) {

	//Connecting used signals to the handler
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	signal(SIGINT, handler);

	//IDs, keys and basic variables
    key_t key;
	pid_t child1, child2;
	sem_t *mutex;
    int sh_mem_id, messageq_id;
	int status1, status2;
	char *semaphore_name = "/my_sem";

	//Creating a key for shared memory and messagequeues
	key = ftok(argv[0], 1);

	//Acquiring shared memory
	int *shared_memory;		//Storing integers
    sh_mem_id = shmget(key, SHARED_MEMORY_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    shared_memory = shmat(sh_mem_id, NULL, 0);

	//Creating a messagequeue
	messageq_id = msgget( key, 0600 | IPC_CREAT );

	//Creating a POSIX Semaphore
	//Default value is 0, 1 sem_post is required
    mutex = create_semaphore(semaphore_name, 0);

	//Blocking SIGUSR1, SIGUSR2 and SIGINT
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGUSR1);
	sigaddset(&sigset,SIGUSR2);
	sigaddset(&sigset,SIGINT);
	sigprocmask(SIG_BLOCK,&sigset,NULL);


	//Creating an unnamed pipe with error handling
	int p1[2];

	//**Error handling**
	if (pipe(p1) == -1)
	{
		perror("Error when trying to open pipe!");
		exit(EXIT_FAILURE);
	}
	//************

	child1 = fork();

	//**Error handling**
	if(child1 < 0)
	{
		perror("The fork calling was not successful for Child1\n");
		exit(1);
	}
	//************


	if(child1 > 0)
	{

		child2 = fork();

		//**Error handling**
		if(child2 < 0){
			perror("The fork calling was not successful for Child2\n");
			exit(1);
		}
		//************

/**********************  Parent procecss *****************************/
//Accepts SIGUSR1

		if(child2 > 0)
		{
			//Defining sigset for parent
			//Waits for Child1 and Child2
			sigset_t parent_sigset;
			sigfillset(&parent_sigset);
			sigdelset(&parent_sigset, SIGUSR1);
			sigdelset(&parent_sigset, SIGUSR2);

			//***************


			// parent waits for (possibly) both children
			sigsuspend(&parent_sigset);
			sigsuspend(&parent_sigset);

			printf("Both children are ready\n");

			// Writing to shared memory
			// Child1 fwill read the pid of child2 from this
			shared_memory[0] = child2;
			shared_memory[1] = 345;

			// Semaphore is now accessible for 1 member
			// Now Child1 can read from it
			sem_post(mutex);

			
			
			
			

			waitpid(child2, &status2, 0);
			waitpid(child1, &status1, 0);

			//Deleting used resources (Semaphore, message queue, shared memory segment)
			int queue_status;
			queue_status = msgctl( messageq_id, IPC_RMID, NULL );
			if ( queue_status < 0 )
				perror("Error while deleting messagequeue");

			delete_semaphore(semaphore_name);
			shmctl(sh_mem_id, IPC_RMID, NULL);

			printf("Parent finished\n");
		}

		/**********************  Child2 *****************************/
		//Accepts SIGUSR2

		else
		{
			//Defining sigset for Child2
			//Waits for SIGUSR2

			sigset_t c2_sigset;
			sigfillset(&c2_sigset);
			sigdelset(&c2_sigset, SIGUSR2);

			//***************
			
			printf("Child2 sends ready signal\n");
			kill(getppid(), SIGUSR1);
			
			// waits for signal from Child1
			sigsuspend(&c2_sigset);
			printf("Child2 received signal from Child1.\n");
			

			printf("Child2 finished.\n");
		}

	}

	/**********************  Child1 *****************************/
	//Accepts SIGUSR2
	else
	{
		//Defining sigset for child 1
		//Waits for SIGUSR1

		sigset_t c1_sigset;
		sigfillset(&c1_sigset);
		sigdelset(&c1_sigset, SIGUSR1);

		//***************

		printf("Child1 sends ready signal\n");
		kill(getppid(), SIGUSR1);

		
		//Reading pid of Child2 from shared memory
		sem_wait(mutex);
		child2 = shared_memory[0];
		printf("Child1 read: %d\n", child2);
		sem_post(mutex);

		printf("Child1 sends signal to Child2\n");
		kill(child2, SIGUSR2);
		
		printf("Child1 finished!\n");
	}


	return 0;
}
