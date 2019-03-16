#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void semmi(){}

int main(int arg, char *args[])
{
  pid_t pid;
  
  if((pid = fork()) < 0)
  {
	perror("fork");
	exit(-1);
  }else if(pid == 0)
  {
	printf("Child process starting, Parent: %d, Child: %d\n", getpid(), getppid());
	
	sleep(5);

	printf("Child continues...\n");
	printf("Child iterminated.\n");
  }else
  {
	printf("Parent process starting, Pid: %d\n", getpid());
		//getpid() --> megadja a child process pid-jét
//	wait(NULL); //Vár a gyerekre

	kill(pid, SIGSTOP);

	printf("Child paused??\n");

	kill(pid, SIGCONT);
	wait(NULL);
//	kill(pid, SIGCONT);

	printf("Parent terminated...\n");
  }
  



  return 0;
}
