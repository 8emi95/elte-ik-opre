#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>

#define MAX_CHILDREN 2
#define CHILDCOUNT 2 //you can use a number from args[] instead just dont forget to pass it to functions
#define SPECIAL 1
#define ERR_ARG "Please pass the address as an argument"
#define ERR_PIPE "Error opening pipe"
#define ERR_WRITING "Error writing to pipe"
#define ERR_READING "Error reading from pipe"
#define proc_owner "Taxi organization"
#define proc_owner2 "Client"
#define proc_owner3 "Cab driver"

void handler(int signumber) {}
void handler2(int signumber) {}

int szemafor_letrehozas(const char* pathname,int szemafor_ertek){
    int semid;
    key_t kulcs;
    
    kulcs=ftok(pathname,1);    
    if((semid=semget(kulcs,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if(semctl(semid,0,SETVAL,szemafor_ertek)<0)    //0= first semaphores
        perror("semctl");
       
    return semid;
}

void szemafor_muvelet(int semid, int op){
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
    muvelet.sem_op  = op; // op=1 up, op=-1 down 
    muvelet.sem_flg = 0;
    
    if(semop(semid,&muvelet,1)<0) // 1 number of sem. operations
        perror("semop");	    
}

void szemafor_torles(int semid){
      semctl(semid,0,IPC_RMID);
}

int main (int argc, char **argv) 
{
	int semid = szemafor_letrehozas(argv[0],0);
	int pipefd[2];
    pid_t parentID = getpid();
	srand(parentID);
	pid_t pids[MAX_CHILDREN]; 
	
	char* mqname = "/msgq";
	struct mq_attr attr;
	mqd_t mq;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = 500;
	
	key_t k = ftok(argv[0],1);
    int shrd_mem_id = shmget(k,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    char *s;
	s = shmat(shrd_mem_id,NULL,0);
	strcpy(s, "");
	
	if (argc < 2)
	{
		perror(ERR_ARG);
		exit(1);
	}
	
	mq_unlink(mqname);
    mq = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    if (mq < 0)
    {
        printf("Error opening message queue: %d\n", errno);
        return 1;
    }
	if (pipe(pipefd) == -1)
    {
        perror(ERR_PIPE);
        exit(1);
    }
	
	int i;
	int pid;
	
	signal(SIGUSR1, handler);
	signal(SIGTERM, handler2);

	for (i = 0; i < CHILDCOUNT; ++i) 
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			abort();
		} 
		else if (pid == 0)
		{
			// Client
			if (i != SPECIAL) //we can start more than one type of child proc
			{
				printf("[CHILD] Starting..(%d)\n", getpid());
				char address[500] = "";
				int j;
				for (j = 1; j < argc; j++)
				{
					strcat(address, argv[j]);
					if (j != argc - 1)
						strcat(address, " ");
				}
				
				if (write(pipefd[1], &address, sizeof(address)) < 0)
				{
					perror(ERR_WRITING);
					exit(1);
				}
				
				pause(); // Wait for the taxi
				int distance = rand() % 20 + 1;
				
				printf("%s: The distance I wish to take: %i\n", proc_owner2, distance);
				// union sigval s_value_int = {distance};
				// sigqueue(pids[1], SIGTERM, s_value_int); 
				
				
				kill(pids[1], SIGUSR1);
				
				printf("[CHILD] Finished.\n");	
				close(pipefd[1]);
				close(pipefd[0]);
			}
			// Taxi
			else 
			{	
				printf("[CHILD] Taxi starting..(%d)\n", getpid());
				
				char buffer[1000];
				int db = mq_receive(mq, buffer, 1000, 0);
				printf("%s: Received: %s\n", proc_owner3, buffer);
				
				strcpy(buffer, "I'll go to the client's location...");
				db = mq_send(mq, buffer, strlen(buffer) + 1, 5);
				
				printf("%s: %s\n", proc_owner3, buffer);
				
				// kill(parentID, SIGUSR1);
				
				// Arrive at the location
				kill(pids[0], SIGUSR1);
				
				printf("Waiting...\n");	
				usleep(3000); // Wait for the distance
				
				int distance = rand() % 20 + 1;
				distance = distance * 300 + 700;
				printf("%s: Received distance: %i\n", proc_owner3, distance);
				
				sprintf(buffer, "%i", distance);
				strcpy(s, buffer);
				szemafor_muvelet(semid, 1);
				
				kill(parentID, SIGUSR1);
				printf("[CHILD] Taxi process finised.\n");
			}
			break;
		} 	
		else
		{
			pids[i] = pid;
		}
	}
	
	if (pid > 0) 
	{
		// Taxi organization
		printf("[PARENT] Starting..(%d)\n", getpid());
		
		char recvaddr[500];
		if (read(pipefd[0], &recvaddr, sizeof(recvaddr)) < 0)
		{
			perror(ERR_READING);
			exit(1);
		}
		close(pipefd[1]);
		close(pipefd[0]);
		printf("%s: The address of the client is: %s\n", proc_owner, recvaddr);
		
		char buffer[1000] = "";
		char buffer2[1000] = "";
		strcpy(buffer, recvaddr);
		sprintf(buffer2, ", %i", pids[0]);
		strcat(buffer, buffer2);
		
		int db = mq_send(mq, buffer, strlen(buffer) + 1, 5);
		
		printf("%s: The address and the process ID of the client is: %s\n", proc_owner, buffer);
		
		pause();
		db = mq_receive(mq, buffer, 1000, 0);
		printf("%s received this message: %s\n", proc_owner, buffer);
		
		// Wait for children
		int n = 0;
		int status;
		pid_t pid;
		
		pause();
		szemafor_muvelet(semid, -1);
		printf("The price of the trip: %s\n", s);
		szemafor_muvelet(semid, 1);
		while (n < CHILDCOUNT) 
		{
			pid = wait(&status);
			++n;  // TODO(pts): Remove pid from the pids array.
		}	
		
		printf("[PARENT] Finished waiting for child processes.\n");
	}
	
	shmdt(s);
}