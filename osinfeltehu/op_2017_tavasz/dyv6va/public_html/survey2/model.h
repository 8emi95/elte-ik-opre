#ifndef model_h
#define model_h

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define ARR_MAX 500
#define LENGTH 200
#define MIN_ANS 2
#define MAX_ANS 4
#define ALLOC(t,n) (t *) malloc((n) * sizeof(t))

// Node representing questions (with MAX_ANS answer options)
struct question
{
	char ques[LENGTH];
	char answers[MAX_ANS][LENGTH];
	int count;
	char t[LENGTH];
};

typedef struct question question;

// Node representing the array of questions (has max size and capacity)
struct question_arr
{
	int capacity;
	int size;
	question* head;
};

typedef struct question_arr question_arr;

void qa_init(question_arr* qa, int cap);

void new_question(question_arr* qa, question *q);
int get_question_size(question_arr* qa);
question* at(question_arr* qa, int pos);
void print_all(question_arr* qa);

void print_question(question *q);
int get_answer_size(question *q);
char* get_ques(question* q);
char* get_ans(question* q, int pos);

void copy(question* lhs, question *rhs);
#endif
