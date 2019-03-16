#include<fcntl.h>  
#include<sys/types.h> 
#include<sys/stat.h>
#include<errno.h> 
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include <signal.h>
#include "survey.h"

void ready_to_work(int sig){
	printf("The commisioner is ready to start working!\n");
	signal(sig, ready_to_work);
}

void shuffle(int array[], int n){
		
	 if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

struct QuestionForm get_q(int number){
	
	FILE *file=fopen("questionbank.txt", "r");
	
	char num[5];
	sprintf(num, "%d.", number);
	
	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));
	struct QuestionForm question;
	
	if(file==NULL) { printf("Open error");}
	
	while(fread(line, sizeof(struct QuestionForm), 1,file)){
			if(strcmp(line->question_number, num)==0){
				question=*line;
				break;
			}
		}
	
	free(line);
	return question;
}

int get_question_count(){
	
	int i=0;

	FILE *file=fopen("questionbank.txt", "r");
	
	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));
	
	if(file==NULL) { printf("Open error");}
	
	while(fread(line, sizeof(struct QuestionForm), 1,file)){
			i++;
		}
		
	free(line);	
	return i;	
}

void fill_arr(int array[], int n){
	int i;
	
	for(i=0; i<n; i++){
		array[i]=i+1;
	}
}

int main(int argc, char *argv[]){
	
	signal(SIGUSR1, ready_to_work);
	srand(time(NULL));
	
	int size=get_question_count();
	
	int array[size];
	fill_arr(array, size);
	shuffle(array, size);
	
	int pipe1[2], pipe2[2];
	int persons=rand() % 11 + 10;
	
	pid_t pid;
		
	if (pipe(pipe1)== -1 || pipe(pipe2)==-1){
        perror("Error creating pipe!");
        exit(EXIT_FAILURE);
    }
	
	int i;
	  
	pid = fork();
	 
    if (pid == -1) 
	{
        perror("Error at fork!");
        exit(EXIT_FAILURE);
    }  
	  
	if(pid==0){
		//the commisioner
		
		close(pipe1[1]); //close write
		close(pipe2[0]); //close read
		
		printf("The commisioner has arrived to %s.\n", argv[1]);
		kill(getppid(),SIGUSR1);
		
		struct QuestionForm rand_questions[3];
		
		read(pipe1[0],&rand_questions,sizeof(rand_questions));
		
		int answers[persons][3];
		
		int i,j;
		
		for(i=0; i<persons; i++){
			for(j=0; j<3; j++){
				int answer=rand() % number_of_answers(rand_questions[j]) +1;
				answers[i][j]=answer;
			}
		}
		
		write(pipe2[1], &answers, sizeof(answers));
		
		close(pipe1[0]); //close read
		close(pipe2[1]);
		
		exit(EXIT_SUCCESS);

	}else{
		//the society of hungarian ball games
		pause();
		
		close(pipe1[0]); //close read
		close(pipe2[1]); //close write
		
		struct QuestionForm rand_questions[3];
		int answers[persons][3];
		
		int i,j;
		
		for(i=0; i<3; i++){
			rand_questions[i]=get_q(array[i]);
		}
		
		write(pipe1[1], &rand_questions, sizeof(rand_questions));
	
		read(pipe2[0], &answers, sizeof(answers));
		
		printf("Results of the survey from %s!\n", argv[1]);
		
		for(i=0; i<persons; i++){
			for(j=0; j<3; j++){
				printf("For question %sPerson %d choose answer %d!\n", rand_questions[j].question, i+1, answers[i][j]);
			}
			printf("**************\n");
		}
	
		close(pipe1[1]); //close write
		close(pipe2[0]); //close read
		
	}
	
	return 0;
}