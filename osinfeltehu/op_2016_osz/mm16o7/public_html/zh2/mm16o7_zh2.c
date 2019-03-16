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

#define MAX_SIZE 30

int kiszor()
{
  int t = rand()%30 +20;
  return t;
}

struct uzenet { 
     long mtype;
     char mtext[1024];
}; 

void handler(int x) { x = x; }

int kuld( int uzenetsor, struct uzenet msg ) 
{  
     int status; 
     
     status = msgsnd( uzenetsor, &msg, sizeof(struct uzenet ) , 0 ); 
	
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
      
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status, jatekdb; 
     
     status = msgrcv(uzenetsor, &uz, sizeof(struct uzenet), 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
		  sscanf(uz.mtext, "%d", &jatekdb);
          printf( "- betoro hanyatt esik %d db apro jatekon -\n", jatekdb); 
     return 0;
} 

int fogad2( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, sizeof(struct uzenet), 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Betoro mondja:  %s\n", uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) 
{ 
  pid_t kevin,betoro; 
  int uzenetsor, status; 
  key_t kulcs; 
  struct sigaction sigact;

  sigact.sa_handler=handler;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags=0;
  sigaction(SIGUSR1,&sigact,NULL);
   
  kulcs = ftok(argv[0],1); 
  //printf ("A kulcs: %d\n",kulcs);
  uzenetsor = msgget(IPC_PRIVATE, 0600 | IPC_CREAT ); 
  if ( uzenetsor < 0 ) { 
	perror("msgget"); 
    return 1; 
  } 
     
  srand(time(NULL)); 
  kevin = fork(); 
  
  if ( kevin > 0 ) //PARENT
  {
	
	betoro = fork();
	if (betoro > 0)	//PARENT
	{
		waitpid(betoro,0,0);
		waitpid(kevin,0,0);
	} 
	else if (betoro == 0)	//BETORO
	{
		fogad(uzenetsor);
		
		struct uzenet msg2;
		msg2.mtype = 5;
		sprintf(msg2.mtext,"%s", "csak varj amig a kezunk koze nem kerulsz!" );
		kuld(uzenetsor, msg2);
		
		exit(EXIT_SUCCESS);
		
	}
	else 
	{ 
		perror("fork2"); 
		return 1; 
	} 
	
    return 0; 
  }
  else if ( kevin == 0 )	//KEVIN 
  { 
	sleep(2);
	int rnd = kiszor();
	struct uzenet msg;
	msg.mtype = 5;
	sprintf(msg.mtext,"%d", rnd );
	//printf("kevin kuldes elott ok\n");
	kuld(uzenetsor, msg);
	//printf("kevin kuldes utan ok\n");
	
	sleep(1);
	fogad2(uzenetsor);
	
    exit(EXIT_SUCCESS);       
  }
  else 
  { 
    perror("fork"); 
    return 1; 
  } 
     
  return 0;  
}
