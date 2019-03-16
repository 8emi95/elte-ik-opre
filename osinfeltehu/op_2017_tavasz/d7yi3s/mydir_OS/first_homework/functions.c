#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h> //opendir, readdir
#include <sys/types.h> //opendir
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h>
#include <memory.h>

#include "myStructs.h"
#include "functions.h"


int NumOfQuestions = 0;
struct  questionAndAnswers arrayofQStructs[1000];

int askForAction(){
	int actionInt;

	printf("Hello and welcome to the survey!\n\n"
			"What would you like to do?\n\n"
			"TYPE 1 for adding a question\n\n"
			"TYPE 2 for modifying a qustion\n\n" 
			"TYPE 3 for deleting question\n\n" 
			"\n");


	scanf("%i", &actionInt);
	
	return actionInt;

	
}


void addQuestionToFile(){
	printf("we are in the addquestiontofile. \n \n");

	NumOfQuestions++;

	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);
    strncpy(arrayofQStructs[NumOfQuestions-1].qctime, s, 1000);
	strcat(arrayofQStructs[NumOfQuestions-1].qctime, "\n");

	printf("Write your question: \n");
	scanf("%s", arrayofQStructs[NumOfQuestions-1].question);

	printf("Write answer to this question: \n");
	scanf("%s", arrayofQStructs[NumOfQuestions-1].answer1);

	char yorn;
	printf("Would you like to add answer2 ? (y/n) \n");
	scanf("%c", &yorn);

	if(yorn == 'y'){
		//ask for the 2answer
		printf("Write second answer to the question: \n");
		scanf("%s", arrayofQStructs[NumOfQuestions-1].answer2);

	}


	printf("Would you like to add answer3 ? (y/n) \n");
	scanf("%c", &yorn);

	if(yorn == 'y'){
		//ask for the 2answer
		printf("Write third answer to the question: \n");
		scanf("%s", arrayofQStructs[NumOfQuestions-1].answer3);

	}

	printf("You are done adding!\n");




	/*//writting in the file
	FILE* fp;
	fp = fopen("survey.bin", "a");
	if(fp != NULL){
		fwrite(&arrayofQStructs[NumOfQuestions-1], sizeof(struct questionAndAnswers), 1, fp);
		fclose(fp);
	}else{
		printf("the survey does not exist yet. we are starting now\n");
		fp = fopen("survey.txt", "w");
		fwrite(&arrayofQStructs[NumOfQuestions-1], sizeof(struct questionAndAnswers), 1, fp);
		fclose(fp);


	}*/
	
}



