#include "survey.h"



void Q_init(Question* q){
  strcpy(q->question , "");
  strcpy(q->a , "");
  strcpy(q->b , "");
  strcpy(q->c , "");
  strcpy(q->d , "");
  q->n = 0;
  q->f = 0;
  q->next = 0;

  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strcpy(q->date , asctime (timeinfo));
  char *pos;
  if ((pos=strchr(q->date, '\n')) != NULL) *pos = '\0';
}

void Q_free(Question* q){
  free(q->next);
}

void Q_check(Question* q){
  int n = 0;
  if (strcmp(q->a , "")) n++;
  if (strcmp(q->b , "")) n++;
  if (strcmp(q->c , "")) n++;
  if (strcmp(q->d , "")) n++;
  q->n = n;
}

void Q_set(Question* q, char* entry, char* value){
  if (strcmp(entry,"q") == 0) {
    strcpy(q->question, value);
  } else if (strcmp(entry,"a") == 0){
    strcpy(q->a, value);
  } else if (strcmp(entry,"b") == 0){
    strcpy(q->b, value);
  } else if (strcmp(entry,"c") == 0){
    strcpy(q->c, value);
  } else if (strcmp(entry,"d") == 0){
    strcpy(q->d, value);
  } else if (strcmp(entry,"f") == 0){
    if (strcmp(value,"0")){
      q->f = 1;
    }
  } else if (strcmp(entry,"date") == 0){
    strcpy(q->date, value);
  }
  Q_check(q);
}

void Q_finalize(Question* q){
  if (q->n >= 2) q->f = 1;
}



void Q_print(Question* q, int index){
  char f = ' ';
  if (q->f) f = 'F';
  printf("[%c]----------------------------------\n",f);
  printf(" Added: %s\n", q->date);
  if (strcmp(q->question,"")) {printf("  (%i). %s\n", index, q->question);} else {printf("(%i). [Empty]\n", index);}
  if (strcmp(q->a,"")) {printf("    a, %s\n", q->a);} else {printf("    a, [Empty]\n");}
  if (strcmp(q->b,"")) {printf("    b, %s\n", q->b);} else {printf("    b, [Empty]\n");}
  if (strcmp(q->c,"")) {printf("    c, %s\n", q->c);} else {printf("    c, [Empty]\n");}
  if (strcmp(q->d,"")) {printf("    d, %s\n", q->d);} else {printf("    d, [Empty]\n");}
  if (q->n < 2) {printf("  Give atleast 2 options!\n");}
}

void Q_print_final(Question* q, int index){
  if (q->f){
    printf("-------------------------------------\n");
    if (strcmp(q->question,"")) {printf("(%i). %s\n", index, q->question);}
    if (strcmp(q->a,"")) {printf("  a, %s\n", q->a);}
    if (strcmp(q->b,"")) {printf("  b, %s\n", q->b);}
    if (strcmp(q->c,"")) {printf("  c, %s\n", q->c);}
    if (strcmp(q->d,"")) {printf("  d, %s\n", q->d);}
  }
}



void S_init(Survey* s){
  s->first = 0;
  s->length = 0;
}

void S_add(Survey* s, Question* q){

  if (s->first){
    Question* p = s->first;
    while (p->next) {
      p = p->next;
    }
    p->next = q;
    s->length++;
  } else {
    s->first = q;
    s->length++;
  }
}

Question* S_get_nth_question(Survey* s, int n){
  if (s->first){
    int i = 1;
    Question* p = s->first;
    while (p->next && i != n) {
      p = p->next;
      i++;
    }
    return p;
  } else {
    return 0;
  }
}

void S_edit_nth_question(Survey* s, int n, char* entry, char* value){
  if (n <= s->length){
    Q_set( S_get_nth_question(s,n) , entry, value);
  } else if (n == s->length + 1){
    Question *q = (Question*)malloc(sizeof(struct Question));
    Q_init(q);
    Q_set(q , entry, value);
    S_add(s,q);
  }
}

void S_delete_nth_question(Survey* s, int n){
  Question* q = S_get_nth_question(s,n-1);
  Question* p = S_get_nth_question(s,n);
  q->next = p->next;
  free(p);
  s->length--;
}

void S_print(Survey* s){
  Question* p = s->first;
  int index = 1;
  while (p) {
    Q_print(p,index);
    printf("\n");
    p = p->next;
    index++;
  }
}

void S_print_final(Survey* s){
  Question* p = s->first;
  int index = 1;
  while (p) {
    Q_print_final(p,index);
    printf("\n");
    p = p->next;
    index++;
  }
}

void S_free(Survey* s){
  while(s->first->next){
    Question* p = s->first;
    Question* q = p;
    while (p->next) {
      q = p;
      p = p->next;
    }
    q->next = 0;
    free(p);
    s->length--;
  }
  free(s->first);
}

void S_write(Survey* s){
  FILE *f = fopen("file.txt", "w");
  if (f == NULL)
  {
      printf("Error opening file!\n");
      exit(1);
  }

  int i;
  for (i=1; i <=s->length; i++){
    fprintf(f, "%s\n", S_get_nth_question(s,i)->question);
    fprintf(f, "%s\n", S_get_nth_question(s,i)->a);
    fprintf(f, "%s\n", S_get_nth_question(s,i)->b);
    fprintf(f, "%s\n", S_get_nth_question(s,i)->c);
    fprintf(f, "%s\n", S_get_nth_question(s,i)->d);
    fprintf(f, "%i\n", S_get_nth_question(s,i)->f);
    fprintf(f, "%s\n", S_get_nth_question(s,i)->date);
  }

  fclose(f);
}

void S_read(Survey* s){
    FILE* file = fopen("file.txt", "r"); /* should check the result */
    char line[100];
    while (fgets(line, sizeof(line), file)) {
      int i = s->length + 1;
      char *pos;
      if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
      S_edit_nth_question(s,i, "q", line);
      fgets(line, sizeof(line), file);
      if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
      S_edit_nth_question(s,i, "a", line);
      fgets(line, sizeof(line), file);
      if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
      S_edit_nth_question(s,i, "b", line);
      fgets(line, sizeof(line), file);
      if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
      S_edit_nth_question(s,i, "c", line);
      fgets(line, sizeof(line), file);
      if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
      S_edit_nth_question(s,i, "d", line);
      fgets(line, sizeof(line), file);
      if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
      S_edit_nth_question(s,i, "f", line);
      fgets(line, sizeof(line), file);
      if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
      S_edit_nth_question(s,i, "date", line);
    }
    fclose(file);
}
