#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CHILDREN 10
#define CHILDCOUNT 2 //you can use a number from args[] instead just dont forget to pass it to functions
#define SPECIAL 1   //utas


int signalFlag = 0;

void handler(int signumber)
{
	if(signumber==2){
		printf("Interrupted by signal!\n");
		exit(-1);
	}else{
   signalFlag = signumber;
	}
}


struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
	 long mpid;
	 //int telosz;
}; 
/*
// sendig a message
int kuld( int uzenetsor ) 
{ 
     
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     
     return 0; 
} */

int main (int argc, char **argv) 
{ 
	
	//you can use the pids array to send signals to every child from the parent
	int pipefd[2];
	pid_t pids[MAX_CHILDREN]; 
	
	if (pipe(pipefd) == -1) 
	{
	perror("Hiba a pipe nyitaskor!");
	exit(EXIT_FAILURE);
	}
	
	
	srand(time(NULL));
	signal(SIGTERM, handler);
	signal(SIGUSR1, handler);
	signal(SIGINT, handler);
	
	//int childCount = 1;
	int i;
	pid_t pid;
	
	int uzenetsor; 
	key_t kulcs; 

	// msgget needs a key, amelyet az ftok generál 
	//. 
	kulcs = ftok(argv[0],1); 
	printf ("A kulcs: %d\n",kulcs);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
	  perror("msgget"); 
	  return 1; 
	} 

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
			//child i
			printf("[CHILD] Starting..(%d)\n", getpid());
			
			
			if (i!=SPECIAL) //we can start more than one type of child proc
			{
				//do anything	
				sleep(4);
				
				struct uzenet uz; 
				struct uzenet uz2;
				int status; 
				// az utolso parameter(0) az uzenet azonositoszama
				// ha az 0, akkor a sor elso uzenetet vesszuk ki
				// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
				// vesszuk ki a sorbol 
				status = msgrcv(uzenetsor, &uz, 2048, 5, 10 ); 

				if ( status < 0 ) 
				  perror("msgsnd"); 
				else
				  printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
			  
				status = msgrcv(uzenetsor, &uz2, 2048, 6, 10 ); 

				if ( status < 0 ) 
				  perror("msgsnd"); 
				else
				  printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz2.mtype, uz2.mpid ); 
				
				struct uzenet uz3;
				uz3.mtype=7;
				strcpy(uz3.mtext,"Megyek az utasert!");
				status = msgsnd( uzenetsor, &uz3, sizeof(uz3.mtext) + 1 , 10 ); 
		// a 3. param ilyen is lehet: sizeof(uz.mtext)
		// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
		if ( status < 0 ) 
		  perror("msgsnd"); 
				
				printf("[CHILD] Finished.\n");	
			}			
			else 
			{	
				//do something different	
				sleep(3);
				
				char lakcim[100];
				
				if(argv[1]==NULL){
					  printf("Nincs megadott argumentum!\n");
					  kill(getppid(),SIGINT);
					  //wait(&status);
					  exit(-1);
					}
				strcpy(lakcim,argv[1]);
				
				//printf("%s\n",lakcim);
				
				kill(getppid(),SIGUSR1);
				write(pipefd[1], lakcim, 100);
				
				
				
				printf("[CHILD] I was special. Now I am nothing.\n");
			}	
			break;	
				
							

			
				
			close(pipefd[0]);
			close(pipefd[1]);
		} 	
		else
		{
			pids[i] = pid;
		}		
	}
	
	if (pid > 0) 
	{
		//parent
		printf("[PARENT] Starting..(%d)\n", getpid());
		
		int n=0;
		int status;
		pid_t pid;
		//int pido=atoi(pid);
		
		char beLakcim[100];
		
		pause();
		read(pipefd[0],beLakcim,100);
		
		printf("Szulo beolvasta az adatot: %s\n",beLakcim);
		
		//const struct uzenet uz = { 5, "beLakcim"}; 
		struct uzenet uz;
		uz.mtype = 5;
		strcpy(uz.mtext,beLakcim);
		//int status; 

		status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) + 1 , 10 ); 
		// a 3. param ilyen is lehet: sizeof(uz.mtext)
		// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
		if ( status < 0 ) 
		  perror("msgsnd"); 
          
		struct uzenet uz2;       // = { 6, pid}; 
		uz2.mtype = 6;
		//uz2.mtext = pid + '\0' ;
		uz2.mpid = pid;
		//strcpy(uz2.mtext,"jani");
		//int status; 

		//printf(" ezaz %d\n",pid + '\0');
		//printf(" ezaz %s\n",pid);
		
		
		status = msgsnd( uzenetsor, &uz2, 2040 , 10 ); 
		// a 3. param ilyen is lehet: sizeof(uz.mtext)
		// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
		if ( status < 0 ) 
		  perror("msgsnd");
		
		struct uzenet uz3;
		status = msgrcv(uzenetsor, &uz3, 2048, 6, 10 ); 

				if ( status < 0 ) 
				  perror("msgsnd"); 
				else
				  printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz3.mtype, uz3.mtext ); 
		
		close(pipefd[0]);
		close(pipefd[1]);
		
		while (n < CHILDCOUNT) 
		{
			pid = wait(&status);
			printf("[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
			++n;  // TODO(pts): Remove pid from the pids array.
		}	
		
		// wait( NULL ); 
		// After terminating child process, the message queue is deleted. 
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
		if ( status < 0 ) 
		perror("msgctl");
	
		printf("[PARENT] Finished waiting for child processes.\n");
		
		
		//parentProc();
		
	}

}