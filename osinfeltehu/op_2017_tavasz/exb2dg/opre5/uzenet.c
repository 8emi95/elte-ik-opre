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

char*lakcim="Pazmany Peter Setany";
telefon=6035128496;

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { telefon, lakcim }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
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
          printf( "A lakcim: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int secondChild(int uzenetsor) {
	printf("[Auto] Starting..(%d)\n", getpid());
	
	fogad( uzenetsor ); 
}
int main (int argc, char **argv) { 
	printf("-----------------------------\n");
	pid_t pids[2]; 
	int pid;
	

	
	int pipefd[2];
    char* sz;
	if (pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
	int uzenetsor, status; 
     key_t kulcs; 
     kulcs = ftok(argv[0],1); 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     }
	
		   
	int i;
	for (i = 0; i < 2; ++i) {
		pid = fork();
		if (pid < 0) {
			perror("fork");
			abort();
		} 
		else if (pid == 0) {
			printf("[Utas] Starting..(%d)\n", getpid());
			sleep(3);
			if (i==0) {
				//firstChild();
				char* lakcim = argv[1];
				
				   close(pipefd[0]); //Usually we close unused read end
					write(pipefd[1], "",13);
				   close(pipefd[1]); // Closing write descriptor 
					sleep(3);
				   fflush(NULL); 	// flushes all write buffers (not necessary)
			}			
			if(i==1) {	
				secondChild(uzenetsor);
			}
			break;			
		} 	
		else {
			pids[i] = pid;
		}		
	}
	
	if (pid > 0) {
		//parent
		printf("[Kozpont] Starting..(%d)\n", getpid());

		//parentProc(sz);
				sleep(3);	// sleeping a few seconds, not necessary
				close(pipefd[1]);  //Usually we close the unused write end
				sleep(1);
				read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
				printf("[Utas] Kuldom a lakcimet.\n");
				printf("[Kozpont] Megvan a lakcim: Pazmany Peter setany");
				printf("\n");
				close(pipefd[0]); // finally we close the used read end
				
				
			kuld( uzenetsor );  // Parent sends a message. 
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 

			/*while (n < 2) {
				pid = wait(&status);
				printf("[Kozpont] Az utas  kiszalt.\n");
				++n;
			}*/
			printf("[Kozpont] Befejeztem a programot.\n");
			printf("-----------------------------\n");
			}
	exit(EXIT_SUCCESS);	// force exit, not necessary
}