#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>  //fork


int main()
{ 
  
  fork(); //minden benne lesz mindkét ágban ami ezután van
  srand(getpid()); //the starting value of random number generation
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}