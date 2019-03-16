#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <errno.h>
#include <dirent.h> //opendir, readdir
#include <sys/types.h> //opendir
#include <sys/stat.h> //stat
#include <unistd.h>   

#include "myStructs.h"


int main(){
	//int numberOfQuestions=0;
	int actionInt;

	//first ask the user what he wants to do
	actionInt = askForAction();
	printf("\n");
	

	//switch cases- according actionInt
	switch(actionInt){
		case 1:
			printf("You chosed to ADD a QUESTION:\n");
			addQuestionToFile();
			break;
		case 2:
			printf("You chosed to MODIFY A QUESTION:\n");
			break;
		case 3:
			printf("You chosed to DELETE A QUESTION:\n");
			break;
		default :
			printf("program terminated: wrong action number entered!!! Next time chose number between 1-6. \n");
			exit(1);

	}


	return 0;
}