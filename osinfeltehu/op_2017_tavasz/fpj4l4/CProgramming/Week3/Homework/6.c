#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{ 
  int processes = 5;
  int i;

  for(i = 0;i<processes;i++) {
    if(fork() == 0) {
      printf("I am a child \n");

      exit(0);
    }
  }  
  
  int status;
  //for(i = 0;i<processes;i++) {
    wait(&status);
  //}
  
  printf("I am a parent \n");

  return 0;
}
