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
	const int MAX_LAKCIM = 100;
	char lakcim[MAX_LAKCIM];
	int hova;
	//osztott memoria
	key_t kulcs=ftok(argv[0],1);
	int sh_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
	int *s = shmat(sh_mem_id,NULL,0);
	//szemafor
	char* sem_nev="/kozpont";
	int semid = szemafor_letrehozas(sem_nev,0);
	
	
	int pipefd[2]; // unnamed pipe file descriptor array
	if (argc < 2) {
	   perror("Nincs utas lakcim megadva!");
	   exit(EXIT_FAILURE);
	}
	//setting signals
	struct sigaction sigact;
	sigact.sa_handler=handler; 
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=0; //nothing special behaviour
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
			printf("Utas megerkezett!\n");
			
			read(pipefd[0], lakcim, sizeof(lakcim));
			printf("Utas cime: %s\n", lakcim);
			
			kill(pid,SIGUSR1);
			write(pipefd[1], lakcim, sizeof(lakcim));
			
			write(pipefd[1], &pid2, sizeof(pid2));
			
			sigsuspend(&sigset); //vár visszajelzésre
			
			char taxis_uzenet[100];
			read(pipefd[0], taxis_uzenet, sizeof(taxis_uzenet));
			printf("Taxis irta: %s\n", taxis_uzenet);
					
			szemafor_muvelet(semid,-1); // down
			printf("[Kozpont]Taxis ennyit keresett: %d\n",*s);  
			szemafor_muvelet(semid,1); //up
			shmdt(s);
			
			wait(NULL);
		    szemafor_torles(semid);
			shmctl(sh_mem_id,IPC_RMID,NULL);
		} else{ //utas
			kill(getppid(),SIGUSR1); //waking up parent
			//lakcim = argv[1];
			write(pipefd[1], argv[1],MAX_LAKCIM);
			
			sigsuspend(&sigset); 
			printf("Megjott a taxi! De gyors!\n");
			hova = rand()%20+1;
			write(pipefd[1], &hova, sizeof(hova));
			sigsuspend(&sigset);  //var amig a taxis szamolja az arat
			int ar;
			read(pipefd[0], &ar, sizeof(ar));
			printf("Taxis mondta, ennyibe fog kerulni: %d!\n", ar);
		}	
	} else {    // taxi
		sigsuspend(&sigset); //vár utasra
		read(pipefd[0], lakcim, sizeof(lakcim));
		printf("Taxi kapott cimet: %s\n", lakcim);
		pid_t utas;
		read(pipefd[0], &utas, sizeof(utas));
		printf("Utas pidje megjott: %d!\n", utas);
		
		kill(getppid(),SIGUSR1);
		write(pipefd[1], "Elindultam!", 12);
		sleep(1); //ennyi ido alatt ott is vana  taxi!
		kill(utas,SIGUSR1);
		
		read(pipefd[0], &hova, sizeof(hova));
		printf("Utas mondta, ilyen messze: %d\n", hova);
		
		hova = hova*300+700; //ar szamolasa
		kill(utas,SIGUSR1); //kiszamoltam!
		write(pipefd[1], &hova, sizeof(hova));
		
		*s = hova; //irunk memoba
		szemafor_muvelet(semid,1); //felengedjuk
		shmdt(s);
	}

	return 0;
}


