#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>

void failed_handler(int signumber)
{
  
}

void ok_handler(int signumber)
{
  exit(0);
}

void main( int argc, const char* argv[] )
{
	signal(SIGTERM,failed_handler);
	signal(SIGINT,ok_handler);
	
	time_t t;
	srand((unsigned) time(&t));
	
	int max = atoi( argv[1] );
	
	int pid;
	int childread[2];
	int childwrite[2];
	
	pipe(childread);
	pipe(childwrite);
	
	pid = fork();
	if (pid == 0) //child
	{
		int recieved_max = 0;
		int recieved_id = 0;
		
		close(childread[1]);
		close(childwrite[0]);
		
		read(childread[0], &recieved_max, sizeof(int));
		read(childread[0], &recieved_id, sizeof(int));
		
		int failed = rand() % recieved_max; //0 == banana, 1 == frozen
		
		if (failed <= 1)
		{
			printf("Ez a %d. szerelveny, utanam meg %ddb lesz.\n",recieved_id, recieved_max - recieved_id);
			printf("Az uzem sikertelen volt\n");
		}
		else
		{
			printf("Ez a %d. szerelveny, utanam meg %ddb lesz.\n",recieved_id, recieved_max - recieved_id);
			printf("Az uzem sikeres volt\n");
		}
		
		sleep(3);
		
		if (failed <= 1)
		{
			kill(getppid(),SIGTERM);
			write(childwrite[1], &failed, sizeof(int));
		}
		else kill(getppid(),SIGINT);
		
	}
	else //parent
	{
		int id = rand() % 10;
		int child_failed;
		
		close(childread[0]);
		close(childwrite[1]);
		
		write(childread[1], &max, sizeof(int));
		write(childread[1], &id, sizeof(int));
		
		pause();
		
		read(childwrite[0], &child_failed, sizeof(int));
		if (child_failed == 0) printf("A szerelvény elcsuszott egy bananhelyon\n");
		else printf("A szerelvény megfagyott\n");
	}
}