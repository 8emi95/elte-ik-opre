#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);

  return 0;
}