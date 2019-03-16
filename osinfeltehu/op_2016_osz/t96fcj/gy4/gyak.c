#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <stdlib.h>


void handler1(int signumber){
	printf("Hopp egy uzenet! Ez a 1. gyerektol jott!\n");
  
}
void handler2(int signumber){
	printf("Hopp egy uzenet! Ez a 2. gyerektol jott!\n");
  
}

int main (int argc,char* argv[]) {
	
	
	pid_t child1;
	pid_t child2;
	pid_t parent=getpid();
	
	signal(SIGUSR1,handler1);
	signal(SIGUSR2,handler2);
	int pipefd[2];
	if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
        }
		

	
	child1= fork();
	if(child1>0) //parent process 
	{
		child2=fork();
		if(child2>0) // parent process after both child started
		{
			
			int id[10];
			close(pipefd[0]);
			int clients=0;
			printf("Megvarom amig a gyerekek elindulnak! \n");
			pause();
			pause();
			printf("Megjottek a gyerekek! jej...\n");
			printf("Kerem adja meg hany szavazo lesz: ");
			
			scanf(" %i",&clients);
			printf("Legeneralom az azonositokat!\n");
			int i;
			for(i=0;i<clients;i++)
			{
				srand(time(NULL)+i);
				id[i] = rand()%100;
				printf(" Amit kuldok : %d \n",id[i]);
				write(pipefd[1],&id[i],sizeof(int));
			}
			int status;
			waitpid(child1,&status,0); 
			waitpid(child2,&status,0); 
			
			
			
		}
		else if (child2==0) //child2 process (Adatellenőzrő)
		{
			int numbers[10];
			sleep(2);
			kill(parent,SIGUSR2);
			int i;
			for(i=0;i<5;i++)
				read(pipefd[0],&numbers[i],sizeof(int));
			printf(" Megkaptam az adatokat! Itt vannak : \n");
			for(i=0;i<5; i++)
			{
				printf("%d.",i);
				printf("%d \n",numbers[i]);
			}
			
		}
		else if (child2<0) //failed to start child2;
		{
			printf("Failed to start child process!!!\n");
			
		}
		
		
	}else 	//child1 process
	{
		sleep(1);
		kill(parent,SIGUSR1);
		
		
	}
	
	
	 return 0;
}