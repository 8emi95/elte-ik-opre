#ifndef controller_h
#define controller_h
#include "model.h"
#define NUM_NEEDED 3 // Number of questions needed

struct controller
{
	question_arr qa;
	question_arr selected;
	int sel_ind[3];
	FILE* f;
};

typedef struct controller controller;

void c_init(controller* c);
void select_questions(controller* c);
void print_questions(controller* c);

#endif