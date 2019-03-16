#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

bool firstPReady = false;
bool secondPReady = false;

int counter = 0;

void handler(int signumber){
	if(signumber == SIGTERM)
	{
		printf("First player ready\n");
		++counter;
	}
}

void childHandler(int signumber){
	if(signumber == SIGUSR1)
	{
		printf("I'm starting\n");
	}
	else if(signumber == SIGUSR2)
	{
		printf("I'm being second\n");
	}
	
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP),  //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
  signal(SIGUSR1, childHandler);
  signal(SIGUSR2, childHandler);                         //handler = SIG_DFL - back to default behavior 
  
  pid_t firstP=fork();
  if (firstP>0)
  { 
    
	pid_t secondP = fork();
	if (secondP < 0)
	{
		perror("fork"); 
	}
	else if(secondP > 0)
	{
		//parent
		while(counter != 2){usleep(100);}     
		srand(time(NULL)); 
		int r=rand()%2;
		if(r)
		{
			kill(secondP, SIGUSR1);
			kill(firstP, SIGUSR2);
			printf("Masodik jatekos kezd\n");
			
		}else
		{
			kill(secondP, SIGUSR2);
			kill(firstP, SIGUSR1);
			printf("Elso jatekos kezd\n");
		}
		wait(NULL);
		wait(NULL);
	}
	else
	{
		//secondP
		printf("Masodik folyamat futasra kesz\n");
		kill(getppid(),SIGTERM);
	}
  }
  else 
  {
   //first
	printf("Elso folyamat futasra kesz\n");
    kill(getppid(),SIGTERM);
 
  }
  return 0;
}