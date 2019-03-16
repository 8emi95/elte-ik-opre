#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <wait.h> 
#include <sys/shm.h>
#include <sys/stat.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

int parentProc(int uzenetsor, int oszt_mem_id, char *s) {	
	int n=0;
	int status;
	while (n < 3) {
		pause();
		printf("[Budapest] Megkaptam a jelzest.\n");

		++n;
	}
	printf("-----------------------------\n");


	printf("[Budapest] Kuldok egy uzenetet.\n");
	kuld( uzenetsor );  // Parent sends a message. 
        wait( NULL ); 
        status = msgctl( uzenetsor, IPC_RMID, NULL ); 
        if ( status < 0 ) perror("msgctl"); 
	

	char buffer[] = "Osztott memoria \n";                   
       	strcpy(s,buffer);
     	shmdt(s);	   
	wait(NULL); 
	shmctl(oszt_mem_id,IPC_RMID,NULL);

	n=0;
	pid_t pid;
	while (n < 3) {
		pid = wait(&status);
		printf("[Budapest] A fiokiroda PID %ld szammal kilepett.\n", (long)pid);
		++n;
	}		
	printf("[Budapest] Befejeztem a programot.\n");
	printf("-----------------------------\n");
}

int firstChild(int uzenetsor, char *s) {
	printf("[Debrecen] Starting..(%d)\n", getpid());	
	sleep(1);
	printf("-----------------------------\n");
	kill(getppid(),SIGTERM); 
	printf("[Debrecen] A fiokiroda keszen. Kuldom a jelet.\n");
	printf("[Debrecen] "); fogad( uzenetsor ); 
	printf("Osztott: %s",s);
	shmdt(s);
}
int secondChild() {
	printf("[Pecs] Starting..(%d)\n", getpid());
	sleep(2);
	kill(getppid(),SIGTERM); 
	printf("[Pecs] A fiokiroda keszen. Kuldom a jelet.\n");
}
int thirdChild() {
	printf("[Gyor] Starting..(%d)\n", getpid());
	sleep(3);
	kill(getppid(),SIGTERM); 
	printf("[Gyor] A fiokiroda keszen. Kuldom a jelet.\n");
}
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}


int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "2017.05.17" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Megkaptam az uzenetet, nyitasi datum:  %s\n", uz.mtext ); 
     return 0; 
} 

int main (int argc, char **argv) { 
	printf("-----------------------------\n");
	pid_t pids[3]; 
	int pid;

	signal(SIGTERM,handler);


	int uzenetsor, status; 
	key_t kulcs;
	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
   	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
        	return 1; 
	} 

	int oszt_mem_id;
	char *s;
	oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
	s = shmat(oszt_mem_id,NULL,0);

	int i;
	for (i = 0; i < 3; ++i) {
		pid = fork();
		if (pid < 0) {
			perror("fork");
			abort();
		} 
		else if (pid == 0) {			
			if (i==0) {
				firstChild(uzenetsor, s);
			}			
			if(i==1) {	
				secondChild();
			}
			if(i==2) {
				thirdChild();
			}	
			break;			
		} 	
		else {
			pids[i] = pid;
		}		
	}
	
	if (pid > 0) {
		//parent
		printf("[Budapest] Starting..(%d)\n", getpid());

		parentProc(uzenetsor, oszt_mem_id,s);
	}

}