#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int childPid = 0;

static void handler(int signo)
{
    printf("%d jelzes kuldese megtortent! \n", signo);
}

 struct uzenet {
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     int maxszerelveny;
	 int akt_szerelvenyszam;
};

struct vegallomas_uzenet {
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ];
};

// sendig a message
int kuld( int uzenetsor, int maxszerelveny )
{
	 srand(time(NULL));
     int akt_szerelvenyszam = ( rand() % maxszerelveny ) + 1;
	 printf("Generalt szerelveny szam: %d \n", akt_szerelvenyszam);
	 
	 struct uzenet uz = { 1, maxszerelveny, akt_szerelvenyszam };
     int status;

     status = msgsnd( uzenetsor, &uz, 2*sizeof(int) + 1 , 0 );
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
     status = msgrcv(uzenetsor, &uz, 2*sizeof(int) + 1, 1, 0 );

     if ( status < 0 )
          perror("msgsnd");
     else
          printf( "Megkaptam a max szerelveny szamot:  %d\n", uz.maxszerelveny);
		  printf( "En vagyok a %d szamu szerelveny\n", uz.akt_szerelvenyszam);
	      printf( "Utanam jon meg %d szerelveny\n", (uz.maxszerelveny - uz.akt_szerelvenyszam));
     return 0;
}

// sendig a message
int vegallomasrol_kuld( int uzenetsor, int statuskod)
{
	
	 if(statuskod == 0)
	 {
		 const struct vegallomas_uzenet vuz = { 2, "Sikeresen celba ertem!"};
		      int status; 
		status = msgsnd( uzenetsor, &vuz, strlen ( vuz.mtext ) + 1 , 0 );
	      if ( status < 0 )
          perror("msgsnd");
     return 0;

	 }
	 if(statuskod == 1)
	 {
		 const struct vegallomas_uzenet vuz = { 2, "Elcsusztam egy bananhejon! Bevontattak!"};
	     int status; 
		status = msgsnd( uzenetsor, &vuz, strlen ( vuz.mtext ) + 1 , 0 );
	      if ( status < 0 )
          perror("msgsnd");
     return 0;

	}
	 if(statuskod == 2)
	 {
		 const struct vegallomas_uzenet vuz = { 2, "Lefagytam, bevontattak!"};
			int status; 
			status = msgsnd( uzenetsor, &vuz, strlen ( vuz.mtext ) + 1 , 0 );	
	      if ( status < 0 )
          perror("msgsnd");
     return 0;

	}

	return 0;

}

// receiving a message.
int vegallomas_fogad( int uzenetsor )
{
     struct vegallomas_uzenet vuz;
     int status;
     status = msgrcv(uzenetsor, &vuz, 1024, 2, 0);
     printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", vuz.mtype, vuz.mtext );
     return 0;
}

 int main(int argc, char* argv[])
{
	signal(SIGTERM, handler);
	signal(SIGUSR1, handler);
	pid_t child;
    int uzenetsor, status, maxszerelveny, parent;
    key_t kulcs;
	 
	 if( argc == 2 ) {
		maxszerelveny = atoi(argv[1]);
		printf("Max szerelveny: %d \n", maxszerelveny);
	  }
	   else if( argc > 2 ) {
		printf("Too many arguments supplied.\n");
	  }
	   else {
		printf("One argument expected.\n");
	  }
	   
	 kulcs = ftok(argv[0],1);
	 printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
     if ( uzenetsor < 0 ) {
          perror("msgget");
          return 1;
     }
	
    child = fork();
	parent = getpid();
     if ( child > 0 ) {
		 // PARENT
		 childPid = child;
		 
		kuld( uzenetsor, maxszerelveny );  // Parent sends a message.
		wait( NULL );
		// After terminating child process, the message queue is deleted.
		status = msgctl( uzenetsor, IPC_RMID, NULL );
		if ( status < 0 )
		   perror("msgctl");
	   
		wait(0);
		
		printf("Signal arrived\n", SIGTERM);

		int status;
		wait(&status);
		vegallomas_fogad(uzenetsor);
		printf("Parent process ended\n");
			
     } else if ( child == 0 ) {
		 // CHILD
		     fogad( uzenetsor );
			 int failed = 0;
			 int statuskod = 0;
			 
			 int elcsusztam_egy_bananhejon;
			 srand(time(NULL));
			 elcsusztam_egy_bananhejon = ( rand() % 100 ) + 1;
			 if(elcsusztam_egy_bananhejon <= 10)
			 {
				 printf("Elcsusztam egy bananhejon! :( \n");
				 failed = 1;
				 statuskod = 1;
				 vegallomasrol_kuld(uzenetsor, statuskod);
				 kill(parent, SIGTERM);
				 
			 } 
			 int lefagytam;
			 srand(time(NULL));
			 lefagytam = ( rand() % 100 ) + 1;
			 if(lefagytam >= 10 && lefagytam < 20)
			 {
				 printf("Lefagytam a zord telben! :( \n");
				 failed = 1;
				 statuskod = 2;
				 vegallomasrol_kuld(uzenetsor, statuskod);
				 kill(parent, SIGTERM);
				 
			 }
			 
			 printf("Sikeresen teljesitettem az uzemet!\n");
			 
			 if(failed == 0)
			 {
				 vegallomasrol_kuld(uzenetsor, statuskod);
				 kill(parent, SIGUSR1); // Sikeresen az allomasra ertem
				 
			 }
			 
			
			printf("Child process ended\n");
			
     } else {
          perror("fork");
          return 1;
     }
	 
	return 0;
}