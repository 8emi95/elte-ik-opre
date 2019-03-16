#include "QuestionnaireManager.h"
#include "utility.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "string.h"
#include "unistd.h"
#include "signal.h"
#include "sys/types.h"
#include "sys/wait.h"


#define QUESTIONNAIRE_INIT_CAP 1

void questionnaireManagerInit(QuestionnaireManager *qManager)
{
	qManager->QUESTIONNAIRE_CAPACITY = QUESTIONNAIRE_INIT_CAP;
	qManager->QUESTIONNAIRE_SIZE = 0;
	qManager->questionnaireAr =
				(Questionnaire*)malloc(QUESTIONNAIRE_INIT_CAP * sizeof(Questionnaire));
}

void addQuestionAndAnswers(QuestionnaireManager *qManager)
{
	int actQuestionnaireSize = qManager->QUESTIONNAIRE_SIZE;
	if(actQuestionnaireSize >= qManager->QUESTIONNAIRE_CAPACITY)
	{
		reAllocateQuestionnaireAr(qManager);
	}
		addQuestion(qManager);
		printf("Type your answers, you have to give minimum of 2 answer and the maximum of 4 \n");
		addAnswers(qManager, actQuestionnaireSize);
}

void addQuestion(QuestionnaireManager *qManager)
{
	printf("Type your question:");
	int actQuestionnaireSize = qManager->QUESTIONNAIRE_SIZE;
	writeToQuestionnaire(qManager->questionnaireAr[actQuestionnaireSize].question);
	refreshDate(&qManager->questionnaireAr[actQuestionnaireSize]);
	qManager->questionnaireAr[actQuestionnaireSize].answerCounter = 0;
	qManager->QUESTIONNAIRE_SIZE++;
}

#define MAX_ANSWERS 4
#define ACCEPTABLE_QUESTIONNAIRE 1
void addAnswers(QuestionnaireManager *qManager, int questionNumber)
{
	if(qManager->questionnaireAr[questionNumber].answerCounter < MAX_ANSWERS)
	{
		int i;
		for (i = qManager->questionnaireAr[questionNumber].answerCounter; i < MAX_ANSWERS; ++i)
		{
	    printf("%d.answer is:",i + 1);
			writeToQuestionnaire(qManager->questionnaireAr[questionNumber].answer[i]);
	    qManager->questionnaireAr[questionNumber].answerCounter++;
			if (i >= ACCEPTABLE_QUESTIONNAIRE && i < MAX_ANSWERS - 1)
			{
				printf("You already added (%d) answer you can add (%d) more \n", i + 1, MAX_ANSWERS - i - 1);
				printf("Press 1 if you want to add more question and 2 if you don't: ");
				int choice = readIntWithBounds(1,2);
				if (choice != 1)
					break;
			}
		}
  }else
	{
		printf("You have reachead the maximum number of answers\n");
	}
}
void reAllocateQuestionnaireAr(QuestionnaireManager *qManager)
{
	qManager->QUESTIONNAIRE_CAPACITY *= 2;
	Questionnaire *copyAr = qManager->questionnaireAr;
	qManager->questionnaireAr = (Questionnaire*)malloc(qManager->QUESTIONNAIRE_CAPACITY * sizeof(Questionnaire));
	int i;
	for (i = 0; i < qManager->QUESTIONNAIRE_SIZE; ++i)
	{
 		qManager->questionnaireAr[i]  = copyAr[i];
	}
	free(copyAr);
}

void printQuestionnaire(QuestionnaireManager *qManager)
{
	if(qManager->QUESTIONNAIRE_SIZE == 0)
	{
		printf("There is nothing to display.\n");
	}else{
		int i;
		for( i = 0; i < qManager->QUESTIONNAIRE_SIZE; ++i)
		{
			Questionnaire actQuestionnaire = qManager->questionnaireAr[i];
			printf("The %d.question is:%s\n",i + 1,actQuestionnaire.question);
			printAnswers(&actQuestionnaire);
		}
	}
}
void printQuestionnaireWithDate(QuestionnaireManager *qManager)
{
	if(qManager->QUESTIONNAIRE_SIZE == 0)
	{
		printf("There is nothing to display.\n");
	}else{
		int i;
		for( i = 0; i < qManager->QUESTIONNAIRE_SIZE; ++i)
		{
			Questionnaire actQuestionnaire = qManager->questionnaireAr[i];
			printf("The %d.question is: %s\n",i + 1,actQuestionnaire.question);
			printAnswers(&actQuestionnaire);
			char buff[70];
			strftime(buff, sizeof buff, "%d/%m/%Y, %H:%M:%S", actQuestionnaire.date);
			printf("Last modified: %s\n", buff);
		}
	}
}

