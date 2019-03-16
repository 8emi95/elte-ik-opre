#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <signal.h>
#include <sys/types.h> 

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <poll.h>

#define MAX_CHILDREN 10
#define CHILDCOUNT 1 

static void handler(int sig){
    printf("ujabb utas jott\n");
    signal(sig, handler);
}

struct Child{

    int id;
    char address[50];

};

int child1Proc(struct Child* child, int* num, int read,int write, char* arg)
{
	sleep(2);
    strcpy(child[*num].address, arg);
    child[*num].id = (*num)++;

    close(read);
    //write();
    kill(getppid(), SIGTERM);
	printf("[GYEREK] MEGYEK.\n");	
}


int parentProc(int read,int write)
{	
	int n=0;
	int status;
	pid_t pid;
	
    close(write);
    pause();
	while (n < CHILDCOUNT) 
	{
		pid = wait(&status);
		printf("[SZULO] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
		++n;  
	}	
	
	printf("[SZULO] varja a gyerekek halalat.\n");
}

int main (int argc, char **argv) 
{ 
	
	pid_t pids[MAX_CHILDREN]; 
	
	int i;
	int pid;
    int number_of_children = 0;
    char* adr = argv[1];
    signal(SIGTERM,handler);

    struct Child children[MAX_CHILDREN];
    //children[0].id = 0;

    int file_desc[2];
    pipe(file_desc);

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

            printf("[GYEREK] elindult..(%d)\n", getpid());
		    child1Proc(children, &number_of_children, file_desc[0], file_desc[1],adr);	
								
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
		printf("[SZULO] elindult..(%d)\n", getpid());
		
		parentProc(file_desc[0],file_desc[1]);

	}

}