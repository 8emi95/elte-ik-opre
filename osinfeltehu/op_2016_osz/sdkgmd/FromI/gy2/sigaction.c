#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //sleep(3);
  printf("Signal handler is finished!\n");
}

int main(){

  struct sigaction sigact;
  sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
  sigemptyset(&sigact.sa_mask); //during execution of handler no signals will be blocked
  // we can add signals to the mask, and that signal will be blocked
  sigaddset(&sigact.sa_mask,SIGUSR1); //SIGUSR1 will be blocked during handler
  // after finishing handler, other signal also will be delivered
  sigact.sa_flags=0; //nothing special behaviour
  //sigact.sa_flags=SA_RESTART; // ha handler alatt jon ujabb signal
  //sigact.sa_flags=SA_SIGINFO; //ekkor nem sa_handler,hanem a sa_sigaction
  //hatarozza meg a kezelot! Ennek alakja: 
  //void func(int signo, siginfo_t *info, void *context); 
  sigaction(SIGUSR2,&sigact,NULL);
  sigaction(SIGUSR1,&sigact,NULL);
  //1. parameter the signal number
  //2. parameter the new sigaction with handler and blocked signals during the execution of handler (sa_mask) and a 
  //special sa_flags - it change the behavior of signal, 
  //e.g. SIGNOCLDSTOP - after the child process ended it won't send a signal to the parent 
  //3. parameter - &old sigset or NULL. 
  //If there is a variable, the function will fill with the value of formerly set sigset
  pid_t child=fork();
  if (child>0)
  {
//    sigset_t sigset;
//    sigfillset(&sigset);
//    sigdelset(&sigset,SIGUSR2);
//    sigsuspend(&sigset); // only SIGUSR2 non blocked
    // like pause() - except it waits only for signals not given in sigset
    //others will be blocked
    printf("The program comes back from suspending\n");
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits some and send a SIGUSR1 %i signal (it is not waited for by suspend) - so the suspend continues waiting\n", SIGUSR1);
    //sleep(1);
    kill(getppid(),SIGUSR1);
    printf("Waits a little, then send an other signal!\n");
    //sleep(1);
    kill(getppid(),SIGUSR2);
    printf("Child process ended\n");  
  }
  return 0;
}
