#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct msg
{
  int number;
  char name[100];
  char task[100];
};

void handler(int signum)
{
        if (signum == SIGUSR1)
        {
		printf("signal received\n");
	}
}

int main(int argc,char *argv[])
{
  srand(time(NULL));
  printf("\n");
  int pipefdparent[2];

  if (pipe(pipefdparent) == -1) 
  {
    perror("pipe error !");
    exit(EXIT_FAILURE);
  }

  int pipefdchild[2];

  if (pipe(pipefdchild) == -1) 
  {
    perror("pipe error!");
    exit(EXIT_FAILURE);
  }  

  int number=0;

  signal(SIGUSR1,handler);

  int pidchild = fork();
  if (pidchild<0)
  {
    printf("fork error\n");
    exit(1);
  }
  else if(pidchild==0) //child
  {

    struct msg msgvc;
    read(pipefdparent[0], &msgvc, sizeof(msgvc));

    printf("%s competitor \ntask: %s \n number: %d \n\n",msgvc.name,msgvc.task,msgvc.number);
	srand(time(NULL)); //the starting value of random number generation
	int r=rand()%100; //number between 0-99
	int success = r%2;
	printf("rnd number is: %i\n",r);
    if(success==1)
    {
      printf("%s :i can't complete the task!\n",msgvc.name);
	  
	  
    }
    else
    {
      printf("%s :success, complete the task\n",msgvc.name);      
    }

    
    write(pipefdchild[1], &success , sizeof(success ));
    sleep(2);
    kill(getppid(),SIGUSR1);
    

    
  }
  else
  {

    struct msg msgvp;
    msgvp.number=number;
    strcpy(msgvp.name,argv[1]);
    strcpy(msgvp.task,"task 1.");
    write(pipefdparent[1], &msgvp, sizeof(msgvp));
  
	
	
    int success=0;
    read(pipefdchild[0], &success, sizeof(success));
    pause();


    if(success==1)
    {
      printf("trainer: the chiled (%s) was not complete the task\n",msgvp.name);
    }
    else
    {
      printf("trainer: the child (%s) completed the task\n",msgvp.name);      
    }

  }

	return 0;
}

