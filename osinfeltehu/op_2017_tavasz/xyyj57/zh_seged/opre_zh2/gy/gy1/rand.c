#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>//gyerekdolyamathoz!!

int main()
{ 
  fork();
  //srand(time(NULL)); //the starting value of random number generation
  srand(getpid());
  //fork();
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}