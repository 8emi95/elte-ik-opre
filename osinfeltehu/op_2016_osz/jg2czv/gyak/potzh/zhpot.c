#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h> 

int hangone = 1;
int hangtwo = 1;
struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

void handler(int signumber){
	// sleep(1);
  	printf("Signal with number %i has arrived in %i\n",signumber, getppid());
  	hangone = 0;
}

void handlertwo(int signumber){
	// sleep(1);
  	printf("Signal with number %i has arrived in %i\n",signumber, getppid());
  	hangtwo = 0;
}

// sendig a message
int kuldjatek( int uzenetsor) 
{ 
	srand(time(NULL));
	int jatek = rand() % 50 + 20;
	char str[15];
	sprintf(str, "%d", jatek);
    const struct uzenet uz = { 5, *str}; 
    int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1, 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

// sendig a message
int kuldfenyeget( int uzenetsor) 
{ 
    const struct uzenet uz = { 5, "csak varj, mig a kezunk koze nem kerulsz"}; 
    int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof( uz.mtext ), 0 ); 
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
          printf( "A kapott uzenet kodja: %ld, uzenet:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int main(int argc, char const *argv[])
{

	//int pipefd[2];	// unnamed pipe file descriptor array
	pid_t child;
    //char sz[100];  // char array for reading from pipe
	int uzenetsor, status; 
    key_t kulcs;
	kulcs = ftok(argv[0],1);
	printf ("A kulcs: %d\n",kulcs);
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) 
	{ 
        perror("msgget"); 
        return 1; 
    } 
    signal(SIGUSR1,handler);
	signal(SIGUSR2,handlertwo);
    //int i, cntn = 0, j = 0, rc = 0;
    //int repeat[argc-1];

    child = fork();	// creating parent-child processes
    if (child == -1) 
    {
       	perror("Fork hiba");
       	exit(EXIT_FAILURE);
    }

    if (child > 0) 
    {	
        pid_t betoro = fork();
		if (betoro == -1) 
		{
			perror("Fork hiba");
			exit(EXIT_FAILURE);
		}
		if(betoro > 0)
		{	// parent process
			printf("Szulo elindult otthonrol!\n");
			
			wait( NULL );
			wait( NULL );
			
			printf("Szulo megerkezett haza!\n");
		}else 
		{	// betoro process
			//while(hangone == 1) sleep(1);
			sleep(1);
			printf("Betoro vagyok!\n");
			fogad( uzenetsor );
			printf("Betoro elesett!\n");
			kuldfenyeget( uzenetsor );
			//kill(getppid(), SIGUSR2);
			wait( NULL );
			
		}

    } 
    else
	{	// child process
		printf("Gyerek vagyok jatekot szorok!\n");
        kuldjatek( uzenetsor );
		//kill(getppid(), SIGUSR1);
		//while(hangtwo == 1) sleep(2);
		sleep(2);
		printf("gyerek vagyok\n");
		fogad( uzenetsor );
    }
	return 0;
}