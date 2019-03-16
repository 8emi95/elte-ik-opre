#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <sys/time.h>

void handler(int signumber){}

int main()
{ 
  signal(SIGTERM,handler);
  pid_t pid;
  
  pid_t child1=fork(), child2;
  
  //char arg[256];
  int num = 0, num2 = 0, i = 0, status, pipefd[2];
  
  if (pipe(pipefd) == -1) 
  {
	perror("Hiba a pipe nyitaskor!");
	exit(EXIT_FAILURE);
  }
		   
  if(child1 < 0)
  {
	  perror("The fork calling for child1 was not succesful\n");
	  exit(1);
  }
  if(child1 > 0) //the parent
  {
	  child2 = fork();
	  if(child2 < 0)
	  {
		perror("The fork calling for child1 was not succesful\n");
		exit(1);
	  }
	  if(child2 > 0) //parent
	  {		
	    close(pipefd[0]);
		pause();
		pause();
		printf("Hany szavazo erkezik?\n");
		scanf("%d", &num);
		//gets(arg);
		//num=atoi(arg);
		if(num > 0)
		{
			write(pipefd[1], &num, sizeof(num));
			pause();
			int azon;
			srand(time(NULL));
			for(i = 0; i < num; ++i)
			{
				azon = rand()%1000;
				printf("Voter number %i has the ID %i\n",i+1,azon);				
				write(pipefd[1], &azon, sizeof(int));
			}			
		}
		wait(&status);
		printf("Parent process ended\n");
	  }
	  else //child2
	  {
		  int ujazon;
		  close(pipefd[1]);
		  sleep(2);
		  kill(getppid(),SIGTERM);
		  sleep(2);
		  read(pipefd[0],&num2,sizeof(num2));
		  kill(getppid(),SIGTERM);
		  for(i = 0; i < num2; ++i)
		  {
			  read(pipefd[0],&ujazon,sizeof(int));
			  printf("New ID: %i\n", ujazon);
		  }	  
	  }
  }
  else //the child
  {
	    close(pipefd[1]);
		sleep(1);
		kill(getppid(),SIGTERM);
		while(1)
		{
			sleep(1);
		}
		exit(1);	     
  }  
  return 0;
}