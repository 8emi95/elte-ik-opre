#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{ 
  //fork()
  //srand(getpid()); majd alatta levõ sor törlése. így elhet megoldani h ne ugyanaz a randszám generálódjon fork() miatt.
  srand(time(NULL)); //the starting value of random number generation
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}