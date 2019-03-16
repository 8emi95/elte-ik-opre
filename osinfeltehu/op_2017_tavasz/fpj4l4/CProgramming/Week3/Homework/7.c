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

  for(i = 0;i<processes;i++) {
    if(fork() == 0) {
      execv("./7E.out", argv);

      exit(0);
    }
  }  
  
  int status;
  for(i = 0;i<processes;i++) {
    wait(&status);
  }
  
	printf("Parent terminating\n"); 

  return 0;
}
