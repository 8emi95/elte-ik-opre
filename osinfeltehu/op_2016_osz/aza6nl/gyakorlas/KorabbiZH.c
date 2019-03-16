#include <unistd.h>
#include <sys/types.h> //pid-ekhez kell
#include <errno.h>
#include <stdio.h>    //Alap
#include <wait.h>	  //varakozáshoz kell
#include <stdlib.h>
#include <signal.h>   //Signalhoz kell

#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h> 
#include <sys/time.h>


void handler(int signumber){
  if(signumber == SIGTERM) printf("Jaror0 bejelentkezett!\n");
  if(signumber == SIGINT) printf("Jaror1 bejelentkezett!\n");
};

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

//struct
int kuld( int uzenetsor ) 
{ 
	int status; 
     
	//Random
	srand(time(NULL));
	int r=rand()%4;
	
	struct uzenet uz2 = { 5, "Hajra Fradi!" };
	
	if(r == 0)
	{
		uz2.mtype = 5; strcpy(uz2.mtext,"Minden jot!");
	}
	if(r == 1)
	{
		uz2.mtype = 5;strcpy(uz2.mtext,"Jo Munkat!");
	}
	if(r == 2)
	{
		uz2.mtype = 5; strcpy(uz2.mtext,"Hosszu es eredmenyes eletet!");
	}
	if(r == 3)
	{
		uz2.mtype = 5; strcpy(uz2.mtext,"Hosszu, hasznos eletet!");
	}
	if(r == 4)
	{
		uz2.mtype = 5; strcpy(uz2.mtext,"Live long and prosper!");
	}

	status = msgsnd( uzenetsor, &uz2, strlen ( uz2.mtext ) + 1 , 0 ); 
	if ( status < 0 ) perror("msgsnd"); 
	return 0; 
} ;
     
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 )
	 {
		perror("msgsnd");
	 }	
     else 
	 {
		 printf( "A kapott uzenet: %s\n", uz.mtext );
	 }
     return 0; 
} 

int main(int argc, char* argv[])
{
    pid_t jaror0;
	pid_t jaror1;
	
	/*/Cső
	int azEnCsovem[2], nbytes;
	char    readbuffer[80];                 
	pipe(azEnCsovem);*/
	
	//Üzenet
	int uzenetsor, status;
	key_t kulcs; 
		//Kulcs
		kulcs = ftok(argv[0],1); 
		printf ("A kulcs: %d\n",kulcs);
		uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
		if ( uzenetsor < 0 ) { 
		  perror("msgget"); 
		  return 1; 
		} 
	
	
	//SIGNAL
	signal(SIGTERM,handler);
	signal(SIGINT,handler);
	
	//innen mar csak a gyerekek vagy a szulo fog futni
    jaror0 = fork();
	jaror1 = fork();
	
    if((jaror0 >= 0) && (jaror1 >= 0))
    {
		
		//Járőr0
		if((jaror0 == 0) && (jaror1 > 0))
		{
			//Reggel vár és jelentkezik SIGINT-el
			sleep(1);
			kill(getppid(),SIGTERM); 
			return fogad( uzenetsor ); 
			printf("Jaror0 leall!\n");
			//Járőr leáll
			exit(0);
		}
		
		//Járőr1
		if((jaror0 > 0) && (jaror1 == 0))
		{
			//Reggel vár és jelentkezik SIGKILL-el
			sleep(2);
			kill(getppid(),SIGINT); 
			return fogad( uzenetsor ); 
			printf("Jaror1 leall!\n");
			//Járőr leáll
			exit(0);
		}
		
		//Központ
		if((jaror0 > 0) && (jaror1 > 0))
		{
			printf("Kozpont elindult!\n");
			int status;
			
			pause(); //Várunk, hogy jelentkezzenek!
			
			sleep(1);
			//Üdvözöljük!
			
			kuld( uzenetsor);
			wait( NULL );
			
			kuld( uzenetsor );
			wait( NULL );
			
			status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			if ( status < 0 ) perror("msgctl"); 
			
			//Megvárjuk amíg leállnak!
			waitpid(jaror0,&status,0);
			waitpid(jaror1,&status,0);
			printf("Kozpont leallt!\n");
		}
    }
    else
    {
		perror("fork-error\n");
		exit(0);
    };
	
    return 0;
}

