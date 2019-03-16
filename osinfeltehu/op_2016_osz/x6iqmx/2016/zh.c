#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>

void handler()
{
	printf("Az adatok megerkeztek!");
}

int main(int argc,char** argv) {
	if(argc < 2)
	{
		 printf("Kell egy parameter!\n");
          return;
	}
	else
	{
		struct Message
		{
			char name[200];
			int task;
			int id;
		};
		
		int status;
		
		int pipefd[2];
		
		
		if(pipe(pipefd) == -1) {
			printf("PIPE HIBA");
			exit(2);
		}
		
		int child = fork();
		
		if(fork < 0) 
		{
			printf("FORK HIBA");
			exit(1);
		}
		
		if(child > 0) 
		{
			//char* name = argv[];
			struct Message m1 = {*argv[1], 1,1};
			write(pipefd[1], &m1, sizeof(m1));
			close(pipefd[1]);
			printf("[Trener] : Elkuldtem a feladatot!\n");	
			
			signal(SIGUSR1, handler);
			pause();
			
			int result;
			read(pipefd[0],&result, sizeof(result));
			printf("\n[Trener] : Az eredmenye : %d\n", result);
			close(pipefd[0]);
		}
		else 
		{
			int parent = getppid();
			struct Message m1;
			read(pipefd[0], &m1, sizeof(m1));
			printf("[Versenyzo] : Nev: %s\n Feladat: %d\n Sorszamom: %d\n", m1.name, m1.task, m1.id);
			close(pipefd[0]);		
			
			srand ( time(NULL) );
			int randomnumber;
			randomnumber = rand() % 100;
			printf("[Versenyzo]: ennyire sikerult megoldani : %d\n", randomnumber);		
			write(pipefd[1], &randomnumber, sizeof(randomnumber));
			kill(parent , SIGUSR1);
			sleep(1);
			close(pipefd[1]);		
			
			exit(0);
		}
		
		return 0;
	}
}