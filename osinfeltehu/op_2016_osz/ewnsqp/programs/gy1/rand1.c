#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main()
{ 
  fork();
  srand(getpid()); // tekintettel arra, hogy a pid az mindig mas, ezzel inditjuk a random generalast a fork utan
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}