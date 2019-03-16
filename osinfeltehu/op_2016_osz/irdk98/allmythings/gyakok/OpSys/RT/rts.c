//gcc -lrt 
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

  struct sigaction sigact;
  sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
  sigemptyset(&sigact.sa_mask); //during execution of handler these signals will be blocked plus the signal    
  //now only the arriving signal, SIGTERM will be blocked
  //sigact.sa_flags=0; //the handler execution interrupts the process - real time signal
  //sleep is not a real time safe function
  sigact.sa_flags=0; //SA_RESTART; //0; //no special behaviour
 //*************************
 //real time signals between SIGRTMIN - SIGRTMAX 32-64
 //Several delivery
 //************************************************  

  sigaction(SIGRTMIN,&sigact,NULL);
  sigaction(SIGRTMAX,&sigact,NULL);
  pid_t ch=fork();
  if (ch>0){
  wait(NULL); sleep(1);}
  else {
  int i;
  for (i=0;i<5;i++)
  {
    kill(getppid(),SIGRTMIN);
    kill(getppid(),SIGRTMAX);
  }
  }  
  
  return 0;
}