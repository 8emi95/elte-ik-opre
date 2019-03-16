#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <time.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Kerdes" }; 
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
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 

     int uzenetsor, status; 
     key_t kulcs; 
     pid_t childs[2]; 
	 pid_t parentpid=getpid();
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
	 
	 int pipefd[2];
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}	

	 int i;
	 for(i=0;i<2 && getpid() == parentpid;i++){
		childs[i]=fork();
	if (childs[i]<0){perror("The fork calling was not succesful\n"); exit(1);} 
		} //forks make a copy of variables
     
	
     if ( getpid()==parentpid ) { 
		char* kerdes = "Kérdeztem";
		int j;
		for (j=0;j<2;j++){
          kuld( uzenetsor );  // Parent sends a message. 
		}

		wait( NULL ); 
		wait( NULL );
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
		   int osztalyz;
		   
		   close(pipefd[1]);
		 for(j=0;j<2;j++) {
			read(pipefd[0],&osztalyz,sizeof(osztalyz));
			printf("osztalyzat: %d \n",osztalyz);
		}
		close(pipefd[0]);
          return 0; 
		  
     } else 
		 { 
		  
          fogad( uzenetsor ); 
		  printf("kicsit gondolkozom: %d\n",getpid());
		  srand(time(NULL));
		  int osztalyzat = (rand()	% 5 )+ 1;
			close(pipefd[0]);
			write(pipefd[1],&osztalyzat,sizeof(osztalyzat));
			close(pipefd[1]); 
          // The child process receives a message. 
     } 
  
     
     return 0; 
} 
