#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


const char filename[] = "kerdoiv";
const char tempFilename[] = "kerdoiv.tmp";
const char finalFilename[] = "finalkerdoiv";

bool stillWaiting = true;

void clean()
{
  while (getchar() != '\n');
}

int getNum(char* str, int lo, int hi)
{
  int num;
  char *p;
  char numstr[100];
  do {
    printf("Number must be between %d and %d. %s", lo, hi, str);
    fgets(numstr, sizeof(numstr),stdin);
    num = strtol(numstr, &p, 10);
  } while (numstr[0] == '\n' || (*p != '\n' && *p != '\0') || num > hi || num < lo);
  return num;
}

struct Question
{
  char qtext[200];
  unsigned int numOfAns;
  char answers[4][200];
  time_t timeCreated;
};

void printQuestion(struct Question qest)
{
  printf("Text: %s", qest.qtext);
  int i;
  for (i = 0; i < qest.numOfAns; i++)
  {
    printf("%d. answer: %s", i+1, qest.answers[i]);
  }
  printf("Created at: %s\n", ctime(&qest.timeCreated));
  return;
}

int countFinal()
{
  struct Question qest;
  int f = open(finalFilename, O_RDONLY);
  if (f < 0)
  {
    return 0;
  }
  int i=0;
  while (read(f, &qest, sizeof(struct Question)))
  {
    i++;
  }
  close(f);
  return i;
}

int list()
{
  printf("--------\n");
  struct Question qest;
  int f = open(filename, O_RDONLY);
  if (f < 0)
  {
    printf("File does not exists. You should first create it by adding a question to it.\n");
    return -1;
  }
  int i=0;
  while (read(f, &qest, sizeof(struct Question)))
  {
    printf("%d. question:\n", i+1);
    printQuestion(qest);
    printf("\n");
    i++;
  }
  close(f);
  return i;
}

void add()
{
  struct Question qest;
  printf("Question text: ");
  //clean();
  fgets(qest.qtext, sizeof(qest.qtext), stdin);
  qest.numOfAns = getNum("Number of answers: ", 2, 4);
  unsigned int i=0;
  //clean();
  for (; i<qest.numOfAns; i++)
  {
    printf("\n%d. Answer: ", i+1);
    fgets(qest.answers[i], sizeof(qest.answers[i]), stdin);
  }
  qest.timeCreated = time(NULL);
  printf("\nQuestion created at time: %s", ctime(&qest.timeCreated));
  int f = open(filename, O_CREAT | O_WRONLY | O_APPEND, S_IWUSR | S_IRUSR);
  write(f, &qest, sizeof(struct Question));
  close(f);
  return;
}

void modify()
{
  int num = list();
  if (num == 0)
  {
    printf("No questions!\n");
    return;
  }
  int f = open(filename, O_RDWR); 
  if (num == 0 || f < 0)
  {
    return;
  }
  printf("Which question do you want to modify?\n");
  int qind = getNum("Index: ", 1, num);
  lseek(f, (qind-1)*sizeof(struct Question), SEEK_SET);
  struct Question qest;
  read(f, &qest, sizeof(struct Question));
  printf("Question:\n");
  printQuestion(qest);
  printf("Which part of the question do you want to modify?\n"
         "1 - Question text\n"
         "2 - Answers\n");
  int option = getNum("Option: ", 1, 2);
  switch(option)
  {
    case 1:
    printf("New question text: \n");
    //clean();
    fgets(qest.qtext, sizeof(qest.qtext), stdin);
    break;

    case 2:
    printf("1 - Add a new answer\n");
    printf("2 - Remove an answer\n");
    int j;
    for (j=0; j<qest.numOfAns; j++)
    {
      printf("%d - Modify %d. answer\n", 3+j, j+1);
    }
    option = getNum("Option: ", 1, 2+qest.numOfAns);
    switch (option)
    {
      case 1:
      if (qest.numOfAns > 3)
      {
        printf("There are 4 answers already!\n");
      } else
      {
        printf("New answer: \n");
        //clean();
        fgets(qest.answers[qest.numOfAns], 200, stdin);
        qest.numOfAns++;
      }
      break;

      case 2:
      if (qest.numOfAns < 3)
      {
        printf("There are only 2 answers to this question!\n");
      } else
      {
        int delans = getNum("Answer to delete: ", 1, qest.numOfAns);
        memcpy(qest.answers[delans-1], qest.answers[qest.numOfAns-1], 200);
        qest.numOfAns--;
      }
      break;

      default:
      printf("New answer text: \n");
      //clean();
      fgets(qest.answers[option-3], sizeof(qest.answers[option-2]), stdin);
      break;
    }
    break;
  }
  lseek(f, (qind-1)*sizeof(struct Question), SEEK_SET);
  write(f, &qest, sizeof(struct Question));
  close(f);
  return;
}

