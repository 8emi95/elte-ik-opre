#include <stdio.h>
#include <stdlib.h> //srand

int main()
{ 
//  srand(time(NULL)); //the starting value of random number generation (use seconds)
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  printf("Working with two processes!\n");
  pid_t child=fork();
  if (child>0) {
    //with one srand - each of the processes use the same random generator
    //USE new srand function calls for each processes!
//     srand(time(NULL)); // why it is not good? - only seconds is used up!!
  //  srand(getpid()); //PID numbers are always different
    r=rand()%100; //number between 0-99
    printf("Random number %i\n",r);
    int status;
    wait(&status);
  }
  else {
//    srand(time(NULL)); // why it is not good? - only seconds is used up!!
//    srand(getpid());
    r=rand()%100; //number between 0-99
    printf("Random number %i\n",r);
  }
  return 0;
}