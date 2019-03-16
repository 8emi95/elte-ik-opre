#ifndef QUESTION_H
#define QUESTION_H

#define MAX_QUESTION_LENGTH (size_t)100
#define MAX_ANSWER_LENGTH   (size_t)40
#define MIN_ANSWER_NUM      (size_t)2
#define MAX_ANSWER_NUM      (size_t)4

#define QSTN_SELECTED       (int)1
#define QSTN_NOT_SELECTED   (int)0

#include <time.h>

typedef struct question {
    size_t  id;
    size_t  answer_num;
    int     selected;
    char    text[MAX_QUESTION_LENGTH];
    char    answers[MAX_ANSWER_NUM][MAX_ANSWER_LENGTH];
    time_t  registered;
} question;

typedef struct question_node {
    question    qstn;
    struct question_node*   prev;
    struct question_node*   next;
} question_node;

typedef struct question_list {
    size_t          number;
    question_node*  head;
    question_node*  last;
} question_list;

void print_qstn(const question*);
void clear_answers(question*);
void insert_qstn(question_list*, question*);
void modify_qstn(question*, const question*);
int remove_qstn(question_list*, size_t);
void init_qstn_list(question_list*);
question* find_qstn(question_list*, const size_t);

#endif // QUESTION_H