#include "model.h"

// Constructor
void qa_init(question_arr* qa, int cap)
{
	if (cap == 0)
	{
		qa->capacity = ARR_MAX;
		qa->size = 0;
		qa->head = ALLOC(question, ARR_MAX);
	}
	else
	{
		qa->capacity = cap;
		qa->size = 0;
		qa->head = ALLOC(question, cap);
	}
}

// Add a new question item to the end of the list
void new_question(question_arr* qa, question *q)
{
	if(qa->size < qa->capacity)
	{
		copy(&qa->head[qa->size], q);
	}
	// If the current capacity is not enough, double the current size
	else
	{
		qa->capacity *= 2;
		question* tmp = qa->head;
		qa->head = ALLOC(question, qa->capacity);

		int i;
		for(i = 0; i < qa->size; i++)
		{
			qa->head[i] = tmp[i];
		}

		copy(&qa->head[qa->size], q);
	}

	qa->size++;
}

// Get the number of existing questions
int get_question_size(question_arr* qa)
{
	return qa->size;
}

// Return the question at pos
question* at(question_arr* qa, int pos)
{
	return &qa->head[pos];
}

// Print every question
void print_all(question_arr* qa)
{
	int i;
	for(i = 0; i < get_question_size(qa); i++)
	{
		printf("#%i: ", i + 1);
		print_question(at(qa, i));
		printf("\n");
	}
}

// Print a certain question
void print_question(question *q)
{
	printf("%s\n", q->ques);
	int j;
	for (j = 0; j < q->count; j++)
	{
		printf("%i. %s\n", j + 1, q->answers[j]);
	}
}

// Get the number of answers that belong to a certain question
int get_answer_size(question *q)
{
	return q->count;
}

// Get the actual question
char* get_ques(question* q)
{
	return q->ques;
}

// Get answer at pos
char* get_ans(question* q, int pos)
{
	return q->answers[pos];
}

void copy(question* lhs, question *rhs)
{
	strcpy(lhs->ques, rhs->ques);
	int i;
	for (i = 0; i < MAX_ANS; i++)
	{
		strcpy(lhs->answers[i], rhs->answers[i]);
	}
	lhs->count = rhs->count;
	strcpy(lhs->t, rhs->t);
}
