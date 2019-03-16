#ifndef Q_CREATION
#define Q_CREATION

struct Question
{
	char text[80];
	int numberOfAnswers;
	char** answers;
	char* timeOfCreation;
};

void printMenu();

void addQuestion(struct Question *questions, int numberOfQuestions);

void listQuestions(struct Question *questions, int numberOfQuestions);

void modifyQuestion(struct Question *questions, int numberOfQuestions);

void deleteQuestion(struct Question *questions, int numberOfQuestions);

#endif
