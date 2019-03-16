#ifndef _QUESTIONNAIRE_
#define _QUESTIONNAIRE_

typedef struct Questionnaire{
	char question[250];
	char answer[4][250];
	int answerCounter;
	struct tm* date;
}Questionnaire;

void refreshDate(Questionnaire *qManager);
void printAnswers(Questionnaire *qManager);
void printAnswers(Questionnaire *qManager);
void modifyAnwers(Questionnaire *qManager);
void deleteAnswer(Questionnaire *qManager, int answerNumber);
void writeToQuestionnaire(char *toString);
#endif
