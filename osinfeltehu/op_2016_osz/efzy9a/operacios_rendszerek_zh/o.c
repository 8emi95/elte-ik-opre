#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
 
#include <wait.h> 

void handler(int signumber){
	//printf("Signal with number %i has arrived\n",signumber);
}

struct uzenet {
	//long mytype;
    int sorszam;
	int ossz_szerelvenyszam;
};



// sendig a message
int kuld( int uzenetsor, pid_t pid_szam, const struct uzenet* uz){ 
     //const struct uzenet uz = { pid_szam, "Hajra Fradi!" }; 
     int status; 
     printf("++");
	 
	 
	 
     status = msgsnd( uzenetsor, uz, sizeof(struct uzenet) + 1, 0 ); 
	 printf("==%i\n", uz->sorszam);
	 printf("==%i\n", uz->ossz_szerelvenyszam);
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}



// receiving a message.
struct uzenet* fogad( int uzenetsor){//, struct uzenet* uz){ 
     static struct uzenet* uz; 
     int status; 
    // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
	//sleep(3);
     status = msgrcv(uzenetsor, uz, sizeof(struct uzenet), getpid(), 0 ); 
	printf("fogad..\n");
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     //else
         printf( "Kapott uzenet tartalma: %i, %i",
			uz->sorszam, uz->ossz_szerelvenyszam);
	return uz;
}


int main(int numOfParams, char* params[]){
	//params[0] - max szerelvenyszam
	
	if(numOfParams < 2 || numOfParams > 2){
		printf("Nincs megadva maximalis szerelvenyszam\n");
		return 2;
	}
	
	printf("Budapest Metro Funs (BMF)\n");
	
	// uzenetsor letrehozasakor\n
	int uzenetsor, status; 
	 key_t kulcs; 
	 
	 // msgget needs a key, amelyet az ftok generál 
	 //. 
	 kulcs = ftok(params[0],1); 
	 printf ("A kulcs: %d\n",kulcs);
	 uzenetsor = msgget( /*kulcs*/IPC_PRIVATE, 0600 | IPC_CREAT ); 
	 if ( uzenetsor < 0 ) { 
		  perror("msgget"); 
		  return 1; 
	 }
	
	// valoszinuseg kezeles
	int skipped = 0;
	time_t t;
	srand((unsigned) time(&t));

	float vals = 0.1f;
	int hibas_esetek_szama;
	
	
	pid_t child = fork();
	
	if(child < 0){
		printf("Hiba a gyerek letrehozasakor\n");
		return 1;
	}
	
	signal(SIGTERM,handler);

	
	if(child > 0){// szulo
		// sorszam kuldese
		struct uzenet* uzenet = malloc(sizeof(struct uzenet));
		time_t t;
		srand((unsigned) time(&t));
		uzenet->sorszam = (int)(rand() % atoi(params[1])) + 1;
		uzenet->ossz_szerelvenyszam = atoi(params[1]);
	printf("!!!!!\n");
		
		printf("/// %i\n", uzenet->sorszam);
		printf("/// %i\n", uzenet->ossz_szerelvenyszam);
		
		//sleep(3);
		int hibas_esetek_szama = (int)(uzenet->ossz_szerelvenyszam * vals);
		//wait(NULL);
		kuld(uzenetsor, child, uzenet);
		
		// allomasuzenet varasa
		pause();
		
		
		
		//status = msgctl( uzenetsor, IPC_RMID, NULL ); 
	}
	else if(child == 0){// gyerek
		struct uzenet* kapott_uzenet = malloc(sizeof(struct uzenet));
		//sleep(3);
		kapott_uzenet = fogad(uzenetsor);
		
		printf("%i szerelveny\n", kapott_uzenet->sorszam);
		printf("%i szerelveny lesz ezutan.\n",
			(kapott_uzenet->ossz_szerelvenyszam - kapott_uzenet->sorszam));
		int hibas = 0;
		if(hibas_esetek_szama >=
			(kapott_uzenet->ossz_szerelvenyszam - kapott_uzenet->sorszam)){
			hibas = 0;
		}else if(hibas_esetek_szama > 0){
			hibas = (rand() % 2);
			--hibas_esetek_szama;
		}else{
			// --
		}
			
		if(hibas == 0){
			printf("A szerelveny sikertelenul teljesitette az uzemet.\n");		
			
		}
		else{
			printf("A szerelveny sikeresen teljesitette az uzemet.\n");
		}
		
		sleep(3);
		kill(getppid(),SIGTERM);
		
		
		
	}

	return 0;
}