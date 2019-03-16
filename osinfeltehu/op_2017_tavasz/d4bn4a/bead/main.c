#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NEW(t) (t*)malloc(sizeof(t))

typedef struct {
  char text[256];
} answer_t;

#define MAX_ANSWERS 4

typedef struct {
  char text[256];
  answer_t answers[4];
  int answerCount;
  time_t time;
} question_t;

#define MAX_QUESTIONS 50

typedef struct {
  question_t questions[50];
  int questionCount;
} survey_t;

int menu();

int main(int argc, char const *argv[]) {
  menu();
  return 0;
}

survey_t* create_survey();
survey_t* load_survey();
int save_survey(survey_t* survey);
int destroy_survey(survey_t** survey);

int list_questions(survey_t* survey);
int add_question(survey_t* survey);
int remove_question(survey_t* survey);
int edit_question(survey_t* survey);

int print_question(question_t* question);
int dump_question(FILE* f, question_t* question);
int load_question(FILE* f, question_t* question);
int copy_question(question_t* dest, question_t* src);

int add_answer(survey_t* survey);
int remove_answer(survey_t* survey);
int edit_answer(survey_t* survey);

int print_answer(answer_t* answer);
int copy_answer(answer_t* dest, answer_t* src);

int help() {
  printf("Lehetőségek:\n"
    "  h|H - Kiírja ezt az üzenetet\n"
    "  n|N - Kérdéssor létrehozása\n"
    "  l|L - Kérdéssor betöltése\n"
    "  s|S - Kérdéssor mentése lemezre\n"
    "  3   - Kérdések listázása\n"
    "  4   - Kérdés felvétele\n"
    "  5   - Kérdés törlése\n"
    "  6   - Kérdés szerkesztése\n"
    "  7   - Válasz felvétele\n"
    "  8   - Válasz törlése\n"
    "  9   - Válasz módosítása\n"
    "  q|Q - Kilépés\n"
  );
}

int getLine(char* str) {
  char ch;
  while ((ch = getchar()) != '\n') {
    *str = ch;
    ++str;
  }
  return 0;
}

int menu() {
  help();
  char opt;
  survey_t* survey = NULL;
  do {
    char line[128];
    getLine(line);
    opt = line[0];
    switch (opt) {
    case 'h': case 'H':
      help();
      break;
    case 'n': case 'N':
      if (survey != NULL)
        destroy_survey(&survey);
      survey = create_survey();
      break;
    case 'l': case 'L':
      if (survey != NULL)
        destroy_survey(&survey);
      survey = load_survey();
      break;
    case 's': case 'S':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      save_survey(survey);
      break;
    case '3':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      list_questions(survey);
      break;
    case '4':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      add_question(survey);
      break;
    case '5':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      remove_question(survey);
      break;
    case '6':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      edit_question(survey);
      break;
    case '7':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      add_answer(survey);
      break;
    case '8':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      remove_answer(survey);
      break;
    case '9':
      if (survey == NULL) {
        printf("Jelenleg nincs kérdéssor\n");
        break;
      }
      edit_answer(survey);
      break;
    }
  } while (opt != 'q' && opt != 'Q');
  if (survey != NULL) {
    destroy_survey(&survey);
  }
  return 0;
}

int fgetLine(FILE* f, char* str) {
  char ch;
  while ((ch = fgetc(f)) != '\n') {
    *str = ch;
    ++str;
  }
  return 0;
}

survey_t* create_survey() {
  survey_t* survey = NEW(survey_t);
  return survey;
}

survey_t* load_survey() {
  char line[128];
  printf("Adja meg a fájlt nevét: ");
  getLine(line);
  FILE* f = fopen(line, "r");
  if (!f) {
    perror("Nem sikerült megnyitni a fájlt!");
    return NULL;
  }

  survey_t* s = create_survey();
  fgetLine(f, line);
  s->questionCount = atol(line);
  int i;
  for(i = 0; i < s->questionCount; ++i) {
    load_question(f, &s->questions[i]);
  }

  fclose(f);
  return 0;
}

int save_survey(survey_t* survey) {
  char line[128];
  printf("Adja meg a fájl nevét: ");
  getLine(line);
  FILE* f = fopen(line, "w");
  if (!f) {
    perror("Nem sikerült megnyitni a fájlt");
    return 1;
  }

  fprintf(f, "%d", survey->questionCount);
  int i;
  for (i = 0; i < survey->questionCount; ++i) {
    dump_question(f, &survey->questions[i]);
  }

  fclose(f);
  return 0;
}

int destroy_survey(survey_t** surveyp) {
  free(*surveyp);
  *surveyp = NULL;
  return 0;
}

int list_questions(survey_t* survey) {
  int i;
  for(i = 0; i < survey->questionCount; ++i) {
    printf("%d. Kérdés: ", i + 1);
    print_question(&survey->questions[i]);
  }
  return 0;
}

