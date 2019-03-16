#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

    
  signal(SIGRTMIN+1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  { 
	pid_t child2 = fork();
	if(child2>0){
		pause(); //waits till a signal arrive 
		pause();
		//int status;
		//wait(&status);
		printf("Parent process ended\n");
	}else{
		printf("Waits 2 seconds, then send a SIGTERM1 %i signal\n",SIGRTMIN+1);
		sleep(2);
		kill(getppid(),SIGRTMIN+1); 
		printf("Child2 process ended\n");  
	}
	
	
    
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM1 %i signal\n",SIGRTMIN+1);
    sleep(2);
    kill(getppid(),SIGRTMIN+1); 
    printf("Child1 process ended\n");  
  }
  return 0;
}