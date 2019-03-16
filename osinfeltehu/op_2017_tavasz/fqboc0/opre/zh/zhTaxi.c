#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

char utasCim[1024];
int pipefd[2];
int uzenetsor;
key_t kulcs;

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     pid_t szam;
	 char lakcim[1024];
}; 

void handler(int signum){printf("SIGNAL\n");}
 
void cab(void)
{
	printf("taxi Start\n");
	
	 struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, sizeof(uz),5,0); 
     
     if ( status < 0 )
	 {
         perror("msgrcv");
		exit(0); 
	 }
   
   printf("az utasom cime:%s telefonszama: %d\n",uz.lakcim,uz.szam);
   /*
   struct uzenet uz2;
   
   uz2.szam=1;
   
   //status = msgsnd( uzenetsor, &uz2, sizeof(uz2),0); 
   
	if ( status < 0 )
	 {
         perror("msgsnd");
		exit(0); 
	 }
	 */
	printf("taxi End\n");
}

void  ChildProcess(void)
{
	printf("utas Start\n");
	int parent=getppid();
	
	write(pipefd[1], utasCim, sizeof(utasCim));
	
	kill(parent,SIGUSR1);
	
	printf("utas end\n");
}

void  ParentProcess(int ChildProcesId)
{
	printf("központ Start\n");
	
	pause();
	
	char fromPipe[1024];
	read(pipefd[0],fromPipe,sizeof(fromPipe));
	
	
	printf("utas erkezett\n");
	printf("utas cime: %s\n",fromPipe);
	pid_t pid2;
	
	pid2=fork();
	if(pid2==0)
	  {
		  //gyerek
		  cab();
	  }else if(pid2<0)
	  {
		  //hiba
		  perror("Fork hiba");
		  exit(1);
	  }
	  else
	  {//szülő
  
			struct uzenet uz = { 5,ChildProcesId,""};
			strcpy(uz.lakcim,fromPipe);
			int status; 
			printf("üzenet taxinak");
			status = msgsnd( uzenetsor, &uz, sizeof(uz),0); 
			
			 if ( status < 0 )
			 {
				  perror("üzenet küld hiba"); 
				  exit(0);
			 }
			 
			 /*
			status = msgrcv(uzenetsor, &uz, sizeof(uz),5,0); 
			 
			 if ( status < 0 )
			 {
				 perror("msgrcv");
			 }
			 
			 if(uz.szam==1)
			 {
				 printf("Elindult a taxi\n");
			 }*/
  
			wait();
			return;
	  }
	  
		wait();
		printf("központ end\n");
}


int main(int argc, char *argv[])
{
	  pid_t pid;
	  signal(SIGUSR1, handler);
	
	  if(argc!=2)
	  {
		  perror("argumentum nem 1");
		  exit(0);
	  }
	  
	  strcpy(utasCim,argv[1]);
	  printf("cím: %s\n",utasCim);
	  
	  if (pipe(pipefd) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   exit(0);
	}
	
	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	
	  if(uzenetsor==-1)
	  {
		   perror("Hiba a üzenetsor nyitaskor!");
	   exit(0);
	  }
	  pid=fork();
	  if(pid==0)
	  {
		  //gyerek
		  ChildProcess();
	  }else if(pid<0)
	  {
		  //hiba
		  perror("Fork hiba");
		  exit(1);
	  }
	  else
	  {//szülő
		  ParentProcess(pid);
	  }
}