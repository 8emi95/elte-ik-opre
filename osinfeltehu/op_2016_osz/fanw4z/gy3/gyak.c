#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>


bool firstPReady = false;
bool secondPReady = false;
void handler(int signumber){
	if(signumber = SIGUSR1)
	{
		printf("First player ready\n");
		firstPReady = true;
	}
	else if(signumber = SIGUSR2)
	{
		printf("Second player ready\n");
		secondPReady = true;
	}
		

}

int main(){

    
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
  signal(SIGUSR2,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t firstP=fork();
  if (firstP>0)
  { 
    
    while(!(firstPReady && secondPReady)){usleep(100);}     
   
	pid_t secondP = fork();
	if(secondP > 0)
	{
		
	}
	else
	{
		kill(getppid(),SIGUSR2);
		printf("Masodik folyamat futasra kesz\n");
	}
  }
  else 
  {
    
    kill(getppid(),SIGUSR1);
	printf("Elso folyamat futasra kesz\n");
    
    
  }
  return 0;
}