#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "question.h"

#define QUESTIONS_FILE "questions.bin"

int save_questions(const question_list* list);
int load_questions(question_list* list);

#endif // PERSISTENCE_H