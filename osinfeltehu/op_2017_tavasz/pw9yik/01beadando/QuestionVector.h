#ifndef QV_H
#define QV_H

#include <time.h>

struct Question
{
	char question[150];
	char answers[4][150];
	int answerCount;
	struct tm* date;
};

typedef struct Question Question;

void Qprint(Question *q);
void QaddAnswer(Question *q, char* str);
void QremoveAnswer(Question *q, int pos);
int QgetAnswerCount(Question *q);

struct QuestionVector
{
	int actualSize;
	int maxSize;
	Question* ptr;
};

typedef struct QuestionVector QuestionVector;

#define BASE_QV_SIZE 5

void QVinit(QuestionVector* qv);
void QVpush_back(QuestionVector* qv, Question *q);
void QVremove(QuestionVector* qv, int pos);
int QVsize(QuestionVector* qv);
Question* QVat(QuestionVector* qv, int pos);
void QVfree(QuestionVector* qv);

#endif