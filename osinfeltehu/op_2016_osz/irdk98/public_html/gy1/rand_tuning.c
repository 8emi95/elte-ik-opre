#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{
  fork();
  // srand(time(NULL)); //the starting value of random number generation FROM SYSTEM TIME!
  // its gonna be unique!
  srand(getpid());
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}
        