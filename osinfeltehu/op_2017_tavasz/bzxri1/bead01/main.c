#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "QuestionnaireManager.h"
#include "Questionnaire.h"
#include "utility.h"
int main(void)
{
  QuestionnaireManager mainManager;
  questionnaireManagerInit(&mainManager);
	int running = 1;
	while(running)
	{
		printf("(1)Adding Question\n(2)Modify Question\n(3)Delete Question\n(4)List Questions and Answers\n(5)Confirm Questionnaire\n(6)Exit\n");
		printf("Type your command:");
		int cmdChoice = readIntWithBounds(1,6);
		switch (cmdChoice)
		{
		case 1:
		addQuestionAndAnswers(&mainManager);
			break;
		case 2:
			modifyQuestionnaire(&mainManager);
			break;
		case 3:
			deleteFromQuestionnaire(&mainManager);
			break;
		case 4:
			printQuestionnaireWithDate(&mainManager);
			break;
		case 5:
			confirmQuestionnaire(&mainManager);
			break;
		case 6:
			running = 0;
			break;
		default:
			break;
		}
	}
	freeMemory(&mainManager);
  return 0;
}
