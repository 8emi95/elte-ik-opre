#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int x = 0;
void handler(int signumber){
  //atomi műveletek legyenek, ne megszakíthatóak
  //ezért handler műveletbe ne legyen pl. printf mert lassú és meg is szakítható 
  printf("Signal with number %i has arrived\n",signumber);
  //signal(SIGTERM,SIG_DFL);
  //raise(SIGTERM);
  x = 1;
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  { 
    sleep(3);
    //pause(); //waits till a signal arrive 
    while(!x) usleep(10); //Amíg az x nulla addig várjunk
    kill(child,SIGTERM);
    printf("Signal arrived\n",SIGTERM);
    int status;
    //Gyerek befejezésére vár
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    //sleep(3);
    kill(getppid(),SIGTERM);
    pause();
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}
