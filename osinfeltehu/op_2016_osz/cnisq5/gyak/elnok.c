#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <wait.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct message {
	int ids[100];
	int size;
};

void handler(int signumber){
  if (signumber == SIGUSR1)
  {
	  
  };
}

void ParentProc(int childPid, int childtwoPid, int* p, struct message m)
{
	pause();	
	pause();
	printf("The members are ready \n");
	
	close(p[0]);
	write(p[1], &m, sizeof(struct message));
	close(p[1]);
	
	int status;
	waitpid(childPid,&status,0);
	waitpid(childtwoPid,&status,0);
	
	
}

void ChildProc(int* p)
{
	sleep(1);
	printf("i am the first member\n");
	kill(getppid(),SIGUSR1);
	
	struct message m;
	
	close(p[1]);
	read(p[0], &m, sizeof(struct message));
	
	printf("Megkaptam az azonositokat\n");
	int n = 0;
	while (n < m.size)
	{
		printf("%d\n", m.ids[n]);
		n++;
	}
	
	close(p[0]);
	
}

void ChildTwoProc()
{
	sleep(2);
	printf("i am the second member\n");
	kill(getppid(),SIGUSR1);
}

int main(int argc, char* argv[])
{
   	signal(SIGUSR1,handler);
	
	int mypipe[2];
	
	if (pipe(mypipe) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	
	
	struct message m;
	int n = 0;
	int s = atol(argv[1]);
	while (n < s)
	{
		m.ids[n] = n;
		n++;
	}
	m.size = n;
	printf("Itt vagyok\n");
	
	pid_t child = fork(); 
	printf("child: %d\n", child);
	if (child < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(child > 0)
	{
		printf("parentben\n");
		
		pid_t childtwo = fork();
		printf("child2: %d\n", childtwo);
		if (childtwo < 0)
		{
			perror("fork"); 
			return 1; 
		}
		else if(childtwo > 0)
		{
			printf("Még mindig parentben.");
			ParentProc(child, childtwo, mypipe, m);
		}
		else
		{
			printf("child 2");
			ChildTwoProc();		
		}
	}
	else if (child == 0)
	{
		printf("child1");
		ChildProc(mypipe);
	}
}