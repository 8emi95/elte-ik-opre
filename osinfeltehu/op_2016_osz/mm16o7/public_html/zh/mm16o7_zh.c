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

int megjott()
{
  int t = rand()%10;
  if (t == 3)
  {
    return 0;
  }
  return 1;
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
     int status, sorsz, maxszerszam; 
     
     status = msgrcv(uzenetsor, &uz, sizeof(struct uzenet), 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
		  sscanf(uz.mtext, "%d %d", &sorsz, &maxszerszam);
          printf( "En ma a %d -dik szerelveny vagyok, meg %d jon utanam!\n", sorsz, maxszerszam-sorsz ); 
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
          printf( "A szerelveny jelentese:  %s\n", uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) 
{ 
  pid_t child; 
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
  child = fork(); 
  
  if ( child > 0 ) //PARENT
  { 
	int maxszerszam = atoi(argv[1]);
	int rnd = rand()%maxszerszam + 1;
	struct uzenet msg;
	msg.mtype = 5;
	sprintf(msg.mtext,"%d %d", rnd, maxszerszam );
	
	
	//printf("parent kuldes elott ok\n");
	kuld( uzenetsor, msg); 
    //printf("parent kuldes utan ok\n");
    /*status = msgctl( uzenetsor, IPC_RMID, NULL ); 
    
    if ( status < 0 )
    { 
		perror("msgctl");
	} */
	
	pause();
	
	
	fogad2(uzenetsor);
	waitpid(child,0,0);
    return 0; 
  }
  else if ( child == 0 )	//CHILD 
  { 
	//printf("child fogadas elott ok\n");
	fogad( uzenetsor ); 
	//printf("child fogadas utan ok\n");
	
	kill(getppid(),SIGUSR1);
	
	struct uzenet msg2;
	msg2.mtype = 5;
	
	if(megjott() == 1)
	{
		printf("a muszakot sikeresen teljesitettem\n");
		sprintf(msg2.mtext,"%s", "beertem" );
	}
	else
	{
		printf("a muszakot nem sikerult vegig csinalnom\n");
		sprintf(msg2.mtext,"%s", "bevontattak" );
	}
	
	sleep(1);
	kuld(uzenetsor, msg2);
	
    exit(EXIT_SUCCESS);       
  }
  else 
  { 
    perror("fork"); 
    return 1; 
  } 
     
  return 0;  
}
