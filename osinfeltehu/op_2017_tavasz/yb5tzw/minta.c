#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

#define MEMSIZE 1024

struct szavazo{
	int id;
	int szavazhat;
};

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


//signal handler
void handler(int signumber){
	return;
}


int main (int argc,char* argv[]) {
	srand(time(NULL)); //setting up random seed	
	
	int pipefd[2]; // unnamed pipe file descriptor array
	struct szavazo szavazok[100]; //maximum 100 szavazo
	
	//setting signals
	struct sigaction sigact;
	sigact.sa_handler=handler; 
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=0; //nothing special behaviour
	sigaction(SIGUSR1,&sigact,NULL);
	sigaction(SIGUSR2,&sigact,NULL);
	
	if (pipe(pipefd) == -1) {
	   perror("Error occured while opening pipe!");
	   exit(EXIT_FAILURE);
	}
	pid_t pid = fork();	
	if (pid == -1) {
	   perror("Error occured during fork!");
	   exit(EXIT_FAILURE);
	}

	if (pid != 0) {	// parent process
		
		pid_t pid2 = fork();
		if (pid2 == -1) {
		   perror("Error occured during fork!");
		   exit(EXIT_FAILURE);
		}
		if (pid2 != 0){ //parent
			sigset_t sigset;
			sigfillset(&sigset);
			sigdelset(&sigset,SIGUSR1);
			sigdelset(&sigset,SIGUSR2);
		
			sigsuspend(&sigset);
			printf("First signal arrived\n");
			sigsuspend(&sigset);
			printf("Second signal arrived\n");
			//TODO argc check
			int szavazo_c = atoi(argv[1]);
			//TODO < 100 check
			int i;
			szavazok[0].id = rand()%10;
			szavazok[0].szavazhat = 0;
			for (i = 1; i < szavazo_c; i++){
				szavazok[i].id = szavazok[i-1].id + rand()%10;
				szavazok[i].szavazhat = 1;
			}
			
			write(pipefd[1], szavazok,sizeof(szavazok)); //writing
			
			wait(NULL);
		} else{ //child proccess 2	
			sleep(1);
			kill(getppid(),SIGUSR2); //waking up parent
			
			read(pipefd[0], szavazok, sizeof(szavazok));
			//sleep(1);
			printf("%d", szavazok[1].id);
		}	
	} else {    // child process 
		sleep(0.5);
		kill(getppid(),SIGUSR1); //waking up parent
	}

	return 0;
}


