#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// #include "readline.h"

typedef struct Question {
  char question[100];
  char answers[4][100];
  int answerCount;
  char creationDate[100];
} Question;

Question questions[100];
int questionCount = 0;

void readFile () {
  printf("Name of file to read questions from: ");

  //char filename[100];
  //scanf("%s", filename);
  
  char* filename = "questions.txt";

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }

  questionCount = 0;
  char line[256];

  while (fgets(line, sizeof(line), file)) {
    strcpy(questions[questionCount].question, line);
    fgets(line, sizeof(line), file);
    questions[questionCount].answerCount = atoi(line);

    int i;
    for (i = 0; i < questions[questionCount].answerCount; i++) {
      fgets(line, sizeof(line), file);
      strcpy(questions[questionCount].answers[i], line);
    }

    fgets(line, sizeof(line), file);
    strcpy(questions[questionCount].creationDate, line);

    questionCount++;
  }

  fclose(file);
}

void swap (int a, int b) {
  Question temp = questions[a];
  questions[a] = questions[b];
  questions[b] = temp;
}

void printQuestion (int i) {
  printf("Question #%i: %s", i, questions[i].question);

  int j;
  for (j = 0; j < questions[i].answerCount; j++) {
    printf("|-Answer #%i: %s", j, questions[i].answers[j]);
  }

  printf("Creation date: %s", questions[i].creationDate);
}

void listQuestions () {
  printf("=====\n");

  int i;
  for (i = 0; i < 3; i++) {
    printQuestion(i);
    printf("=====\n");
  }
}

int parent (pid_t childId) {
  int status;
  waitpid(childId, &status, 0);
  return status;
}

void child () {
  printf("child\n");
}

int main () {
  readFile();

  srand(time(NULL));
  
  int q = rand() % (questionCount);
  swap(0, q);
  q = rand() % (questionCount - 1) + 1;
  swap(1, q);
  q = rand() % (questionCount - 2) + 2;
  swap(2, q);

  printf("The randomly chosen questions: \n");
  listQuestions();

  int status; 
  pid_t childId = fork();
  if (childId < 0) {
    perror("Cannot create child process.");
    exit(1);
  }
  
  if (childId > 0) {
    parent(childId);
  }
  else {
    child();
  }

  return 0;
}
