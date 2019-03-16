#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

  if(argc != 3) {
    printf("WRONG NUMBER OF ARGS \n");
    return -1;
  }
 
  int processes = atoi(argv[2]);
  int i;
  char command[] = "./7E.out yep";
  for(i = 0;i<processes;i++) {
    if(fork() == 0) {
      system(command);

      exit(0);
    }
  }  
  
  int status;
  for(i = 0;i<processes;i++) {
    wait(&status);
  }
  
	  

  return 0;
}