void delete()
{
  int qnum = list();
  if (qnum == 0)
  {
    printf("No questions!\n");
    return;
  }
  int f = open(filename, O_RDWR); 
  if (f < 0)
  {
    return;
  }
  int f2 = open(tempFilename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  printf("Which question do you want to delete?\n");
  int delq = getNum("Index: ", 1, qnum);
  struct Question qest;
  int i = 1;
  while (read(f, &qest, sizeof(struct Question)))
  {
    if (i != delq)
    {
      write(f2, &qest, sizeof(struct Question));
    }
    i++;
  }
  close(f2);
  close(f);
  rename(tempFilename, filename);
}

void finalize()
{
  int qnum = list();
  int f = open(filename, O_RDWR); 
  if (f < 0)
  {
    return;
  }
  int f2 = open(finalFilename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
  struct Question qest;
  while (read(f, &qest, sizeof(struct Question)))
  {
    printf("Do you want this question on the final sheet?\n");
    printQuestion(qest);
    printf("0 - no\n"
           "1 - yes\n");
    int ans = getNum("Option: ", 0, 1);
    if (ans)
    {
      write(f2, &qest, sizeof(struct Question));
    }
  }
  close(f2);
  close(f);
}

void handler(int signum)
{
  stillWaiting = false;
  printf("Interviewer logged in to MLSZ.\n");
}

void bead2()
{
  int status;
  printf("-----------\n");
  int numOfQs = countFinal();
  if (numOfQs < 3)
  {
    printf("Not enough Questions!\n");
    return;
  }

  char city[100];
  printf("Which city you want to send MLSZ representative to? ");
  fgets(city, sizeof(city), stdin);
  signal(SIGUSR1, handler);
  int toParent[2];
  int toChild[2];
  pipe(toParent);
  pipe(toChild);
  struct Question chosenQs[3];
  pid_t child=fork();
  int answers[3];
  if (child > 0) //MLSZ
  {
    while (stillWaiting)
    {
      usleep(1);
    }
    close(toParent[1]);
    close(toChild[0]);
    
    struct Question temp;
    int chosen[3] = {0,0,0};
    while (chosen[0] == chosen[1] || chosen[0] == chosen[2] 
            || chosen[2] == chosen[1])
    {
      int i;
      for (i=0; i<3; i++)
      {
        chosen[i] = rand() % numOfQs + 1;
      }
    }
    int f = open(finalFilename, O_RDONLY);
    int i=0;
    int j=0;
    while (read(f, &temp, sizeof(struct Question)))
    {
      i++;
      if (i==chosen[0] || chosen[1] == i || chosen[2] == i)
      {
        chosenQs[j] = temp;
        j++;
      }
    }
    close(f);
    
    printf("Chosen Questions:  \n");
    for(i=0; i<3; i++)
    {
      printQuestion(chosenQs[i]);
    }

    write(toChild[1], chosenQs, sizeof(chosenQs));
    int tester = 0;
    while (read(toParent[0], answers, sizeof(answers)) != 0)
    {
      printf("%d. answerer's answers: \n", tester+1);
      int i;
      for (i=0; i<3; i++)
      {
        printf("Answer to %d. question: %d\n", i, answers[i]);
      }
      tester++;
    }
    printf("MLSZ sent questions.\n");
    waitpid(child, &status, 0);
    close(toParent[0]);
    close(toChild[1]);
    stillWaiting = true;
  } else
  {
    close(toParent[0]);
    close(toChild[1]);
    printf("MLSZ sent interviewer to %s\n", city);
    kill(getppid(), SIGUSR1);
    read(toChild[0], chosenQs, sizeof(chosenQs));
    int numOfPeople = rand() % 11 + 10;
    int i;
    for (i=0; i<numOfPeople; i++)
    {
      int j;
      for (j=0; j<3; j++)
      {
        answers[j] = rand() % chosenQs[j].numOfAns + 1;
      }
      write(toParent[1], answers, sizeof(answers));
    }
    close(toParent[1]);
    close(toChild[0]);
    exit(0);
  }
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  bool q = false;
  while(!q)
  {
    //drawing menu
    printf("1 - List questions\n"
           "2 - Add question\n"
           "3 - Modify question\n"
           "4 - Delete question\n"
           "5 - Finalize sheet\n"
           "6 - Quit\n");

    //getting choice
    int option = getNum("Choice: ", 1, 6);

    //doing choice
    switch (option)
    {
      case 1:
      list();
      break;

      case 2:
      add();
      break;

      case 3:
      modify();
      break;

      case 4:
      delete();
      break;

      case 5:
      finalize();
      bead2();
      break;

      case 6:
      q = true;
      break;

      default:
      printf("Error! Entered value: %d\n", option);
      break;
    }
  }
}