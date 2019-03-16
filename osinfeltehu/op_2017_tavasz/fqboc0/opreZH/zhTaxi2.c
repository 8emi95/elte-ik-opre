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

void handler(int signum){printf("SIGNAL\n");}
 
void cab(void)
{
	printf("taxi Start\n");
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