int add_question(survey_t* survey) {
  if (survey->questionCount >= MAX_QUESTIONS) {
    printf("Túllépte a maximális kérdések számát!");
    return 1;
  }
  question_t q;
  printf("Adja meg a kérdést: ");
  getLine(q.text);
  printf("Adjon meg két válasz lehetőséget\n");
  printf("Első: ");
  getLine(q.answers[0].text);
  printf("Második: ");
  getLine(q.answers[1].text);
  q.answerCount = 2;

  copy_question(&survey->questions[survey->questionCount], &q);
  ++survey->questionCount;
  return 0;
}

int remove_question(survey_t* survey) {
  char line[128];
  getLine(line);
  int index = atol(line);

  if (index <= 0 || index < survey->questionCount) {
    return 0;
  }
  if (index >= MAX_QUESTIONS) {
    return 0;
  }
  int i;
  for (i = index - 1; i < MAX_QUESTIONS - 1; ++i) {
    copy_question(&survey->questions[i], &survey->questions[i + 1]);
  }
  --survey->questionCount;
  return 0;
}

int edit_question(survey_t* survey) {
  printf("Melyik kérdést? ");
  char line[128];
  getLine(line);
  int index = atol(line);

  if (index <= 0 || index > survey->questionCount) {
    return 0;
  }
  if (index >= MAX_QUESTIONS) {
    return 0;
  }

  printf("Adja meg az új kérdés szöveget: ");
  getLine(line);
  strcpy(survey->questions[index - 1].text, line);

  return 0;
}


int print_question(question_t* question) {
  printf("%s\n", question->text);
  int i;
  for (i = 0; i < question->answerCount; ++i) {
    printf("%d. Válasz: ", i + 1);
    print_answer(&question->answers[i]);
  }
  return 0;
}

int dump_question(FILE* f, question_t* question) {
  fprintf(f, "%s\n", question->text);
  fprintf(f, "%d\n", question->answerCount);
  int i;
  for (i = 0; i < question->answerCount; ++i) {
    dump_answer(f, &question->answers[i]);
  }
  return 0;
}

int load_question(FILE* f, question_t* question) {
  char line[128];
  fgetLine(f, line);
  strcpy(question->text, line);

  fgetLine(f, line);
  question->answerCount = atol(line);

  int i;
  for (i = 0; i < question->answerCount; ++i) {
    fgetLine(f, line);
    strcpy(question->answers[i].text, line);
  }

  return 0;
}

int copy_question(question_t* dest, question_t* src) {
  int i;
  strcpy(dest->text, src->text);
  for (i = 0; i < MAX_ANSWERS; ++i) {
    copy_answer(&dest->answers[i], &src->answers[i]);
  }
  dest->answerCount = src->answerCount;
  dest->time = src->time;
  return 0;
}

int add_answer(survey_t* survey) {
  printf("Melyik kérdés? ");
  char line[128];
  getLine(line);
  int index = atol(line);

  if (index <= 0 || index > survey->questionCount) {
    return 0;
  }
  if (index >= MAX_QUESTIONS) {
    return 0;
  }

  question_t* q = &survey->questions[index - 1];
  if (q->answerCount == 4) {
    return 0;
  }
  printf("Mi az új válasz? ");
  getLine(q->answers[q->answerCount].text);
  ++q->answerCount;
  return 0;
}

int remove_answer(survey_t* survey) {
  printf("Melyik kérdés? ");
  char line[128];
  getLine(line);
  int index = atol(line);

  if (index <= 0 || index > survey->questionCount) {
    return 0;
  }
  if (index >= MAX_QUESTIONS) {
    return 0;
  }

  question_t* q = &survey->questions[index - 1];
  if (q->answerCount == 2) {
    return 0;
  }

  printf("Melyik választ töröljük? ");
  getLine(line);
  index = atol(line);
  if (0 <= index || index > q->answerCount) {
    return 0;
  }
  for(index = index - 1; index < MAX_ANSWERS - 1; ++index) {
    copy_answer(&q->answers[index], &q->answers[index + 1]);
  }
  --q->answerCount;
  return 0;
}

int edit_answer(survey_t* survey) {
  printf("Melyik kérdés? ");
  char line[128];
  getLine(line);
  int index = atol(line);

  if (index <= 0 || index > survey->questionCount) {
    return 0;
  }
  if (index >= MAX_QUESTIONS) {
    return 0;
  }

  question_t* q = &survey->questions[index - 1];
  if (q->answerCount == 2) {
    return 0;
  }

  printf("Melyik választ töröljük? ");
  getLine(line);
  index = atol(line);
  if (0 <= index || index > q->answerCount) {
    return 0;
  }
  printf("Mi az új válasz? ");
  getLine(q->answers[index - 1].text);
  return 0;
}

int print_answer(answer_t* answer) {
  printf("%s\n", answer->text);
}

int dump_answer(FILE* f, answer_t* answer) {
  fprintf(f, "%s\n", answer->text);
}

int copy_answer(answer_t* dest, answer_t* src) {
  strcpy(dest->text, src->text);
  return 0;
}
