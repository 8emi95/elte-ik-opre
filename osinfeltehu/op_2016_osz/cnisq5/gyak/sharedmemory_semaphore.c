#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <wait.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/shm.h>
#include <sys/sem.h>

struct Question { 
     long mtype;
     char mQuestion[1000]; 
}; 

int CreateSemaphore(const char* pathname,int semaphoreVal){
    int semid;
    key_t key;
    
    key=ftok(pathname,1);    
    if((semid=semget(key,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if(semctl(semid,0,SETVAL,semaphoreVal)<0)    //0= first semaphores
        perror("semctl");
       
    return semid;
}


void SemaphoreOperation(int semid, int op){
    struct sembuf operation;
    
    operation.sem_num = 0;
    operation.sem_op = op; // op=1 up, op=-1 down 
    operation.sem_flg = 0;
    
    if(semop(semid,&operation,1)<0) // 1 number of sem. operations
        perror("semop");	    
}

void DeleteSemaphore(int semid){
      semctl(semid,0,IPC_RMID);
}


int mqSend(int mq, struct Question qu)
{ 
     int status;
     status = msgsnd( mq, &qu, sizeof(qu) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct Question* qu) 
{ 
     int status; 
     status = msgrcv(mq, qu, sizeof(*qu), 0, 0 ); 
	 //printf("%ld %s\n",qu->mtype, qu->mQuestion);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

void ParentProc(int child1Pid, int child2Pid, int messageQueue, int* child1Pipe, int* child2Pipe, int* sharedInt, int semid)
{
	//initializing shared variable in parent
	*sharedInt = 0;
	SemaphoreOperation(semid, 1);
	
	//messagequeue
	struct Question qu = { 2, "Kerdes?" };
	mqSend(messageQueue, qu);
	mqSend(messageQueue, qu);
	//pipe
	close(child1Pipe[1]);
	close(child2Pipe[1]);
	int answer1, answer2;
	read(child1Pipe[0], &answer1, sizeof(answer1));
	read(child2Pipe[0], &answer2, sizeof(answer2));
	printf("Valaszok: %d, %d\n", answer1, answer2);
	//wait for childs to write semaphore
	sleep(1);
	//semaphore
	int notUnderstand = 0;
	SemaphoreOperation(semid, -1);
	notUnderstand = *sharedInt;
	SemaphoreOperation(semid, 1);
	if(notUnderstand == 0)
	{
		printf("Mindenki megertette.\n");
	}
	else
	{
		printf("Nem ertette meg: %d\n", notUnderstand);
	}
	//waiting for the childs to terminate
	int status;
	waitpid(child1Pid,&status,0); 
	waitpid(child2Pid,&status,0); 
	//delete mq
	status = msgctl( messageQueue, IPC_RMID, NULL ); 
	if ( status < 0 ) 
		perror("msgctl"); 
}

void Child1Proc(int messageQueue, int* child1Pipe, int* sharedInt, int semid)
{
	//seed
	srand(time(NULL) + 100 * getpid());
	//messagequeue
	struct Question qu;
	mqReceive(messageQueue, &qu);
	printf("Child 1: A kapott uzenet kodja: %ld, szovege:  %s\n", qu.mtype, qu.mQuestion);
	//pipe
	close(child1Pipe[0]);
	int r = (rand() % 4) + 1;
	write(child1Pipe[1], &r, sizeof(r));
	close(child1Pipe[1]);
	//semaphore
	r = (rand() % 100) + 1;
	if(r <= 50)
	{
		//didnt understand -> write shared memory
		SemaphoreOperation(semid, -1);
		*sharedInt = 1;
		SemaphoreOperation(semid, 1);
	}
	sleep(1);
}

void Child2Proc(int messageQueue, int* child2Pipe, int* sharedInt, int semid)
{
	//seed
	srand(time(NULL) + 100 * getpid());
	//messagequeue
	struct Question qu;
	mqReceive(messageQueue, &qu);
	printf("Child 2: A kapott uzenet kodja: %ld, szovege:  %s\n", qu.mtype, qu.mQuestion);
	//pipe
	close(child2Pipe[0]);
	int r = (rand() % 4) + 1;
	write(child2Pipe[1], &r, sizeof(r));
	close(child2Pipe[1]);
	//semaphore
	r = (rand() % 100) + 1;
	if(r <= 50)
	{
		//didnt understand -> write shared memory
		SemaphoreOperation(semid, -1);
		*sharedInt = 2;
		SemaphoreOperation(semid, 1);
	}
	
	sleep(1);
}

int main(int argc, char* argv[])
{
    int messageQueue, status; 
    key_t key;
	//get key
	key = ftok(argv[0],2); 
	//create mq
	messageQueue = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	//create pipe - maybe the other one can be declared here too? idc
	int child1Pipe[2];
	if (pipe(child1Pipe) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	//create shared memory
	int shared_mem_id;
	int* sharedInt = malloc(sizeof(int)); //didnt keep its value when initialized after this line
										  //so I did it in parent
	//second parameter can be any number
	//e.g:1000 is good if you share a char* insted of int - (it could also be 1000 now)
	shared_mem_id=shmget(key,sizeof(sharedInt),IPC_CREAT|S_IRUSR|S_IWUSR);
	sharedInt = shmat(shared_mem_id, NULL, 0); //returns a pointer so shared data must be a pointer
	
	//semaphore - ez a fajta módszer !azt hiszem! csak a bináris szemaforhoz jó
	int semid = CreateSemaphore(argv[0], 0); //second parameter is the state of semaphore
	//it's down because we want the parent to be the first to use it
	
	pid_t child1 = fork(); 
	
	if (child1 < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(child1 > 0)
	{
		//second pipe
		int child2Pipe[2];
		if (pipe(child2Pipe) == -1) 
		{
			perror("Hiba a pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}
		pid_t child2 = fork();
		if (child2 < 0)
		{
			perror("fork"); 
		}
		else if(child2 > 0)
		{
			//parent
			ParentProc(child1, child2, messageQueue, child1Pipe, child2Pipe, sharedInt, semid);
		}
		else
		{
			//child2
			Child2Proc(messageQueue, child2Pipe, sharedInt, semid);
		}
		
	}
	else
	{
		//child1
		Child1Proc(messageQueue, child1Pipe, sharedInt, semid);
	}
}