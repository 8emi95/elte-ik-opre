#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
    printf("There was a number between 50 and 75!\n");
  //printf("Signal with number %i has arrived\n",signumber);
}

int main(){

    
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  while (1) {
    int num = rand()%100+1;
    printf("Number generated: %d\n",num);  
    if ( num >= 50 && num <= 75 ) {
        kill( getpid(), SIGUSR1);
    }
    sleep(1);
  }

  return 0;
}
