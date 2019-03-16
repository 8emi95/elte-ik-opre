#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived from %i\n",signumber,getppid());
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  {
  sleep(3);
   kill(child,SIGTERM);
    printf(" %i signal szulo\n",getppid());
    printf(" %i signal szulo\n",child);
     
    pause(); //waits till a signal arrive 
    printf("Signal arrived %d\n",SIGTERM);
    kill(child,SIGTERM);
    int status;
    wait(&status);
    printf("ez valami %i\n",status);
    printf("Parent process ended\n");
  }
  else 
  {
    pause();
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    printf(" %i signal\n",getppid());
    sleep(3);
    kill(child,SIGTERM); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    pause();
    printf("Child process ended\n");  
  }
  return 0;
}