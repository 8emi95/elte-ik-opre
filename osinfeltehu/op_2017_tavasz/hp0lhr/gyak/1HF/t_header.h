#ifndef T_HEADER_H
#define T_HEADER_H

//_VARIABLES_//////////////////////////////////////////////////
#define POLLFILENAME "kerdoiv.txt"

#define MAX_QUESTIONS 20
#define QUESTION_LENGTH 64

#define MIN_ANSWERS 2
#define MAX_ANSWERS 4
#define ANSWER_LENGTH 32

typedef enum { false = 0, true = !false } bool;

struct Answers{
    int Count;
    char Text[MAX_ANSWERS][ANSWER_LENGTH];
};

struct Question{
	int Id;
	char Text[QUESTION_LENGTH];
	struct Answers TheAnswers;
	char* TimeOfAddition;
};
/////////////////////////////////////////////////////////////

void loadPollIfExists();

void menu();
void displayMainMenu();
void waitForInput();

void newQuestion();
char* getText();
void addQuestion(char*);

void modifyQuestion();
void listQuestions();
int getId();
void modificationMenu(int);
void displayModificationMenu(int);

void deleteQuestion();

void listAnswers(int);
int getAnswerCount(int);
char* getAnswerText(int, int);

void newAnswer(int);

void deleteAnswer(int);

void modifyQuestionText(int);

void finalisePoll();
bool questionsHaveCorrectAnswerCount();
void writePollToFile();

#endif //T_HEADER_H