void modifyQuestionnaire (QuestionnaireManager *qManager)
{
	if(qManager->QUESTIONNAIRE_SIZE > 0)
	{
		printQuestionnaire(qManager);
		printf("Type the index of the question you would like to modify?\n>");
		int chosenQuestion = readIntWithBounds(1,qManager->QUESTIONNAIRE_SIZE);
		printf("(1)Modify question and answers\n(2)Modify answers\n>");
		int modifyChoice = readIntWithBounds(1,2);
		if(modifyChoice == 1)
		{
			printf("Type your new question:");
			writeToQuestionnaire(qManager->questionnaireAr[chosenQuestion - 1].question);
		}
		modifyAnswersAndQuestion(qManager,chosenQuestion);
		refreshDate(&qManager->questionnaireAr[chosenQuestion - 1]);
	}else{
		printf("There is no question to modify.\n");
	}
}

void modifyAnswersAndQuestion(QuestionnaireManager *qManager,int chosenQuestion)
{
	for(;;)
	{
		printf("(1)Adding answers\n(2)Modifying answers\n(3)Exit\n>");
		int cmdChoice = readIntWithBounds(1,3);
		if(cmdChoice == 1)
		{
			printf("Type your new answer:");
			addAnswers(qManager, chosenQuestion - 1);
		}else if(cmdChoice == 2)
		{
			modifyAnwers(&qManager->questionnaireAr[chosenQuestion - 1]);
		}else{
			break;
		}
	}
}

void deleteFromQuestionnaire(QuestionnaireManager *qManager)
{
		if(qManager->QUESTIONNAIRE_SIZE > 0)
		{
				printQuestionnaire(qManager);
				printf("Type the index of the question you would like to erase from.\n>");
				int chosenQuestion = readIntWithBounds(1,qManager->QUESTIONNAIRE_SIZE);
				printf("(1)Erase question\n(2)Erase answer\n(3)Exit\n>");
				int cmdChoice = readIntWithBounds(1,3);
				if(cmdChoice == 1)
				{
					deleteQuestion(qManager, chosenQuestion - 1);
				}else if(cmdChoice == 2)
				{
					if(qManager->questionnaireAr[chosenQuestion - 1].answerCounter > ACCEPTABLE_QUESTIONNAIRE + 1)
					{
						printAnswers(&qManager->questionnaireAr[chosenQuestion - 1]);
						printf("Type the index of the answer you would like to erase\n>");
						int chosenAnswer = readIntWithBounds(1,
									qManager->questionnaireAr[chosenQuestion - 1].answerCounter + 1);
						deleteAnswer(&qManager->questionnaireAr[chosenQuestion - 1],chosenAnswer);
						refreshDate(&qManager->questionnaireAr[chosenQuestion - 1]);
					}else
					{
						printf("You need minimum 3 answer to be able to erase.\n");
					}
			}
		}else{
			printf("There is no question to delete\n");
		}
}

void deleteQuestion(QuestionnaireManager *qManager, int questionNumber)
{
	free(qManager->questionnaireAr[questionNumber].date);
	qManager->questionnaireAr[questionNumber] =
							qManager->questionnaireAr[qManager->QUESTIONNAIRE_SIZE - 1];
	qManager->QUESTIONNAIRE_SIZE--;
}

