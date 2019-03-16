#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //fork
#include <signal.h> //signal
#include <sys/types.h> //wait
#include <sys/wait.h> //wait
#include <string.h> //strsignal

void handler(int signumber){
  printf("Signal \'%s\' with number %i has arrived\n",strsignal(signumber),signumber);
//  signal(SIGTERM,SIG_DFL);
//  raise(SIGTERM);
  //strsignal function gives back the description of signal
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
  signal(SIGUSR1,handler);                         //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  
  if (child>0)
  { pid_t child2=fork();
    if (child2>0){
      sleep(3);
      kill(-1,SIGTERM);
    
    printf("Signal send...\n",SIGTERM);
    int status;
    waitpid(0,&status,0);
    printf("Parent process ended\n");
    }else {pause();}
  }
  else 
  {

    pause();    
   //  kill(-1,SIGTERM);
    
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then each of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
  
    ;  
  }

  return 0;
}