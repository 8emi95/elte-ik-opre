#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <string.h>
#include <fcntl.h>
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

void handler(int signumber)
{
   ;
}

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 1, "Kerdes?" };
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
	status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
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
     status = msgrcv(uzenetsor, &uz, 1024, 3, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
     return 0; 
} 

int main(int argc, char** argv)
{
     int uzenetsor, status; 
     key_t kulcs; 
	 
	  int pipefdf[2]; // unnamed pipe file descriptor array
	  int pipefds[2]; // unnamed pipe file descriptor array
	  
	  char sz1[100];
	  char sz2[100];
	  	  
	  signal(SIGUSR1,handler);
	  signal(SIGUSR2,handler);
	  
	  srand(time(NULL));
	  
    if (pipe(pipefdf) == -1) 
	{
		perror("Hiba az elso pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefds) == -1) 
	{
		perror("Hiba az masodik pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	 
	 kulcs = ftok(argv[0],1); // generalok egy kulcsot
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); //letrehozom az uzenetsort, ehhez kellett a kulcs
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 

	pid_t child1 = fork();
	if(child1 < 0)
	{
		printf("Error.\n");
	}
	else if (child1 > 0)
	{
		pid_t child2 = fork();
		if (child2 < 0)
		{ 
		   printf("Error\n");
		}
		else if (child2 > 0)
		{
			//printf("Parent.\n");
			kuld( uzenetsor );
			
			printf("Eloado: Varom a valaszokat!\n");
			
			int answers = 0;
			int answerf;
			
			close(pipefdf[1]); // Az iro részre nincs szükség azt bezárjuk
			close(pipefds[1]); 
						
			pause();
			read(pipefdf[0], &answerf, sizeof(int));
			printf("Eloado: Az elso hallgato valasza: %i\n", answerf);
			pause();
			read(pipefds[0], &answers, sizeof(int));
			printf("Eloado: A masodik hallgato valasza: %i\n", answers);
			
			close(pipefdf[0]); // Már az olvasó részre sincs szükség ezt is bezárom
			close(pipefds[0]);
			
			waitpid(child1, NULL, 0);
			waitpid(child2, NULL, 0);
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
		   printf("Eloado: Befejeztem\n");
          return 0; 
		   
		}
		else
		{
			//printf("Child 2\n");
			struct uzenet uz; 
			int status; 
			int answer;

			status = msgrcv(uzenetsor, &uz, 1024, 0, 0 ); 
     
			if ( status < 0 ) 
				perror("msgsnd"); 
			else
				printf( "Hallgato2: Az eloado kerdese: %s\n",  uz.mtext );
			answer = rand()%4 +1;
			
			printf( "Hallgato2: Az en valaszom: %d\n",  answer );
			close(pipefds[0]);  //Usually we close the unused write end
			write(pipefds[1], &answer, sizeof(int) );
			close(pipefds[1]);  //Usually we close the unused write end
			kill(getppid(), SIGUSR2);
			sleep(2);
			printf( "Hallgato2: befejeztem\n" );
			return 0; 

		   
		}
	}
	else
	{
		//printf("Child 1\n");
		struct uzenet uz; 
		int status; 
		int answer;

		status = msgrcv(uzenetsor, &uz, 1024, 0, 0 ); 
     
		if ( status < 0 ) 
			perror("msgsnd"); 
		else
			printf( "Hallgato1: Az eloado kerdese: %s\n",  uz.mtext );
		answer = rand()%4 +1;
		printf( "Hallgato1: Az en valaszom: %d\n",  answer );
		close(pipefdf[0]);  //Usually we close the unused write end
		write(pipefdf[1], &answer, sizeof(int) );
		close(pipefdf[1]);  //Usually we close the unused write end
		kill(getppid(), SIGUSR1);
		sleep(2);
		printf( "Hallgato1: befejeztem\n" );
		return 0; 
			
	}
  return 0;
}

/*
int msgrcv(int msgid, void *addr, size_t nbytes, long msgtype, int flg);
	msgtype = 0  –  a sor elsõ üzenetét olvassa ki,
	msgtype > 0  –  a sor elsõ msgtype típusú üzenetét olvassa ki,
	msgtype < 0  –  a sor legkisebb, de az abs(msgtype)-nál nagyobb vagy azzal egyenlõ értékû üzenetét olvassa ki.
*/

/*read-elni a [0]-n write-olni az [1]n*/