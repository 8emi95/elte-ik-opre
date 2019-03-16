#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{ 	
	
I am trying to spawn n child processes, then let each child process request a random number of resources. However, each child currently requests an identical number of resources, though that number changes each time I run the program.

/* Create the appropriate number of processes */
int pid;
for(int i = 0; i < numberOfProcesses; i++){
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork Failed");
        exit(1);
    }
    else if(pid == 0){
        time_t t;
        srand((unsigned) time(&t));

        printf("Child (%d): %d.", i+1, getpid());

        /* Generate a random number [0, MAX_RESOURCES] of resources to request */

        int requestNum = rand() % (MAX_RESOURCES + 1);
        printf(" Requesting %d resources\n", requestNum);

        exit(0);
    }
    else{ wait(NULL); }
}

  return 0;
}