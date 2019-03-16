#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

//most a gyerekfolyamat is csinaljon veletelnszamot

int main()
{ 
	//mivel az idotol fugg a rnd generator, ugyanazt generalnak
	//srand(time(NULL)); //the starting value of random number generation
	//kulonbozo random seedek kellenenek, hasznaljuk pl a pid szamukat
  fork();
  srand(getpid());
  
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}