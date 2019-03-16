/*	
 * Template for generating any number of child processes *  
 * */

 //////////////
 /////////////////////////////
 //kill -9 <pid>
 //top -u c7oxde
 /////////////////////////
 /////////////
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 

#define MAX_CHILDREN 10
#define CHILDCOUNT 5 //you can use a number from args[] instead just dont forget to pass it to functions
#define SPECIAL 3





int childProc()
{
	//do anything	
	sleep(1);
	
	printf("[CHILD] Finished.\n");	
}

int parentProc()
{	
	int n=0;
	int status;
	pid_t pid;
	
	while (n < CHILDCOUNT) 
	{
		pid = wait(&status);
		printf("[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
		++n;  // TODO(pts): Remove pid from the pids array.
	}	
	
	printf("[PARENT] Finished waiting for child processes.\n");
}

int specialChild()
{
	//do something different	
	sleep(3);
	
	printf("[CHILD] I was special. Now I am nothing.\n");
}

int main (int argc, char **argv) 
{ 
	
	//you can use the pids array to send signals to every child from the parent
	pid_t pids[MAX_CHILDREN]; 
	
	
	//int childCount = 1;
	int i;
	int pid;

	for (i = 0; i < CHILDCOUNT; ++i) 
	{
		pid = fork();
		if (pid < 0) 
		{
			perror("fork");
			abort();
		} 
		else if (pid == 0) 
		{
			//child i
			printf("[CHILD] Starting..(%d)\n", getpid());
			
			if (i!=SPECIAL) //we can start more than one type of child proc
			{
				childProc();
			}			
			else 
			{	
				specialChild();
			}	
			break;			
		} 	
		else
		{
			pids[i] = pid;
		}		
	}
	
	if (pid > 0) 
	{
		//parent
		printf("[PARENT] Starting..(%d)\n", getpid());
		
		parentProc();
		/*
		int n=0;
		int status;
		pid_t pid;
		
		while (n < childCount) 
		{
			pid = wait(&status);
			printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
			++n;  // TODO(pts): Remove pid from the pids array.
		}
		*/
	}

}
