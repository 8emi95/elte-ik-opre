//> /tmp/mqueue_server_key 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>

#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key" 
#define PROJECT_ID 'M' 
#define QUEUE_PERMISSIONS 0660 
#define MAX_CHILDREN 10

struct message_text { 
int qid; 
char buf [200]; 
}; 

struct message { 
long message_type; 
struct message_text message_text; 
}; 

void sig_handler(int signo)
{
	switch(signo)
	{
		case SIGINT:		
			printf("received SIGINT\n");
			break;
		case SIGUSR1:		
			printf("received SIGUSR\n");
			break;
	}
}

int childProc(int par)
{
	sleep(par);
	kill (getppid (), SIGUSR1);
	sleep(par*2);
	kill (getppid (), SIGINT);
	printf("[CHILD] Finished. (%d)\n", getpid());

	return 0;
}

int parProc(int par)
{
	//
	sleep(9);
	
	return 0;
}

int main (int argc, char **argv) 
{ 
	pid_t pids[MAX_CHILDREN]; 
	int childCount = 2;
	int i;
	int pid;
	
	signal(SIGINT, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGCONT, sig_handler);

	for (i = 0; i < childCount; ++i) 
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
			
			childProc(i);
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
		
		parProc(0);
		
		printf("[PARENT] Finished.(%d)\n", getpid());
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