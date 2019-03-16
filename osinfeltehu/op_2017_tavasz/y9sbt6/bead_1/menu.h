#ifndef MENU_H
#define MENU_H

#include "question.h"

#define VERT_SPACER "---------------------------------------------\n"

void menu_print(void);
void menu_run(question_list* list);

void menu_read_qstn_text(question*);
void menu_read_answ_num(question*, const size_t, const size_t);
void menu_read_answers(question*);
int menu_select_qstns(question_list*, int);
void bubble_sort(size_t*, int);
void rem_dupl(size_t*, int*);

void menu_add_qstn(question_list*);
void menu_modify_qstn(question_list*);
void menu_remove_qstn(question_list*);
void menu_print_qstns(const question_list*);
void menu_compose_list(question_list*);
void menu_print_composed_list(const question_list*);
void menu_save(const question_list*);

#endif // MENU_H
