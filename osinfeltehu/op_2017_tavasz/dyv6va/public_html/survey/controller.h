#ifndef controller_h
#define controller_h

#include "time.h"
#include "model.h"

struct controller
{
	question_arr qa;
	FILE* f;
};

typedef struct controller controller;

void c_begin(controller* c);
void c_end_save(controller* c);
void c_end(controller* c);
void add_question(controller* c);
void edit_question(controller* c);
void delete_question(controller* c);
void print_questions(controller* c);
void finalize_questions(controller* c);

int get_int(int lower_bound, int upper_bound);

#endif
