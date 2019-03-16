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
  { 
    pause(); //waits for a signal
    printf("Signal arrived\n",SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {

    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3); //Only making a demo we use sleep!!
    kill(getppid(),SIGTERM);
    printf("SIGTERM lefutott\n");
    
   //  kill(-1,SIGTERM);
    
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then each of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n")
  
    ;  
  }

  return 0;
}