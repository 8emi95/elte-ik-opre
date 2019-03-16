//gcc -lrt 
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  printf("Signal handler ends \n");
}

int main(){

  struct sigaction sigact;
  sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
  sigemptyset(&sigact.sa_mask); //during execution no signals blocked
  //
  sigact.sa_flags=0; //the handler execution interrupts the process
  //sigact.sa_flags=SA_RESTART; //the interrupted signal will restart!!!
  sigaction(SIGUSR1,&sigact,NULL);
  //SIGUSR1 normal signal *** ONLY ONE is delivered
  //1. parameter the signal number
  //special sa_flags - it change the behavior of signal, 
  //SIGNOCLDSTOP-after the child process ended it won't send a signal to parent 
  //3. parameter - &old sigset or NULL. 
 //real time signals between SIGRTMIN - SIGRTMAX 32-64
  sigaction(SIGRTMIN,&sigact,NULL);
 
  pid_t child=fork();
  if (child>0)
  {
    printf("The program comes back from suspending\n");
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 1 seconds, then send 5 SIGUSR %i signals and 5 SIGRTMIN %i signals \n", SIGUSR1,SIGRTMIN);
    //real time signals are between SIGRTMIN and SIGRTMAX
    //the smalest real time signal will be executed first
    //the execution order of normal signals is not predefined
    
    printf("If multiple copies of a normal signal is pending, only once of them will be executed\n ");
    //sleep(1);
    int i;
    for (i=0;i<3;i++)
    {
      kill(getppid(),SIGUSR1);
      kill(getppid(),SIGRTMIN);
    //  kill(getppid(),SIGUSR1);
      sleep(1);  //if sleep is working, then sigusr1 has done before the next call performs
   // so all SIGUSR1 will arrive, sa_flags SA_RESTART
   // if sa_flags=0, only 1 sigusr1 and sigrtmin will arrive 
   }
    printf("Child process ended\n");  
  }
  return 0;
}
