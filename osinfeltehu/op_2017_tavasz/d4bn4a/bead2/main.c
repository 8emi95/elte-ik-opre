#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

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

int is_ready(survey_t* surveyp);
int take_survey(survey_t* surveyp);

int help() {
  printf("Lehetőségek:\n"
    "  h|H - Kiírja ezt az üzenetet\n"
    "  n|N - Kérdéssor létrehozása\n"
    "  l|L - Kérdéssor betöltése\n"
    "  s|S - Kérdéssor mentése lemezre\n"
    "  v|V - Véglegesítés\n"
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

int prompt(char* str) {
  printf(">>> ");
  char ch;
  while ((ch = getchar()) != '\n') {
    *str = ch;
    ++str;
  }
  *str = '\0';
  return 0;
}

int menu() {
  help();
  char opt;
  survey_t* survey = NULL;
  do {
    char line[128];
    prompt(line);
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
    case 'v': case 'V':
      if (is_ready(survey) == 0) {
        if (take_survey(survey) == 1) {
          return 0;
        }
      } else {
        printf("A kérdőív nem tekinthető véglegesnek, "
               "ugyanis nincs meg minden kétdéshez a minimum 2 válasz lehetőség!");
      }
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
  *str = '\0';
  return 0;
}

survey_t* create_survey() {
  survey_t* survey = NEW(survey_t);
  return survey;
}

survey_t* load_survey() {
  char line[128];
  printf("Adja meg a fájlt nevét: ");
  prompt(line);
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
  return s;
}

int save_survey(survey_t* survey) {
  char line[128];
  printf("Adja meg a fájl nevét: ");
  prompt(line);
  FILE* f = fopen(line, "w");
  if (!f) {
    perror("Nem sikerült megnyitni a fájlt");
    return 1;
  }

  fprintf(f, "%d\n", survey->questionCount);
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
    printf("Túllépte a maximális kérdések számát!\n");
    return 1;
  }
  question_t q;
  printf("Adja meg a kérdést:\n");
  prompt(q.text);
  printf("Adjon meg két válasz lehetőséget\n");
  printf("Első:\n");
  prompt(q.answers[0].text);
  printf("Második:\n");
  prompt(q.answers[1].text);
  q.answerCount = 2;
  q.time = time(NULL);

  copy_question(&survey->questions[survey->questionCount], &q);
  ++survey->questionCount;
  return 0;
}

int remove_question(survey_t* survey) {
  char line[128];
  prompt(line);
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
  printf("Melyik kérdést?\n");
  char line[128];
  prompt(line);
  int index = atol(line);

  if (index <= 0 || index > survey->questionCount) {
    return 0;
  }
  if (index >= MAX_QUESTIONS) {
    return 0;
  }

  printf("Adja meg az új kérdés szöveget:\n");
  prompt(line);
  strcpy(survey->questions[index - 1].text, line);
  survey->questions[index - 1].time = time(NULL);

  return 0;
}


int print_question(question_t* question) {
  printf("%s - %s", question->text, ctime(&question->time));
  int i;
  for (i = 0; i < question->answerCount; ++i) {
    printf("%d. Válasz: ", i + 1);
    print_answer(&question->answers[i]);
  }
  return 0;
}

int dump_question(FILE* f, question_t* question) {
  fprintf(f, "%s\n", question->text);
  { // time
    struct tm* ti = gmtime(&question->time);
    printf("%d %d %d %d %d\n", ti->tm_year,
                                   ti->tm_mon,
                                   ti->tm_mday,
                                   ti->tm_hour,
                                   ti->tm_min);
    fprintf(f, "%d %d %d %d %d\n", ti->tm_year,
                                   ti->tm_mon,
                                   ti->tm_mday,
                                   ti->tm_hour,
                                   ti->tm_min);
  }
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
  { // time
    fgetLine(f, line);
    printf("\"%s\"\n", line);
    struct tm ti;
    ti.tm_sec = 0;
    sscanf(line, "%d %d %d %d %d", &ti.tm_year,
                                   &ti.tm_mon,
                                   &ti.tm_mday,
                                   &ti.tm_hour,
                                   &ti.tm_min);
    question->time = mktime(&ti);
  }

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
  printf("Melyik kérdés?\n");
  char line[128];
  prompt(line);
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
  printf("Mi az új válasz?\n");
  prompt(q->answers[q->answerCount].text);
  ++q->answerCount;
  return 0;
}

int remove_answer(survey_t* survey) {
  printf("Melyik kérdés?\n");
  char line[128];
  prompt(line);
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
  prompt(line);
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
  prompt(line);
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
  prompt(line);
  index = atol(line);
  if (0 <= index || index > q->answerCount) {
    return 0;
  }
  printf("Mi az új válasz? ");
  prompt(q->answers[index - 1].text);
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

int is_ready(survey_t* surveyp) {
  if (surveyp == NULL) {
    return -1;
  }
  if (surveyp->questionCount < 3) {
    return 1;
  }
  int i;
  for (i = 0; i < surveyp->questionCount; ++i) {
    if (surveyp->questions[i].answerCount < 2) {
      return 2;
    }
  }
  return 0;
}

#define READ_BIN(fd, resname, type) type resname; {\
  char buf[sizeof(type)];\
  read(fd, buf, sizeof(type));\
  memcpy(&resname, buf, sizeof(type));\
}

int take_survey(survey_t* surveyp) {
  srand(time(NULL));
  char* serverToClientName = "./server.to.client";
  char* clientToServerName = "./client.to.server";
  if (mkfifo(serverToClientName, S_IRUSR|S_IWUSR) != 0) {
    perror("ERR: `mkfifo` failed!");
    exit(1);
  }
  if (mkfifo(clientToServerName, S_IRUSR|S_IWUSR) != 0) {
    perror("ERR: `mkfifo` failed!");
    exit(2);
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("ERR: `fork` failed!");
    exit(3);
  }
  if (pid == 0) { // child
    question_t* questions[3];
    {
      int fd = open(serverToClientName, O_RDONLY);
      int i;
      for (i = 0; i < 3; ++i) {
        READ_BIN(fd, rd, int);
        questions[i] = &surveyp->questions[rd];
      }
      close(fd);
    }
    {
      int fd = open(clientToServerName, O_WRONLY);
      FILE* f = fopen("asd.txt", "w+");
      int count = (rand() % 10) + 11;
      fprintf(f, "%d\n", count);
      int i, j;
      write(fd, &count, sizeof(int));
      for (i = 0; i < 3; ++i) {
        question_t* q = questions[i];
        for (j = 0; j < count; ++j) {
          int r = rand() % q->answerCount;
          write(fd, &r, sizeof(int));
          fprintf(f, "%d\n", r);
        }
      }
      fclose(f);
      close(fd);
    }
    return 1;
  } else { // parent
    question_t* questions[3];
    {
      int fd = open(serverToClientName, O_WRONLY);
      int i = 0;
      do {
        int j = 0;
        int r = rand() % surveyp->questionCount;
        question_t* q = &surveyp->questions[r];
        for (; j < i; ++j) {
          if (questions[j] == q) {
            break;
          }
        }
        if (j == i) {
          questions[i] = q;
          i += 1;
          write(fd, &r, sizeof(int));
        }
      } while (i < 3);
      close(fd);
      unlink(serverToClientName);
    }
    {
      int fd = open(clientToServerName, O_RDONLY);
      READ_BIN(fd, count, int);
      printf("%d válasz érkezett:\n", count);
      int i, j;
      for (i = 0; i < 3; ++i) {
        question_t* q = questions[i];
        print_question(q);
        for (j = 0; j < count; ++j) {
          READ_BIN(fd, r, int);
          printf("válaszok: %d ", r + 1);
        }
        printf("\n");
      }
      close(fd);
      unlink(clientToServerName);
    }
  }
  return 0;
}
