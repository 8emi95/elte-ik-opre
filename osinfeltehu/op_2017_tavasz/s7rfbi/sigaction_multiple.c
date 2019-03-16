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
  sigemptyset(&sigact.sa_mask);

  sigact.sa_flags=SA_RESTART;
  sigaction(SIGUSR1,&sigact,NULL);

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


    printf("If multiple copies of a normal signal is pending, only once of them will be executed\n ");
    printf("Each of real time signals will be executed - the order of execution is the same as the calling order\n ");
    sleep(1);
    int i;
    for (i=0;i<5;i++){
      kill(getppid(),SIGUSR1);
      kill(getppid(),SIGRTMIN);

    }
    printf("Child process ended\n");
  }
  return 0;
}
