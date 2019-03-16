#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGTERM,SIG_DFL);
  raise(SIGTERM);
}

int main(){

  signal(SIGTERM,handler);
  
  pause();
  printf("Signal arrived\n",SIGTERM);
  
  return 0;
}