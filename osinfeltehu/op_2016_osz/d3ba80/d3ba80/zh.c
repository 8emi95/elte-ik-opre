#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <time.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 
 
int kuld( int uzenetsor, int szerelveny ) 
{ 
     struct uzenet uz = { 5, "" }; 
   	 sprintf(uz.mtext,"%d",szerelveny);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

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
	 {
		perror("msgsnd"); 
	 } else {
		//printf( "A kapott sorszam:  %s\n", uz.mtext ); 
		int sorszam;
		sscanf (uz.mtext,"%d",&sorszam);
		return sorszam;
	 }      

     return -1;
}
int szerelveny_befejezte( int uzenetsor, int siker, int sorsz ) 
{ 
     struct uzenet uz = { 3, "" }; 
     int status; 
     if (siker == 1){
		sprintf(uz.mtext,"\n%d-es kocsi sikeres volt!", sorsz);
	 } else {
		sprintf(uz.mtext,"\n%d-es kocsi sikertelen volt!", sorsz);
	 }
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
int fogad_szerelveny_vege( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 3, 0 ); 
     if ( status < 0 )
	 {
		perror("msgsnd"); 
	 } else {
		printf( "%s\n", uz.mtext );
	 }      

     return 0;
}

static int szerelveny_vege = 0;
static void handleSignal(int signumber)
{
	if (signumber == SIGTERM)
	{
		szerelveny_vege = 1;
	}
}

int main(int argc, char* argv[])
{
  pid_t child1; 
  int uzenetsor, status;
  key_t kulcs; 
	 
  kulcs = ftok(argv[0],1);
  uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
  if ( uzenetsor < 0 ) { 
	perror("msgget"); 
	return 1; 
  }
  	

	
  srand(time(NULL));
  int max;
  sscanf (argv[1],"%d",&max);

  child1 = fork();
  if (child1 < 0)
  {
    perror("The fork calling was not succesful\n");
    exit(1);
  }
  
  signal(SIGTERM, handleSignal);
  if (child1 == 0)
  {
    //printf("I am the first child\n");
	sleep(1);
		kill(getppid(),SIGTERM);

	int sorszam = fogad(uzenetsor);
	printf("En vagyok a %d . kocsi!\n", sorszam);
	printf("Meg %d, kocsi van mogottem!\n",max-sorszam);
	int sikerese = 0;
	if(rand() % 10 < 1)
	{
		sikerese = 0;
		printf("Nem sikerult befejeznem a kuldetesem elcsusztam egy bananhejon!");
	} else {
		sikerese = 1;
		printf("Sikerult befejeznem a kuldetesem!");
	}
	sleep(1);
	szerelveny_befejezte(uzenetsor,sikerese,sorszam);
	exit(EXIT_SUCCESS);
  } else {
	//printf("I am the parent\n");
	int szerelveny = rand() % max + 1;
	
	kuld( uzenetsor, szerelveny );
	wait(NULL);
	
	
	//pause();
	if(szerelveny_vege);
	{
		printf("\nSzerelveny vissza tert");
		fogad_szerelveny_vege(uzenetsor);
	}
	//waitpid(child1,&status,0);
	int status = msgctl( uzenetsor, IPC_RMID, NULL );
  }

}