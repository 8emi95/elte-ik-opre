#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "readline.h"

typedef struct Question {
  char question[100];
  char answers[4][100];
  int answerCount;
  char creationDate[100];
} Question;

Question questions[100];
int questionCount = 0;

void printMenu () {
  printf("-----\n");
  printf("1. List questions\n");
  printf("2. Add qestion\n");
  printf("3. Edit question\n");
  printf("4. Delete question\n");
  printf("5. Read from file\n");
  printf("6. Write to file\n");
  printf("7. Finalize (select a subset of questions into a file)\n");
  printf("8. Exit\n");
  printf("-----\n");
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
  for (i = 0; i < questionCount; i++) {
    printQuestion(i);
    printf("=====\n");
  }
}

void addQuestion () {
  printf("Enter the question:\n");
  strcpy(questions[questionCount].question, readline());

  printf("How many answers will this question have? (2-4)\n");
  questions[questionCount].answerCount = atoi(readline());

  while (questions[questionCount].answerCount < 2 || questions[questionCount].answerCount > 4) {
    printf("Please enter a number between 2 and 4.\n");
    questions[questionCount].answerCount = atoi(readline());
  }

  int i;
  for (i = 0; i < questions[questionCount].answerCount; i++) {
    printf("Answer #%i: ", i);
    strcpy(questions[questionCount].answers[i], readline());
  }

  time_t timer;
  time(&timer);

  strftime(questions[questionCount].creationDate, 100, "%Y-%m-%d %H:%M:%S", localtime(&timer));
  strcpy(questions[questionCount].creationDate, strcat(questions[questionCount].creationDate, "\n"));

  questionCount++;
}

void editQuestion () {
  listQuestions();

  printf("Whict question would you like to edit? (Only enter it's number!)\n");
  int questionId = atoi(readline());

  printf("The old question:\n%s", questions[questionId].question);
  printf("Enter the new question:\n");
  strcpy(questions[questionId].question, readline());

  printf("What is the new number of answers? (2-4)\n");
  questions[questionId].answerCount = atoi(readline());

  while (questions[questionId].answerCount < 2 || questions[questionId].answerCount > 4) {
    printf("Please enter a number between 2 and 4.\n");
    questions[questionId].answerCount = atoi(readline());
  }

  int i;
  for (i = 0; i < questions[questionId].answerCount; i++) {
    printf("Old answer #%i: %s", i, questions[questionId].answers[i]);

    printf("New answer #%i: ", i);
    strcpy(questions[questionId].answers[i], readline());
  }
}

void deleteQuestion () {
  listQuestions();
  printf("Which question would you like to delete? (Only enter it's number!)\n");
  int questionId = atoi(readline());

  int i;
  for (i = questionId + 1; i < questionCount; i++) {
    questions[i - 1] = questions[i];
  }

  questionCount--;
}

void readFile () {
  printf("name of file to read: ");

  char filename[100];
  scanf("%s", filename);
  // fgets(filename, 100, stdin);
  // filename = readline(); // this somehow puts an extra ? at the end of the filename...

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

void writeFile () {
  printf("Name of file to write: ");

  char filename[100];
  scanf("%s", filename);
  // fgets(filename, 100, stdin);
  // filename = readline(); // this somehow puts an extra ? at the end of the filename...

  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error while opening file!\n");
    return;
  }

  int i;
  for (i = 0; i < questionCount; i++) {
    fprintf(file, "%s", questions[i].question);
    fprintf(file, "%i\n", questions[i].answerCount);

    int j;
    for (j = 0; j < questions[i].answerCount; j++) {
      fprintf(file, "%s", questions[i].answers[j]);
    }

    fprintf(file, "%s", questions[i].creationDate);
  }

  fclose(file);
}

void finalize () {
  printf("Name of file for exporting to: ");

  char filename[100];
  scanf("%s", filename);

  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error while opening file!\n");
    return;
  }

  int i;
  for (i = 0; i < questionCount; i++) {
    printQuestion(i);
    printf("-----\nDo you want to include this question? (y/n): ");

    char *choice;
    choice = readline();

    while (choice[0] == '\n') {
      choice = readline();
    }

    while (!(choice[0] == 'y' || choice[0] == 'n')) {
      printf("Unknown answer. Do you want to include this question? (y/n): ");
      choice = readline();
    }

    if (choice[0] == 'y') {
      fprintf(file, "%s", questions[i].question);
      fprintf(file, "%i\n", questions[i].answerCount);

      int j;
      for (j = 0; j < questions[i].answerCount; j++) {
        fprintf(file, "%s", questions[i].answers[j]);
      }

      fprintf(file, "%s", questions[i].creationDate);
    }
  }

  fclose(file);
}

int main () {
  int exit = 0;
  char *choice;

  while (!exit) {
    if (choice[0] != '\n') {
      printMenu();
    }
    choice = readline();
    printf("-----\n");

    if (choice[0] == '1') {
      listQuestions();
    } else if (choice[0] == '2') {
      addQuestion();
    } else if (choice[0] == '3') {
      editQuestion();
    } else if (choice[0] == '4') {
      deleteQuestion();
    } else if (choice[0] == '5') {
      readFile();
    } else if (choice[0] == '6') {
      writeFile();
    } else if (choice[0] == '7') {
      finalize();
    } else if (choice[0] == '8') {
      exit = 1;
    } else if (choice[0] != '\n') {
      printf("Unrecognized command: '%s'. Try again!\n", choice);
    }
  }

  return 0;
}
