#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Meg lesz-e a ZH?" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor , char * text) 
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
		strcpy( text, uz.mtext );
        //printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
}

int main(int argc,char** argv){	
	pid_t pids[2];
	int i;
	int n = 2;
	
	int pipefd[2]; // unnamed pipe file descriptor array
    int sz;  // char array for reading from pipe
	
	/** Uzenetsor **/
	int uzenetsor; 
	key_t kulcs; 
	
	kulcs = ftok(argv[0],1); 
	
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

	/* Start children. */
	for (i = 0; i < n; ++i) {
	  if ((pids[i] = fork()) < 0) {
		perror("fork");
		abort();
	  } else if (pids[i] == 0) { // Child process
		//printf("Start of child process...\n");
		char text[100];
		fogad( uzenetsor , text );
		printf("%s\n" , text);
		
		srand(time(NULL));
		int response = (rand() % 4) + 1;
		
		close(pipefd[0]); //Usually we close unused read end
        write(pipefd[1], &response,sizeof(int));
        close(pipefd[1]); 
		
		sleep(1);
		exit(0);
	  }else{ // Parent process
		//printf("Parent process\n");
		kuld( uzenetsor );
		//wait( NULL );
		close(pipefd[1]);  
        read(pipefd[0],&sz,sizeof(sz)); 
        printf("Diakok valasza: %i\n",sz);
        close(pipefd[0]);
		wait( NULL );
	  }
	}

	/* Wait for children to exit. */
	int status;
	pid_t pid;
	while (n > 0) {
	  pid = wait(&status);
	  //printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
	  --n;  // TODO(pts): Remove pid from the pids array.
	}
}
