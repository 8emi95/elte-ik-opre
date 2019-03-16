#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
int x = 0;

void handler(int signumber){
  printf("signal user 1: ");
  signal(SIGUSR1, SIG_DFL);
  x++;
}

int main(){

    int st=0;
    srand(time(NULL));   // should only be called once


  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP),
                           //handler = SIG_DFL - back to default behavior

  pid_t child1=fork();
  if (child1>0)
  {
    //while(x==0){}
    pause();
    printf("The signal arrived.\n");
    sleep(3);
    kill(child1, SIGUSR1);
    
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else
  {
    sleep(3);

    kill(getpid(),SIGUSR1);
    printf("Value of x: %d",x);
    pause();
    printf("Signal arrived\n", SIGUSR1);
    printf("First child process ended\n");
  }
  return 0;
}
