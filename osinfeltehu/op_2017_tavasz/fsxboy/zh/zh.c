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

int szemafor_letrehozas(const char* ut,int szemafor_ertek){
	int semid;
	key_t kulcs;

	kulcs=ftok(ut,1);    
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
	const int MAX_CIM = 500;
	char lakcim[MAX_CIM];
	int tav;
	//osztott memoria
	key_t kulcs=ftok(argv[0],1);
	int sh_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
	int *s = shmat(sh_mem_id,NULL,0);

	//szemafor
	char* sem_nev="/kozpont";
	int semid = szemafor_letrehozas(sem_nev,0);

	int pipefd[2]; // unnamed pipe file descriptor array
	if (argc < 2) {
	   perror("Nincs lakcim!");
	   exit(EXIT_FAILURE);
	}
	//setting signals
	struct sigaction sigact;
	sigact.sa_handler=handler; 
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=0; 
	sigaction(SIGUSR1,&sigact,NULL);
	sigaction(SIGUSR2,&sigact,NULL);
	
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset,SIGUSR1);
	
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
			sigsuspend(&sigset);

			printf("-------\n");
			printf("Utas megerkezett!\n");			
			printf("-------\n");

			read(pipefd[0], lakcim, sizeof(lakcim));

			printf("-------\n");
			printf("Utas cime: %s\n", lakcim);
			printf("-------\n");
			
			kill(pid,SIGUSR1);
			write(pipefd[1], lakcim, sizeof(lakcim));
			
			write(pipefd[1], &pid2, sizeof(pid2));
			
			sigsuspend(&sigset);
			
			char taxis_uzenet[100];
			read(pipefd[0], taxis_uzenet, sizeof(taxis_uzenet));

			printf("-------\n");
			printf("Taxis: %s\n", taxis_uzenet);
			printf("-------\n");
					
			szemafor_muvelet(semid,-1); // down
			szemafor_muvelet(semid,1); //up
			shmdt(s);
			
			wait(NULL);
		    	szemafor_torles(semid);
			shmctl(sh_mem_id,IPC_RMID,NULL);
		} else{ //utas
			kill(getppid(),SIGUSR1); //szulo ebresztese
			write(pipefd[1], argv[1],MAX_CIM);
			sigsuspend(&sigset);

			printf("-------\n");
			printf("Taxi megerkezett\n");
			printf("-------\n");

			tav= rand()%20+1;
			write(pipefd[1], &tav, sizeof(tav));
			sigsuspend(&sigset);  			
			int ar;
			read(pipefd[0], &ar, sizeof(ar));

			printf("-------\n");
			printf("Taxis penze: %d!\n", ar);
			printf("-------\n");
		}	
	} else {    // taxi
		sigsuspend(&sigset); //vár utasra
		read(pipefd[0], lakcim, sizeof(lakcim));

		printf("-------\n");
		printf("Taxi megkapta a cimet");
		printf("-------\n");

		pid_t utas;
		read(pipefd[0], &utas, sizeof(utas));
		kill(getppid(),SIGUSR1);
		write(pipefd[1], "Indulok!", 12);
		sleep(1);
		kill(utas,SIGUSR1);
		read(pipefd[0], &tav, sizeof(tav));

		printf("-------\n");
		printf("Utas tavolsaga: %d\n", tav);
		printf("-------\n");
		
		tav= tav*300+700; 
		kill(utas,SIGUSR1); 	
		write(pipefd[1], &tav, sizeof(tav));
		*s = tav;
		szemafor_muvelet(semid,1); 
		shmdt(s);
	}

	return 0;
}