void confirmQuestionnaire(QuestionnaireManager *qManager)
{
	if(qManager->QUESTIONNAIRE_SIZE == 0)
	{
		printf("There is nothing to display.\n");
	}else{
		qManager->f = fopen("data.txt", "w");
		int actSize = qManager->QUESTIONNAIRE_SIZE;
		int i;
		for( i = 0; i < actSize; ++i)
		{
			printf("The question is:%s",qManager->questionnaireAr[i].question);
			printf("\n");
			printAnswers(&qManager->questionnaireAr[i]);
			printf("Would you like this in the Questionnaire?\nPress (1) if you do and (2) if you don't\n>");
			int cmdChoice = readIntWithBounds(1,2);
			if(cmdChoice == 1)
			{
				Questionnaire actQuestionnaire = qManager->questionnaireAr[i];
				fputs("The question is:\n",qManager->f);
				fputs(actQuestionnaire.question, qManager->f);
				answersToFile(qManager,i);
				char buff[70];
				strftime(buff, sizeof buff, "%d/%m/%Y, %H:%M:%S", 
														actQuestionnaire.date);
				fputs(buff, qManager->f);
				fputs("\n", qManager->f);
			}else{
				deleteQuestion(qManager,i);
			}
		}
	}
}

void answersToFile(QuestionnaireManager *qManager, int questionNumber)
{
	Questionnaire actQuestionnaire = 
								qManager->questionnaireAr[questionNumber];
	int i;
	fputs("The answers are:\n",qManager->f);
	for(i = 0; i < actQuestionnaire.answerCounter; i++)
	{
		fputs(actQuestionnaire.answer[i], qManager->f);
	}
	fputs("\n", qManager->f);
}

void freeMemory(QuestionnaireManager *qManager)
{
	int i;
	for( i = 0; i < qManager->QUESTIONNAIRE_SIZE; ++i)
	{
		free(qManager->questionnaireAr[i].date);
	}
	free(qManager->questionnaireAr);
}

void sendToCommission(QuestionnaireManager *qManager)
{
	signal(SIGUSR1,handler);
	int pipefd[2];
	int finalizeAr[3][MAX_ANSWERS];
	pid_t child;
	if (pipe(pipefd) == -1) 
	{
	   perror("The pipe open has failed!");
	   exit(EXIT_FAILURE);
	}
	child = fork();
	if(child < 0)
	{
		perror("The fork open has failed");
		exit(EXIT_FAILURE);
	}
	if(child == 0){
		sleep(2);
		kill(getppid(),SIGUSR1);
		int childfinalizeAr[3][MAX_ANSWERS];
		int commissionerCount = (rand() % 11) + 10;
		Questionnaire questionnaireArForPipe[3];
		read(pipefd[0],questionnaireArForPipe, sizeof(questionnaireArForPipe));
		close(pipefd[0]);
		int k;
		for( k = 0; k < 3; ++k)
		{
			int l;
			for( l = 0; l < MAX_ANSWERS; ++l)
			{
				childfinalizeAr[k][l] = 0;
			}
		}
		int i, j;
		for ( i = 0; i < 3; ++i)
		{
			for(j = 0; j < commissionerCount; ++j)
			{
				int chosenAnswer = rand() % questionnaireArForPipe[i].answerCounter;
				childfinalizeAr[i][chosenAnswer]++;
			}
		}
		write(pipefd[1],childfinalizeAr,sizeof(childfinalizeAr));
		close(pipefd[1]);
	}else{
		pause();
		shrinkToFit(qManager);
		write(pipefd[1],qManager->questionnaireAr, 3 * sizeof(Questionnaire));
		int status;
		wait(&status);
		read(pipefd[0],finalizeAr,sizeof(finalizeAr));
		close(pipefd[0]);
		int i,j;
		/*
		for(i = 0; i < 3; ++i)
		{
			int j;
			for(j = 0; j < qManager->questionnaireAr[i].answerCounter; ++j)
			{
				printf("%d ", finalizeAr[i][j]);
			}
			printf("\n");
		}*/
		for(i = 0; i < 3; ++i)
		{
			printf("%d.Question is:%s",i + 1,qManager->questionnaireAr[i].question);
			for(j = 0; j < qManager->questionnaireAr[i].answerCounter; ++j)
			{
				strtok(qManager->questionnaireAr[i].answer[j],"\n");
				printf("%s answer got:%d vote \n", qManager->questionnaireAr[i].answer[j], finalizeAr[i][j]);
			}

		}
	}
}

void shrinkToFit(QuestionnaireManager *qManager)
{
	srand(time(NULL));
	while(qManager->QUESTIONNAIRE_SIZE > 3)
	{
		int randNumb = rand() % (qManager->QUESTIONNAIRE_SIZE);
		deleteQuestion(qManager, randNumb);
	}
}

void handler(int signumber){}


