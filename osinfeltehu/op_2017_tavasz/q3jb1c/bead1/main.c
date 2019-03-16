#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "readline.h"

#define LENGTH 100
#define INPUT_LENGTH 100
#define MAX_ANSWER 4
#define MIN_ANSWER 2

typedef struct Question {
  char question[LENGTH];
  char answers[MAX_ANSWER][LENGTH];
  int answerCount;
  char date[LENGTH];
} Question;

Question questions[LENGTH];
int questionCount = 0;

void printMenu () {
  printf("\n");
  printf("1. Show survey\n");
  printf("2. Add question\n");
  printf("3. Modify question\n");
  printf("4. Delete question\n");
  printf("5. Read from file\n");
  printf("6. Write to file\n");
  printf("7. Finalize\n");
  printf("8. Exit\n");
  printf("\n");
}

void printQuestion(int i){
  printf("Question %i: %s", i+1, questions[i].question);

  int j;
  for (j = 0; j < questions[i].answerCount; j++) {
    printf("Answer %i: %s", j+1, questions[i].answers[j]);
  }

  printf("Date: %s", questions[i].date);
}

void listQuestions(){
  printf("\n");

  int i;
  for (i = 0; i < questionCount; i++) {
    printQuestion(i);
    printf("\n");
  }
}

void newQuestion(){
  printf("Enter the question:\n");
  strcpy(questions[questionCount].question, readline());

  printf("How many answers will this question have? (2-4)\n");
  questions[questionCount].answerCount = atoi(readline());

  while (questions[questionCount].answerCount < MIN_ANSWER || questions[questionCount].answerCount > MAX_ANSWER) {
    printf("Please enter a number between 2 and 4.\n");
    questions[questionCount].answerCount = atoi(readline());
  }

  int i;
  for (i = 0; i < questions[questionCount].answerCount; i++) {
    printf("Answer %i: ", i+1);
    strcpy(questions[questionCount].answers[i], readline());
  }

  time_t t;
  time(&t);
  strftime(questions[questionCount].date, LENGTH, "%Y-%m-%d %H:%M:%S", localtime(&t));
  strcpy(questions[questionCount].date, strcat(questions[questionCount].date, "\n"));

  questionCount++;
}

void modifyQuestion(){
  listQuestions();

  printf("Which question would you like to modify?\n");
  int id = atoi(readline())-1;

  printf("The old question:\n%s", questions[id].question);
  printf("Enter the new question:\n");
  strcpy(questions[id].question, readline());

  printf("What is the new number of answers? (2-4)\n");
  questions[id].answerCount = atoi(readline());

  while (questions[id].answerCount < MIN_ANSWER || questions[id].answerCount > MAX_ANSWER) {
    printf("Please enter a number between 2 and 4.\n");
    questions[id].answerCount = atoi(readline());
  }

  int i;
  for (i = 0; i < questions[id].answerCount; i++) {
    printf("Previuos answer %i: %s", i+1, questions[id].answers[i]);

    printf("New answer %i: ", i+1);
    strcpy(questions[id].answers[i], readline());
  }
}

void deleteQuestion(){
  listQuestions();
  printf("Which question would you like to delete? (Enter it's entry!)\n");
  int id = atoi(readline());

  int i;
  for (i = id; i < questionCount-1; i++) {
    questions[i] = questions[i+1];
  }

  questionCount--;
}

void readFromFile(){
  printf("Name of file:\n");

  char filename[INPUT_LENGTH];
  scanf("%s", filename);

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }

  questionCount = 0;
  char line[LENGTH];

  while (fgets(line, LENGTH, file)) {
    strcpy(questions[questionCount].question, line);
    fgets(line, LENGTH, file);
    questions[questionCount].answerCount = atoi(line);

    int i;
    for (i = 0; i < questions[questionCount].answerCount; i++) {
      fgets(line, LENGTH, file);
      strcpy(questions[questionCount].answers[i], line);
    }

    fgets(line, LENGTH, file);
    strcpy(questions[questionCount].date, line);

    questionCount++;
  }

  fclose(file);
}

void writeToFile(){
  printf("Name of file to write: ");

  char filename[INPUT_LENGTH];
  scanf("%s", filename);

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

    fprintf(file, "%s", questions[i].date);
  }

  fclose(file);
}

void finalizeSurvey(){
  printf("Name of file for exporting to: ");

  char filename[INPUT_LENGTH];
  scanf("%s", filename);

  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error while opening file!\n");
    return;
  }

  int i;
  for (i = 0; i < questionCount; i++) {
    printQuestion(i);
    printf("\nDo you want to include this question? (y/n): ");

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

      fprintf(file, "%s", questions[i].date);
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
    printf("\n");

    if (choice[0] == '1') {
      listQuestions();
    } else if (choice[0] == '2') {
      newQuestion();
    } else if (choice[0] == '3') {
      modifyQuestion();
    } else if (choice[0] == '4') {
      deleteQuestion();
    } else if (choice[0] == '5') {
      readFromFile();
    } else if (choice[0] == '6') {
      writeToFile();
    } else if (choice[0] == '7') {
      finalizeSurvey();
    } else if (choice[0] == '8') {
      exit = 1;
    } else if (choice[0] != '\n') {
      printf("Unknown entry, try again!\n", choice);
    }
  }

  return 0;
}
