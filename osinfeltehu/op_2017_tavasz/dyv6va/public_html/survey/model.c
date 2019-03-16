#include "model.h"

// Constructor
void qa_begin(question_arr* qa)
{
	qa->capacity = ARR_MAX;
	qa->size = 0;
	qa->head = ALLOC(question, ARR_MAX);
}

// Destructor
void qa_end(question_arr* qa)
{
	free(qa->head);
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
		free(tmp);
	}

	qa->size++;
}

// Remove item at position pos
void remove_question(question_arr* qa, int pos)
{
	if (qa->size == 0)
		return;

	int i;
	for (i = pos; i < qa->size - 1; i++)
	{
		qa->head[i] = qa->head[i + 1];
	}

	qa->size--;
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

// Add a new answer option to an existing question
void add_answer(question *q, char* str)
{
	if (q->count < MAX_ANS)
	{
		strcpy(q->answers[q->count], str);
		q->count++;
	}
	else
	{
		printf("The limit for possible answer options is %i.\n\n", MAX_ANS);
	}
}

// Remove an answer option
void remove_answer(question *q, int pos)
{
	if (q->count > MIN_ANS)
	{
		int i;
		for (i = pos; i < q->count - 1; i++)
		{
			strcpy(q->answers[i], q->answers[i + 1]);
		}
		q->count--;
	}
	else
	{
		printf("At least two answer options are required.");
	}
}

// Get the number of answers that belong to a certain question
int get_answer_size(question *q)
{
	return q->count;
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
	lhs->t = rhs->t;
}
