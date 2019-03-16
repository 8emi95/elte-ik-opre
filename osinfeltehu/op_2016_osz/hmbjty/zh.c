#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <time.h>

void handler(int signumber){
  printf("Signal fogadva a gyerektol, a szama: %i \n",signumber);
}

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     int mtext; //hanyadik szerelveny
     int mMax;	//max szerelvenyek
};

struct uzenet2 { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
};

// sendig a message
int kuld( int uzenetsor, int max ) 
{ 

	srand(time(NULL));
	int r = rand() % max + 1;

     const struct uzenet uz = { 5, r, max };
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof( uz ) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
 	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int kuld2( int uzenetsor ) 
{ 
     const struct uzenet2 uz = { 5, "Az ok" }; 
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
     status = msgrcv(uzenetsor, &uz, sizeof( uz ), 5, 0 ); 

     srand(time(NULL));
	 int r = rand() % 10 + 1;
	 char* str;
	 //printf("random: %i\n", r);
	 if(r == 1){
	 	str = "Sikertelen";
	 }
	 else{
	 	str = "Sikeres";
	 }
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, A sorszamom:  %i, Utanam meg ennyi szerelveny jon: %i, Uzem: %s \n", uz.mtype, uz.mtext, uz.mMax - uz.mtext, str ); 
     return 0; 
}

int fogad2( int uzenetsor ) 
{ 
     struct uzenet2 uz; 
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

	signal(SIGTERM,handler);

	if(argc < 2){
		perror("arg"); 
          return 1;
	}

	int max = atoi(argv[1]);

     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     //printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor, max );  // Parent sends a message. 
          pause(); //waits till a signal arrive 
          fogad2( uzenetsor );
          printf("Gyerektol a jelzes megjott. \n");
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          fogad( uzenetsor );
          kuld2( uzenetsor );
          sleep(3);
    	  kill(getppid(),SIGTERM);
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
