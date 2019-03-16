/*	
 * Template for generating any number of child processes *  
 * */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h>


#define MAX_CHILDREN 10
#define CHILDCOUNT 2 //you can use a number from args[] instead just dont forget to pass it to functions
#define SPECIAL 1

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 


int childProc(int msgq)
{
	struct uzenet uz; 
	msgrcv(msgq, &uz, 1024, 5, 0 ); 
	printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
	//do anything	
	sleep(1);
	
	printf("[CHILD] Finished.\n");
    return 0;	
}

int parentProc(int msgq)
{	
	int n=0;
	pid_t pid;
	int status;

	
	if (msgq <0)
		return 1;
		
	
	while (n < CHILDCOUNT) 
	{
		struct uzenet uz;
		uz.mtype = 5;
		sprintf(uz.mtext, "Hajra Ujpest! (%i)", n);
		msgsnd( msgq, &uz, strlen ( uz.mtext ) + 1 , 0 );
		pid = wait(&status);
		printf("[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
		++n;  
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
	int status, msgq;	
	key_t key = ftok(argv[0],1);
	msgq = msgget( key, 0600 | IPC_CREAT);
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
			
			childProc(msgq);
			
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
		
		parentProc(msgq);
	}

}