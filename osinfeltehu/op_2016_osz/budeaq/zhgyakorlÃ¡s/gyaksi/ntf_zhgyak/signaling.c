#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h> 
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void handler(int signumber){
	//sleep(1);
	printf("Signal arrived now\n");
}

void handler2(int signumber){
  srand(time(NULL)); //the starting value of random number generation
  int r = rand()%2;
  printf("%i\n", r);
//  kill(getppid(), r);	
}
int main(int argc, char** argv)
{  
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler2);
pid_t child1;
pid_t child2;
  char message[50];
  char message_read[50];

	int firstpipe[2];
	int secondpipe[2];
 if (pipe(firstpipe) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
 if (pipe(secondpipe) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
  child1 = fork();
  if(child1 < 0)
  {
    printf("Error.\n");
  }
  else if (child1 > 0)
  {
    child2 = fork();
    if (child2 < 0)
    { 
       printf("Error\n");
    }
    else if (child2 > 0)
    {
		   printf("Parent Started\n");
		   //sleep(3);
		   printf("Waiting for signal!\n");
		   pause();
		   printf("Waiting for signal!\n");
		   pause();
		   printf("Ready to go\n");
		   kill(child1, SIGUSR2);
		   kill(child2, SIGUSR2);
		   wait();
/*		   strcpy(message,"55");
		   close(firstpipe[0]);
		   close(secondpipe[0]);
		   write(firstpipe[1], message, sizeof(message));
		   strcpy(message,"60");
		   write(secondpipe[1], message, sizeof(message));
		   close(firstpipe[1]);
		   close(secondpipe[1]);
			fflush(NULL);
			strcpy(message,"something else message");
		close(pipefd[1][0]);
		     write(pipefd[1][1], message, sizeof(message));
		    close(pipefd[1][1]);*/
			printf("Parent Ended\n");
		   //sleep(1);
		
    }
    else
    {
	sleep(3);
	   printf("Child 2 Started\n");
	//   printf("Child 2 pid: %i\n", getpid());
	   kill(getppid(), SIGUSR1);
	//   wait();
	 //  sleep(1);
	//   pause();
/*	   close(secondpipe[1]);
	   read(secondpipe[0], message_read, sizeof(message_read));
	   close(secondpipe[0]);
	   printf("Child 2: %s\n", message_read);
	   fflush(NULL);
	    close(pipefd[1]);
		read(pipefd[0], message_read, sizeof(message_read));
		close(pipefd[0]);
		printf("Child 2: %i\n", atoi(message_read) + 1 );*/
		printf("Child 2 - Who starts: ");
		pause();
		printf("Child 2 Ended\n");
	   //sleep(1);
    }
  }
  else
  {
    sleep(2);
	 printf("Child 1 Started\n");
	 kill(getppid(), SIGUSR1);
	//  wait();
//	 printf("Child 1 pid: %i\n", getpid());
//	 kill(child2, SIGUSR1);
	// pause();
	//sleep(1);
	printf("Child 1 - Who starts: ");
	pause();
	 printf("Child 1 Ended\n");
	 //sleep(1);
   }
  return 0;
}
