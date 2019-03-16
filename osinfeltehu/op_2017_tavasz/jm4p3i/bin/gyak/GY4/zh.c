//1 parent 2 child
/*
c1 checks
c2 ack

1.) 
	a. p waits until c1&c2 are ready
	b. p receives # of voters from c1
	c. p generates rnd for sum of voters, sends back to c2
	d. c2 prints results
	e. p waits for c2 to finish
	
2.)

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

int main(int argc, char *argv[])
{
	int pipefd[2]; // unnamed pipe file descriptor array
    pid_t pid;
    char sz[100];  // char array for reading from pipe

	if (pipe(pipefd) == -1) 
	{
	    perror("Hiba a pipe nyitaskor!");
	    exit(EXIT_FAILURE);
	}


	switch (pid = fork())
    {
    	case -1:
      		err_sys("fork hiba"); 
      	case 0: 
      		//child;
      	default:
      		//parent
      		switch(pidd = fork())
      		{
      			case -1:
      				err_sys("2. fork hiba");
      			case 0:
      				//child2
      			default: 
      				//parent
      		}
    }

	
	pid = fork();	// creating parent-child processes
	if (pid == -1) 
	{
	    perror("Fork hiba");
	    exit(EXIT_FAILURE);
	}
	
	if (pid == 0) 
	{	
		//2 pause for 2 children
		pause(); 
		pause(); 
		
		int n = atoi(argv[1]);
		//Rand rnd = Rand(); //srand
		
		//send through pipe
		//read() returns amount of bytes read
		
		//2 waits
		//wait();
		//wait();
	} 
	else 
	{
		//C1 ready, send signal
		kill(getppid(), SIGRTMIN+5); 
		//vagy:
		//kill(getppid(), SIGUSR1); 
	}
}