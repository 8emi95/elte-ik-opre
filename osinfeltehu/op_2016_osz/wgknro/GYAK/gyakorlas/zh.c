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
     long mtype;
     char mtext [ 1024 ]; 
}; 

void handler(int signumber)
{
   ;
}

// sendig a message
/*int kuld( int uzenetsor ) 

//elküldi a szulo hogy hanyadik a gyerek, és mennyi az összes szerelvenyszam
//elkuldi a gyerek hogy beert rendesen, vagy 10%
{ 
     const struct uzenet uz = { 1, "Kerdes?" };
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
	status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, 3, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
     return 0; 
} */

int main(int argc, char** argv)
{
     int uzenetsor, status; 
     key_t kulcs; 
	 
	  int pipefdf[2]; // unnamed pipe file descriptor array
	  
	  char sz1[100];
	  char sz2[100];
	  	  
	  signal(SIGUSR1,handler);
	  signal(SIGUSR2,handler);
	  
	  srand(time(NULL));
	  
    if (pipe(pipefdf) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
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
	if (child1 == 0) //CHILD
	{
			//printf("Parent.\n");
			
			
			//kuld( uzenetsor );
			
			//printf("Eloado: Varom a valaszokat!\n");
			int status;
			struct uzenet messageFromParent; 
			status = msgrcv(uzenetsor, &messageFromParent, 1024, 0, 0 ); 
			printf ("uzenet a szulotol : %s\n", messageFromParent.mtext);
			
			/*int answers = 0;
			int answerf;*/
			
			
			/*close(pipefdf[1]); // Az iro részre nincs szükség azt bezárjuk
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
          return 0; */
	}
	else //szulo
	{
		//printf("Child 1\n");
		struct uzenet uz; 
		int answer;
		
		int status;
		int all = atoi(argv[0]);
			
		char num[80];
		sprintf(num, "%d", all);
		const struct uzenet numMessage = { 1, *num }; 
		status = msgsnd( uzenetsor, &numMessage, sizeof(numMessage.mtext), 0 );
		printf("elkuldtem a szamot");

		//status = msgrcv(uzenetsor, &uz, 1024, 0, 0 ); 
     
		if ( status < 0 ) 
			perror("msgsnd"); 
		/*else
			printf( "Hallgato1: Az eloado kerdese: %s\n",  uz.mtext );*/
		/*answer = rand()%4 +1;
		printf( "Hallgato1: Az en valaszom: %d\n",  answer );
		close(pipefdf[0]);  //Usually we close the unused write end
		write(pipefdf[1], &answer, sizeof(int) );
		close(pipefdf[1]);  //Usually we close the unused write end
		kill(getppid(), SIGUSR1);
		sleep(2);
		printf( "Hallgato1: befejeztem\n" );
		return 0; */
			
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