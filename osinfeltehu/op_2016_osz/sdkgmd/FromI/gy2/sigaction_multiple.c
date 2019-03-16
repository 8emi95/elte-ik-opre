//gcc -lrt 
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
int db=0;
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //printf("Signal handler ends \n");
  db++;
}

int main(){
  
  struct sigaction sigact;
  sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
  sigemptyset(&sigact.sa_mask); //during execution no signals blocked
  //
  sigact.sa_flags=0; //the handler execution interrupts the process - real time signal
  //sleep is not a real time safe function
 sigact.sa_flags=SA_RESTART; //the interrupted function will restart!!!
  sigaction(SIGUSR1,&sigact,NULL);
  sigaction(SIGUSR2,&sigact,NULL);
  //SIGUSR1 normal signal *** ONLY ONE is delivered
  //1. parameter the signal number
  //2. parameter the new sigaction with handler and blocked signals during the execution of handler (sa_mask) and a 
  //special sa_flags - it change the behavior of signal, 
  //e.g. SIGNOCLDSTOP - after the child process ended it won't send a signal to the parent 
  //3. parameter - &old sigset or NULL. 
  //If there is a variable, the function will fill with the value of formerly set sigset
 //*************************
 //real time signals between SIGRTMIN - SIGRTMAX 32-64
 //************************************************  
  sigaction(SIGRTMIN,&sigact,NULL);
 
  pid_t child=fork();
  if (child>0)
  {
   // printf("The program comes back from suspending\n");
    int status;
  //  sleep(2);
    waitpid(child,&status,0);
    printf("Parent process ended, db=%i.\n",db);
  }
  else 
  {
    printf("Sending multiple signals SIGUSR %i and SIGRTMIN %i. \n", SIGUSR1,SIGRTMIN);
    //real time signals are between SIGRTMIN and SIGRTMAX
    //the smalest real time signal will be executed first
    //the execution order of normal signals is not predefined
    printf("If multiple copies of a normal signal is pending, only once of them will be executed\n ");
    printf("Each of real time signals will be executed - the order of execution is the same as the calling order\n ");
    //sleep(1);
    int i;
    for (i=0;i<3;i++)
    {
      kill(getppid(),SIGUSR1);
      kill(getppid(),SIGUSR2);
//	printf("2 signals are sent");
//	printf("2 signals are sent");
//	printf("2 signals are sent\n");
   sleep(1);  //if sleep is working, then sigusr1 has done before the next call performs
   // so all SIGUSR1 will arrive, sa_flags SA_RESTART
   // if sa_flags=0, only 1 sigusr1 and sigrtmin will arrive 
   }
  //  sleep(3);
    printf("Child process ended.\n");  
  }
  return 0;
}
