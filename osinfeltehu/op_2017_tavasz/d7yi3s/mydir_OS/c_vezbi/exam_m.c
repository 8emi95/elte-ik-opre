#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

void handler1(int sig){
	printf("Child one ready to work!\n");
	signal(sig, handler1);
}

void handler2(int sig){
	printf("Child two ready to work!\n");
	signal(sig, handler2);
}

int main(int argc, char *argv[]){
	signal(SIGUSR1, handler1);
	signal(SIGUSR2, handler2);
	
	srand(time(NULL));
	
	pid_t child1, child2;
	
	int number_of_voters=atoi(argv[1]);
	
	
	child1 = fork();
	
	if(child1<0){
		perror("Error frok child1");
		exit(1);
	}
	
	if(child1 > 0){
		child2 = fork();
	}
	
	if(child1<0){
		perror("Error frok child2");
		exit(1);
	}
	
	if(child1 == 0){
		//child1 process
		sleep(1);
		kill(getppid(), SIGUSR1);
		
	}else if(child2 == 0){
		//child2 process
		sleep(2);
		kill(getppid(), SIGUSR2);
		
	}else{
		//parent process
		printf("The parents wait for signals from children\n");
		pause();
		printf("parent broke the first pause\n");
		pause();
		printf("parent broke the second pause\n");
		
		int ID[number_of_voters];
		int i;
		
		for(i=0; i<number_of_voters; i++){
			ID[i]=rand() % 1000+1001;
		}
		
		
	}
	
	
	
}