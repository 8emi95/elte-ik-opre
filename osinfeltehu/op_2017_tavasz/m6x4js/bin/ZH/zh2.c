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

struct int_message { 
	long mtype; 		  //Number to distinguish messages(message type)
	int mtext;   //The message itself
}; 

int send_int_message( int messageq_id, long message_type, const int *message) 
{ 
	struct int_message msg;
	msg.mtype = message_type;
	msg.mtext = *message;
	
	int status; 
	status = msgsnd( messageq_id, &msg, sizeof(msg.mtext) , 0 ); 

	if ( status < 0 ) 
		perror("Error while sending message through message queue"); 
	return 0; 
} 

int receive_int_message( int messageq_id, long message_type, int *message_buffer) 
{ 
	struct int_message msg; 
	int status; 
	status = msgrcv(messageq_id, &msg, MESSAGE_SIZE, message_type, 0);

	if ( status < 0 ){
		perror("Error while receiving message through message queue");
		return -1;
	}
	else{
		*message_buffer = msg.mtext;
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
	srand(time(NULL));
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
	int p1[2],  p2[2];

	//**Error handling**
	if (pipe(p1) == -1)
	{
		perror("Error when trying to open pipe!");
		exit(EXIT_FAILURE);
	}

	if (pipe(p2) == -1)
	{
		perror("Error when trying to open pipe!");
		exit(EXIT_FAILURE);
	}

	child1 = fork();
	
	//**Error handling**
	if(child1 < 0){
		perror("The fork calling was not succesful for Child1\n"); 
		exit(1);
	}
	
	if(child1 > 0){
		
		child2 = fork();
		

		if(child2 < 0){
			perror("The fork calling was not succesful for Child2\n"); 
			exit(1);
		}

		
/**********************  Parent procecss *****************************/		
		if(child2 > 0){		
			//Defining sigset for parent
			//Waits for SIGUSR1
			sigset_t parent_sigset;
			sigfillset(&parent_sigset);
			sigdelset(&parent_sigset, SIGUSR1);
			
			//Semaphore 
			sem_post(mutex);
			
			sigsuspend(&parent_sigset);
			
			send_int_message(messageq_id, 4, &child2);
			
			kill(child1, SIGUSR2);
			//printf("signal sent");
			
			sigsuspend(&parent_sigset);
			
			//printf("Parent before mutex");
			
			sem_wait(mutex);
			printf("Agency received money: %d\n", shared_memory[0]);
			sem_post;
			
			
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
		else{
			//Defining sigset for Child2
			//Waits for SIGINT
			
			sigset_t c2_sigset;
			sigfillset(&c2_sigset);
			sigdelset(&c2_sigset, SIGINT);
			
			kill(getppid(), SIGUSR1);
			sigsuspend(&c2_sigset);
            printf("I accepted the phone call!");

            int offer;

            /*while(ans != 0 || offer != offer/2){

            }*/
            //close(p1[1]);
            //read(p1[0], &offer, sizeof(offer));

			int ans = random() & 1;
			printf("%d\n", ans);
			
			close(p2[0]);
			write(p2[1], &ans, sizeof(int));
			
			kill(child1, SIGUSR2);
			
			printf("Child 2 finished.\n");
		}
		
	}
	
	/**********************  Child1 *****************************/
	else{
		//Defining sigset for child 1
		//Waits for SIGUSR2
		
		sigset_t c1_sigset;
		sigfillset(&c1_sigset);
		sigdelset(&c1_sigset, SIGUSR2);
		
		sigsuspend(&c1_sigset);
		
		int child2;

        int usr_answear;
        int currentPrice = atoi(argv[2]);
				
		receive_int_message(messageq_id, 4, &child2);
		
        printf("Phone: %d, started calling.\n", child2);

        printf("The offer: %s - %s \n", argv[1], argv[2]);
		
		
		kill(child2, SIGINT);
		
		sigsuspend(&c1_sigset);

        /*while(usr_answear != 0 || offer != offer/2){

        }*/

        //close(p1[0]);
       // write(p1[1], &currentPrice, sizeof(currentPrice));
		close(p2[1]);
		read(p2[0], &usr_answear, sizeof(int));
        if(usr_answear == 0){
            printf("Elfogadta.\n");
        }
        else
        {
            printf("Nem fogadta el.\n");
            //currentPrice = 0;
            currentPrice = currentPrice - 1;
        }

		sem_wait(mutex);
		shared_memory[0] = currentPrice;
		sem_post(mutex);
		
		kill(getppid(), SIGUSR1);
		
		printf("Child1 finished!\n");
	}
	return 0;
}
