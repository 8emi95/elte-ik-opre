#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

pid_t child;

int h=0;

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGTERM, SIG_DFL); // sigterm beállítjuk defaultra
  raise(SIGTERM);	// magunknak küldünk jelet
  h++;
}

int main(){    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  child=fork();
  if (child>0)
  { 
    printf("Parent pause\n");
    pause(); //waits till a signal arrive
    
    while(h == 0) { usleep(10); } // proci killer, lol dont do it pls.
    
    kill(child, SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    kill(getppid(),SIGTERM); 
    printf("Child pause\n");
    pause();
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}