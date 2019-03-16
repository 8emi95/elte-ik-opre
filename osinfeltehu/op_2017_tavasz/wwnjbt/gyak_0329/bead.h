#ifndef BEAD_H
#define BEAD_H

#define QUESTION_MAX_LENGTH 100
#define ANSWER_MAX_LENGTH 	100
#define ANSWER_MIN_COUNT	2
#define ANSWER_MAX_COUNT 	4

void readString(char* string, unsigned int maxLength);
int	 readPositiveInt(unsigned int min, unsigned int max);

void printMenu();

typedef struct Survey {
	struct SurveyNode* head;
	size_t size;
} Survey;

typedef struct SurveyNode {
	char 				question[QUESTION_MAX_LENGTH];
	char				questionDate[20];
	char 				answers[ANSWER_MAX_COUNT][ANSWER_MAX_LENGTH];
	struct SurveyNode*	next;
	int					answerCount;
} SurveyNode; 

int  initializeSurvey(Survey* survey);
void destroySurvey(Survey* survey);

void printSurvey(Survey* survey);
int  printNodeToScreen(SurveyNode* node);
int  printNodeToFile(SurveyNode* node, FILE* fp);
int  finalizeAndSaveSurvey(Survey* survey);

SurveyNode* getLastNode(Survey* survey);

int  addQuestion(Survey* survey, char* question);
int  deleteQuestion(Survey* survey, unsigned int nodeNumber);
int  modifyQuestion(Survey* survey, unsigned int nodeNumber, char* newQuestion);
void updateQuestionDate(SurveyNode* p);

int addAnswer(SurveyNode* node, char* answer);
int deleteAnswer(SurveyNode* node, unsigned int ansNumber);
int modifyAnswer(SurveyNode* node, unsigned int ansNumber, char* newAnswer);

int menuAddQuestionWithAnswers(Survey* survey);
int menuModifyQuestionAndAnswers(Survey* survey);

int menuModifyQuestion(Survey* survey, unsigned int nodeNumber);
int menuDeleteQuestion(Survey* survey);

int menuAddAnswer(SurveyNode* node, unsigned int ansNumber);
int menuModifyAnswer(SurveyNode* node, unsigned int ansNumber);
int menuEditAnswer(Survey* survey, unsigned int nodeNumber);
#endif