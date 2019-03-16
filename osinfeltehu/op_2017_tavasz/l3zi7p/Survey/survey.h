#ifndef SV_H
#define SV_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

typedef struct Question Question;

struct Question {
  char question[100];
  char a[100]; //Nevezzük el őket külön, csak hogy csúnya legyen a kód ;)
  char b[100];
  char c[100];
  char d[100];
  int n;
  int f;
  char date[100];

  Question *next;
};

void Q_init(Question* q);
void Q_free(Question* q);
void Q_check(Question* q);
void Q_set(Question* q, char* entry, char* value);
void Q_finalize(Question* q);



void Q_print(Question* q, int index);

void Q_print_final(Question* q, int index);

typedef struct Survey Survey;

struct Survey {
  Question* first;
  int length;
};

void S_init(Survey* s);
void S_add(Survey* s, Question* q);
Question* S_get_nth_question(Survey* s, int n);
void S_edit_nth_question(Survey* s, int n, char* entry, char* value);
void S_delete_nth_question(Survey* s, int n);
void S_print(Survey* s);
void S_print_final(Survey* s);
void S_free(Survey* s);
void S_read(Survey* s);
void S_write(Survey* s);

#endif
