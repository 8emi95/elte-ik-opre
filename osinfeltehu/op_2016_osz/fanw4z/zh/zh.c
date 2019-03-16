#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

int trainStopped = 0;

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     int trainNumber; 
	 int trainCount;
	 int isSubstitute;
}; 

struct ok{
	long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
	char reasonString [ 1024 ]; 
	int amIsubstitute;
};

void handler(int signumber)
{
	if(signumber == SIGUSR1)
	{
		printf("(BMF)Szerelveny az allomason, hurra\n");
	}
	else if(signumber == SIGUSR2)
	{
		trainStopped = 1; 
		printf("(BMF)Ajjaj, a szerelvenyt bevontattak\n");
		
	}
}


// sendig a message
int kuld( int uzenetsor, pid_t target, int trainNumber, int trainCount, int isSubstitute ) 
{ 
     const struct uzenet uz = { target, trainNumber, trainCount, isSubstitute}; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.trainNumber) + sizeof(uz.trainCount) + sizeof(uz.isSubstitute), 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
    
     status = msgrcv(uzenetsor, &uz,16, getpid(), 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
	 {
		srand(getpid());
		printf( "(szerelveny)%d szerelvney van \n", uz.trainCount ); 
		printf( "(szerelveny)En az %d. szerelvney vagyok\n", uz.trainNumber); 
		if(uz.isSubstitute != 0)
		{
			printf("(szerelveny)Helyettesito vagyok\n");
		}
		if(rand()%10)
		{
			printf("(szerelveny)Sikeresen teljesitettem a feladatot\n");
			kill(getppid(), SIGUSR1);
		}
		else
		{
			printf("(szerelveny)Bevontattak\n");
			
			kill(getppid(), SIGUSR2);
			const struct ok uz2 = { getppid(), "bananhely", uz.isSubstitute}; 
			int status; 
			printf("(szerelveny)Leallas okanak kuldese\n");
			status = msgsnd( uzenetsor, &uz2, sizeof(uz2.reasonString), 0 ); 
			if ( status < 0 ) 
				  perror("msgsnd"); 
			}
	

	 }
     return 0; 
} 

int createChild(int trainNumber, int trainCount, int uzenetsor, int isSubstitute)
{
	int trCount = 0;
	pid_t child = fork(); 
    if ( child > 0 ) 
	{ 
		kuld( uzenetsor, child, trainNumber, trainCount, isSubstitute);  // Parent sends a message. 
		pause();
		
		if(trainStopped)
		{
			struct ok uz; 
			int status; 
			
			status = msgrcv(uzenetsor, &uz, 1024, getpid(), 0 ); 
			printf("(BMF)Leallas oka: %s\n", uz.reasonString); 
			if ( status < 0 ) 
			  perror("msgsnd"); 
			if(!isSubstitute)
			{
				printf("Inditok helyettesitot!\n");
				trainStopped = 0;
				createChild(trainNumber, trainCount, uzenetsor, 1);
				trCount = 1;
			}
			
		}
		wait( NULL );
		trainStopped = 0;

		return trCount;
		
	 }
	 else if ( child == 0) 
	 { 
        fogad( uzenetsor ); 
		exit(1);
     } else 
	 { 
          perror("fork"); 
          exit(1); 
     } 
}



int main (int argc, char* argv[]) { 
	int uzenetsor, status; 
	key_t kulcs; 

	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    
	
    
    if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
    } 
  signal(SIGUSR1,handler);
  signal(SIGUSR2,handler);
  int trainCount = atoi(argv[1]);
  int i=1;
  int substitutedTrains = 0;
  while(i <= trainCount)
  {
	  substitutedTrains += createChild(i,trainCount, uzenetsor, 0);
	  i++;
  }
  printf("Osszesen %d jaratot helyettesitettek\n", substitutedTrains);  
  
  status = msgctl( uzenetsor, IPC_RMID, NULL ); 
  if ( status < 0 ) 
	   perror("msgctl"); 
  return 0; 
    
} 
