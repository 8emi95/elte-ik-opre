#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "ezt a kerdest a tanar kuldi" }; 
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
          printf( "A kapott uzenet szovege:  %s\n", uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child, child2; 
     int uzenetsor, uzenetsor2, status; 
     key_t kulcs; 
     
	 int pipefd[2]; // unnamed pipe file descriptor array
     char sz[100];  // char array for reading from pipe
	 
	 int pipefd2[2]; // unnamed pipe file descriptor array
     char sz2[100];  // char array for reading from pipe
	 
     char valasz[1];
	 char valasz2[1]; 
	 srand(time(NULL));
	
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     //printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
	 if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
       }
	   
	   if (pipe(pipefd2) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
       }
//1. gyerek
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor );  // Parent sends a message. 
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
              // 
		   close(pipefd[1]);
            read(pipefd[0], valasz, sizeof(valasz));
            close(pipefd[0]);
			printf("%s\n", valasz);
          return 0; 
     } else if ( child == 0 ) { 
          fogad( uzenetsor ); 
		  int m = (rand() % 4) + 1;
          char c = m + '0';
		  close(pipefd[0]); //Usually we close unused read end
          write(pipefd[1], &c, sizeof(c));
          close(pipefd[1]); // Closing write descriptor 

          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     }
	 child2 = fork(); 
     if ( child2 > 0 ) { 
          kuld( uzenetsor );  // Parent sends a message. 
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
			  //
		   close(pipefd2[1]);
            read(pipefd2[0], valasz2, sizeof(valasz2));
            close(pipefd2[0]);
			printf("%s\n", valasz2);
          return 0; 
     } else if ( child2 == 0 ) { 
          fogad( uzenetsor ); 
		  int m = (rand() % 4) + 1;
          char c = m + '0';
		  close(pipefd2[0]); //Usually we close unused read end
          write(pipefd2[1], &c, sizeof(c));
          close(pipefd2[1]); // Closing write descriptor 

          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     }

     return 0; 
} 