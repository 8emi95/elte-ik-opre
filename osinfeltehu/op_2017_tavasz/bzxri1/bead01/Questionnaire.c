#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "utility.h"
#include "Questionnaire.h"



void writeToQuestionnaire(char *toString)
{
	char *tempString = getlineQ();
	strcpy(toString, tempString);
	free(tempString);
}

void refreshDate(Questionnaire *qManager)
{
	time_t curTime = time(NULL);
	qManager->date = malloc(sizeof(struct tm));
	*qManager->date = *localtime(&curTime);
}

void printAnswers(Questionnaire *qManager)
{
	int j;
	for( j = 0; j < qManager->answerCounter; ++j)
	{
		printf("The %d.answer is: %s",j + 1, qManager->answer[j]);
	}
	printf("\n");
}

void modifyAnwers(Questionnaire *qManager)
{
	printAnswers(qManager);
	printf("\nType the index of the answer you would like to modify:");
	int chosenAnswer = readIntWithBounds(1,qManager->answerCounter);
	printf("Type your new answer:");
	switch (chosenAnswer) {
		case 1:
			writeToQuestionnaire(qManager->answer[0]);
			break;
		case 2:
			writeToQuestionnaire(qManager->answer[1]);
			break;
		case 3:
			writeToQuestionnaire(qManager->answer[2]);
			break;
		case 4:
			writeToQuestionnaire(qManager->answer[3]);
			break;
		}
}

void deleteAnswer(Questionnaire *qManager, int answerNumber)
{
	strcpy(qManager->answer[answerNumber],qManager->answer[qManager->answerCounter - 1]);
	qManager->answerCounter--;
}
