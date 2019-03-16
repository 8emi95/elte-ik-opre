#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int childPid = 0;

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //kill(childPid, 10);
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
  signal(SIGUSR1,handler);                        //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  { 
	childPid = child;
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);
    int status;
    
	kill(child, SIGUSR1);
	wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
	printf("before sleep\n");
    sleep(3);
	printf("Signal will be sent from child\n");
    kill(getppid(),SIGTERM); 
	printf("Signal sent from child\n");
	pause();
	printf("Signal arriced\n", SIGUSR1);
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}