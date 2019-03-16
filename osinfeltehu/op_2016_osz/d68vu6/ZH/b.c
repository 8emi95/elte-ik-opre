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
	 int id;
     char ms1[1024]; 
	 char ms2[1024];
	 int usage;
}; 

// sendig a message
int kuld( int uzenetsor, int id , int us) 
{ 
	printf("Kuldes...\n");
     const struct uzenet uz = { 1, id, "Azonosito: " , "Sz.szam: ", us};
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
		
     if ( status < 0 ) 
          perror("msgsnd"); 
	  printf("Elkuldve.\n");
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
	printf("Fogadas...\n");
     struct uzenet uz; 
     int status; 
	 int wsl;
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, sizeof(uz), 1, 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
  printf( "A kapott uzenet kodja: %ld, szovege:  %s %d , %s %d\n", uz.mtype, uz.ms1, uz.id, uz.ms2, uz.usage);

		srand(time(NULL));
		wsl = rand() % 11 + 1 ;
		printf("wsl: %d \n", wsl);
		if(wsl == 10)
		{
			printf("Gyerek: En vagyok a(z) %d. szerelveny, utanam meg lesz %d db, de BEVONTATTAK.", uz.id, uz.usage - uz.id);
			printf("\n");
		}
		else
		{
			printf("Gyerek: En vagyok a(z) %d. szerelveny, utanam meg lesz %d db, es BEERTEM.", uz.id, uz.usage-uz.id);
			printf("\n");
		}
		
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status, capacity, temp; 
     key_t kulcs; 
     
	 printf("Kerem adja meg a maximum szerelvenyszam erteket: \n");
		scanf("%d", &capacity);
	
		srand(time(NULL));
	 
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 )
	 { 
	 
		temp = (rand() % capacity) + 1; 
		
		printf("Elkuldott szerelvenyszam: %d \n", temp);
		
			//printf("argv[0] = %d \n", *argv[0]);
		//	printf("argc = %d \n", argc);
          kuld( uzenetsor, temp, *argv[0] ); // Parent sends a message. 
          wait( NULL ); 
		  printf("Gyerek terminalt.\n");
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     }
	 else if ( child == 0 )
	 { 
          return fogad( uzenetsor ); 
		  
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
