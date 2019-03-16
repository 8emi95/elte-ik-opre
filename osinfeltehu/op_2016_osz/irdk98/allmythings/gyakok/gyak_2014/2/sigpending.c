#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //fork
#include <signal.h> //signal
#include <sys/types.h> //wait
#include <sys/wait.h> //wait
#include <string.h> //strsignal

void handler(int signumber){
  printf("Signal \'%s\' with number %i has arrived\n",strsignal(signumber),signumber);
}
sigset_t st;

void alarmhandler(int signumber){
 raise(SIGKILL);
}
int main(){

  signal(SIGUSR2,SIG_IGN);
  signal(SIGUSR1,handler);    
  signal(SIGTERM,handler);  
  signal(SIGALRM,alarmhandler);
  
  sigfillset(&st);
  sigdelset(&st,SIGUSR2); //if it is blocked, then it is pending!!! 
  sigprocmask(SIG_BLOCK,&st,NULL); //everything is blocked
  
  pid_t child=fork();
  if (child>0)
  { 
    wait(NULL);
    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending,SIGUSR1))
     {printf("SIGUSR1 is pending\n");}
     else 
     {printf("SIGUSR1 is not pending\n");}
    if (sigismember(&pending,SIGUSR2))
     {printf("SIGUSR2 is pending\n");}
     else 
     {printf("SIGUSR2 is not pending\n");}
    if (sigismember(&pending,SIGTERM))
    {printf("SIGTERM is pending\n");}
    else 
    {printf("SIGTERM is not pending\n");}
    
    sleep(1); //unblock signals
    sigprocmask(SIG_UNBLOCK,&st,NULL); //everything is blocked
    alarm(3);//stops process 
    
    while(1){sleep(1);}
    
    printf("Parent process ended\n");
  }
  else 
  {

    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    kill(getppid(),SIGTERM);
    printf("SIGTERM was sent\n");
    kill(getppid(),SIGUSR1);
    printf("SIGUSR1 was sent\n");
    kill(getppid(),SIGUSR2);
    printf("SIGUSR2 was sent\n");
    
    printf("Child process ended\n")
  
    ;  
  }

  return 0;
}