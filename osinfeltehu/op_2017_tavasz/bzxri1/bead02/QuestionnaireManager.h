#ifndef _QUESTIONNAIRE_MANAGER_
#define  _QUESTIONNAIRE_MANAGER_

#include "stdio.h"
#include "Questionnaire.h"

typedef struct QuestionnaireManager{
	int QUESTIONNAIRE_CAPACITY;
	int QUESTIONNAIRE_SIZE;
	Questionnaire *questionnaireAr;
	FILE *f;
}QuestionnaireManager;

void questionnaireManagerInit(QuestionnaireManager * qManager);
void addQuestion(QuestionnaireManager *qManager);
void addAnswers(QuestionnaireManager *qManager, int questionNumber);
void reAllocateQuestionnaireAr(QuestionnaireManager * qManager);
void addQuestionAndAnswers(QuestionnaireManager * qManager);
void addQuestion(QuestionnaireManager *qManager);
void printQuestionnaire(QuestionnaireManager *qManager);
void printQuestionnaireWithDate(QuestionnaireManager *qManager);
void modifyAnswersAndQuestion(QuestionnaireManager *qManager,int chosenQuestion);
void modifyQuestionnaire (QuestionnaireManager *qManager);
void deleteQuestion(QuestionnaireManager *qManager, int questionNumber);
void deleteFromQuestionnaire(QuestionnaireManager *qManager);
void answersToFile(QuestionnaireManager *qManager, int questionNumber);
void confirmQuestionnaire(QuestionnaireManager *qManager);
void freeMemory(QuestionnaireManager *qManager);
void shrinkToFit(QuestionnaireManager *qManager);
void sendToCommission(QuestionnaireManager *qManager);
void handler(int signumber);

#endif
