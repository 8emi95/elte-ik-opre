#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <wait.h> 

#define QUESTIONLIMIT 100
#define MAXANSWERS 4
#define MINANSWERS 2

void handler(int signumber){}
void taxihandler(int signumber,siginfo_t* info,void* nonused){
	 switch (info->si_code){ 
    case SI_QUEUE: printf("Az utazas ara: %i\n",(info->si_value.sival_int)*300+700);
                   break;
    default: printf("It was sent by something else \n");
  } 
}

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor ,struct uzenet uz ) 
{ 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 

     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor  ) 
{ 
     struct uzenet uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "%s\n", uz.mtext ); 
     return 0; 
} 


int main(int argc,char ** argv)
{
	if(argc != 2){
		printf("\nNem adta meg az utas cimet.\n");
		return 1;
	}
	
	  struct sigaction sigact;
  
	  sigact.sa_sigaction=taxihandler; //instead of sa_handler, we use the 3 parameter version
	  sigemptyset(&sigact.sa_mask); 
	  sigact.sa_flags=SA_SIGINFO; //we need to set the siginfo flag 
	  sigaction(SIGTERM,&sigact,NULL); 
	  
	
	
	int status;
	signal(SIGRTMAX,handler);
	printf("\nÜdvözöljük ZH taxi társaságnál!\n");
	int fd1;
	unlink("pipe");
    int fid1=mkfifo("pipe", S_IRUSR|S_IWUSR );
	if (fid1==-1){
        printf("Error number: %i",errno); 
        exit(EXIT_FAILURE);
    }	
	srand(time(NULL)); //the starting value of random number generation
	pid_t pid1 = fork();
	pid_t pid2;
	
	if(pid1>0){ //Kozpont
		fd1=open("pipe",O_RDONLY);
		char Adress[100];
		read(fd1,Adress,sizeof(Adress));
		close(fd1);		
		printf("Utas erkezett es a(z) %s cimre kell vinni.\n",argv[1]);	
		
		int uzenetsor;
		key_t kulcs;      
		kulcs = ftok(argv[0],1); 
		uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
		if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
		} 
		pid2 = fork();
		if(pid2>0){//Kozpont
			struct uzenet uz;
			uz.mtype = 5;
			sprintf(uz.mtext,"Cim: %s \nTelefonszam: %i ",Adress,pid2);
			kuld( uzenetsor ,uz );
			
			pause();
			fogad( uzenetsor );

			  status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			  if ( status < 0 ) 
				   perror("msgctl"); 
			  waitpid(pid2,&status,0); 
			  return 0; 
			
		}
		else
		{//Taxi	
			fogad( uzenetsor ); 
			struct uzenet uz;
			uz.mtype = 5;
			strcpy(uz.mtext,"Elindultam fonok! \n");
			kuld( uzenetsor ,uz );	
			kill(getppid(),SIGRTMAX);
			sleep(1);
			printf("Megerkeztem\n");
			kill(pid1,SIGRTMAX);
		}
		waitpid(pid1,&status,0); 
	}
	else
	{	//Utas
		char Adress[100];
		sprintf(Adress,"%s",argv[1]);
		fd1=open("pipe",O_WRONLY);
		write(fd1,Adress,sizeof(Adress));
		close(fd1);	

		pause();
		srand(time(NULL));
		int dist = 1 + (rand() % 20);
		union sigval s_value_int;
	    s_value_int.sival_int=dist;
		sigqueue(getppid(),SIGTERM,s_value_int);
		//printf("Felebredtem\n",SIGRTMAX);
	}
    return 0;
}