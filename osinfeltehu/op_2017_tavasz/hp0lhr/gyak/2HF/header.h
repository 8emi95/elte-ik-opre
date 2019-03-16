#ifndef HEADER_H
#define HEADER_H

//_VARIABLES_//////////////////////////////////////////////////
#define POLLFILENAME "kerdoiv.txt"

#define MAX_QUESTIONS 20
#define QUESTION_LENGTH 64

#define MIN_ANSWERS 2
#define MAX_ANSWERS 4
#define ANSWER_LENGTH 32

#define PIPE_BUFFER_SIZE 256
#define QUESTIONS_TO_SEND_COUNT 3
#define CONSULTANT_COUNT_MIN 10
#define CONSULTANT_COUNT_MAX 20

typedef enum { false = 0, true = !false } bool;

struct Answers{
    int Count;
    char Text[MAX_ANSWERS][ANSWER_LENGTH];
};

struct Question{
	int Id;
	char Text[QUESTION_LENGTH];
	struct Answers TheAnswers;
	char TimeOfAddition[512];
};
/////////////////////////////////////////////////////////////

bool loadPollIfExists();

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

void displayPoll();
/////////////////////////////////////////////////////////////
struct PollStruct{
	struct Question FQS[QUESTIONS_TO_SEND_COUNT];
};
typedef struct PollStruct Poll;

void startPolling();
void createPipe(int*);
int generateIntBetween(int, int);
void addRandInt(int*, int, int);
bool checkDuplicates(int*, int);
void fillQuery(Poll*, struct Question*, int);
#endif //HEADER_H
