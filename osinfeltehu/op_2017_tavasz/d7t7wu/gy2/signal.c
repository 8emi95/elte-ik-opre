#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
int x = 0;
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGUSR1, SIG_DFL);
  
  x++;
}

int main(){

    
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
 
  pid_t child=fork();
  if (child>0)
  {
    pause(); //waits till a signal arrives 
    printf("Signal arrived\n",SIGUSR1);
    
    sleep(3);
    kill(child, SIGUSR1);
    
    int status;
    wait(&status);
    
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGUSR1);
    sleep(3);
    kill(getppid(),SIGUSR1); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    pause();
    printf("Signal arrived\n", SIGUSR1);
    
    printf("Child process ended\n");  
  }
  return 0;
}