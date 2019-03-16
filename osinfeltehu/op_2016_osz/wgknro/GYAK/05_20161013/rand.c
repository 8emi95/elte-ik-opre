#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h> //fork

int main()
{ 
//  srand(time(NULL)); //the starting value of random number generation
  fork();
  srand(getpid()); //mind a két pid különböző time-t kap ha a pid-et hozzáadjuk akkor biztos
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}