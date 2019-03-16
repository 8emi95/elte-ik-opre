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


#define length 256

typedef struct qb{
    //DATE
    char qctime[100];
    //QNUMBER
    char qNumber[10];
    //TITLE
    char question[length];
    //ANSWERS
    char answer1[length];
    char answer2[length];
    char answer3[length];
    char answer4[length];
} QB;

const char fname[20] = "qustionBank.txt";
//void createIds();


void handler1(int sig){
	printf("Comisioner is ready to work!\n");
	signal(sig, handler1);
}

void handler2(int sig){
	printf("Comisioner ready to continue!\n");
	signal(sig, handler2);
}

int main(){
	//second part hw
	signal(SIGUSR1, handler1);
	signal(SIGUSR2, handler2);
	srand(time(NULL));
	
	int pipe1[2];
	int pipe2[2];
	
	if(pipe(pipe1)==-1){
		perror("Error in creating pipe!\n");
		exit(1);	
	}
	
	if(pipe(pipe2)==-1){
		perror("Error in creating pipe2!\n");
		exit(1);	
	}
	
	pid_t child = fork();
	
	if(child < -1){
		perror("Fork error");
		exit(1);
	}
	
	if(child>0){
		
		//parent process
		//pause();
		
		FILE* readfile = fopen(fname, "r");
		QB* qb = malloc(sizeof(QB));
		close(pipe1[0]);
		
		
		printf("The Society of Hungarian Ball Games chooses 3 random questions...\n");
		int rNumbers[3];
		int i;
		for(i=0; i<3; i++){
			rNumbers[i] = rand() % 3;
		}
		
		
		if(readfile == NULL){
				printf("The file that Sociaty of Hungarian Ball Games is trying to access, does not exits");
				exit(1);
		}
		else {
			while(fread(qb,sizeof(QB),1,readfile)) {
				for(i=0; i<3; i++){
					if(atoi(qb->qNumber) != rNumbers[i]) {
						//write to the pipe
						write(pipe1[1], &qb, sizeof(QB));
						close(pipe1[1]); //close the write end
						
					}
				
				}
			}
		}
		
		free(qb);
		printf("The Society of Hungarian Ball Games finihed and sent the questions to the comisioner.\n");
		
		kill(getppid(), SIGUSR2);
		wait();
		
		//now the parent will read and print the results
		
		int n;
		close(pipe2[1]);//closing the writing end
		read(pipe2[0], &n, sizeof(int));
		
		//people fill the questions randomply
		int q=1;
		while(q!=4){
			printf("For question %i people filled: \n", q);
			for(i=0; i<n; i++){
				int a= rand() % 4 + 1;
				printf("Person %i: %i answers", i, a);
			}
			q--;
		}
		
		
		
		
		
		
		
		
	}else if(child == 1){
		
		//comisioner, child process
		sleep(1);
		kill(getppid(), SIGUSR1);
		
		printf("Comisioner is waiting for the Sociaty of Hungarian Ball Game to chose the 3 random questions. ");
		pause();
		
		//choses 10-20 people randomly
		printf("Comisioner choses 10-20 random people\n");
		
		int n = rand() % 20 + 1;
		close(pipe2[0]); //closing reading end
		write(pipe2[1], &n, sizeof(int));
		close(pipe2[1]); //close the write end
		
		printf("Comisioner sent the result to the Sociaty of Hungarian Ball.\n");
		
		
		
		
	}
	
}
