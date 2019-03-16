#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <signal.h>
#include <sys/types.h> 

#define MAX_CHILDREN 10
#define CHILDCOUNT 2 

void handler(int sig){
    switch(sig){
        case SIGTERM:
            printf("[GYEREK 1] Elindultam\n");
            break;
        case SIGINT:
            printf("[GYEREK 2] Elindultam\n");
            break;
        case SIGILL:
            printf("[SZULO] BLAAA\n");
            break;
    }
    
}

struct Child{

    int id;
};

int child1Proc()
{
	sleep(1);
    raise(SIGTERM);
	//printf("[GYEREK] Finished.\n");	
}

int child2Proc()
{
	sleep(1);
    raise(SIGINT);
	//printf("[GYEREK] Finished.\n");	
}


int parentProc()
{	
	int n=0;
	int status;
	pid_t pid;
	
	while (n < CHILDCOUNT) 
	{
		pid = wait(&status);
		printf("[SZULO] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
		++n;  
	}	
	
	printf("[SZULO] Finished waiting for child processes.\n");
}

int main (int argc, char **argv) 
{ 
	
	pid_t pids[MAX_CHILDREN]; 
	
	int i;
	int pid;

    signal(SIGTERM,handler);
    signal(SIGINT,handler);
    signal(SIGILL,handler);

    struct Child children[2];

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
			if(i == 0)
            {
                //printf("[GYEREK] Starting..(%d)\n", getpid());
			    child1Proc();	
            } else {
                child2Proc();
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
		printf("[SZULO] Starting..(%d)\n", getpid());
		
		parentProc();

	}

}