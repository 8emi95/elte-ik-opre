#include "all.h"

typedef struct Question {
  char question[100];
  char answers[4][100];
  int answerCount;
  char creationDate[100];
  float votes[4];
} Question;

Question questions[100];
int questionCount = 0;

int readFile () {
  printf("Name of file to read questions from: ");
  char filename[100];
  scanf("%s", filename);
  
  //char* filename = "questions.txt";

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    return -1;
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
  return 0;
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
    printf(
      "votes: %f \t |-Answer #%i: %s",
      questions[i].votes[j], j, questions[i].answers[j]
    );
  }
}

void listQuestions () {
  printf("=====\n");

  int i;
  for (i = 0; i < 3; i++) {
    printQuestion(i);
    printf("=====\n");
  }
}

char *questionFIFO = "questionFIFO";
char *resultsFIFO = "resultsFIFO";

void child () {
  sleep(1);
  pid_t parentId = getppid();
  printf("PID of parent: %i\n", parentId);
  kill(parentId, SIGUSR1);
  
  Question childQuestions[3];
  int questionsPipe = open(questionFIFO, O_RDONLY);
  read(questionsPipe, childQuestions, sizeof(childQuestions));

  int citizenCount = rand() % 10 + 11;

  int i;
  for (i = 0; i < citizenCount; i++) {
    int j;
    for (j = 0; j < 3; j++) {
      int currentAnswer = rand() % childQuestions[j].answerCount;
      childQuestions[j].votes[currentAnswer]++;
    }
  }
  
  int resultsPipe = open(resultsFIFO, O_WRONLY);
  write(resultsPipe, childQuestions, sizeof(childQuestions));
}

void sendQuestions () {
  int questionsPipe = open(questionFIFO, O_WRONLY);
  
  Question parentQuestions[3];
  parentQuestions[0] = questions[0];
  parentQuestions[1] = questions[1];
  parentQuestions[2] = questions[2];
  
  write(questionsPipe, parentQuestions, sizeof(parentQuestions));
}

int parent (pid_t childId) {
  signal(SIGUSR1, sendQuestions);

  Question parentQuestions[3];
  int resultsPipe = open(resultsFIFO, O_RDONLY);
  read(resultsPipe, parentQuestions, sizeof(parentQuestions));

  questions[0] = parentQuestions[0];
  questions[1] = parentQuestions[1];
  questions[2] = parentQuestions[2];

  printf("The results returned by the child for the random questions: \n");
  listQuestions();

  int status;
  waitpid(childId, &status, 0);
  return status;
}

int main () {
  int success = readFile();
  if (success == -1) {
    return -1; 
  }

  srand(time(NULL));
  
  int q;
  q = rand() % (questionCount - 0) + 0;
  swap(0, q);
  q = rand() % (questionCount - 1) + 1;
  swap(1, q);
  q = rand() % (questionCount - 2) + 2;
  swap(2, q);

  mkfifo(questionFIFO, 0777);
  mkfifo(resultsFIFO, 0777);

  int status; 
  pid_t childId = fork();
  if (childId < 0) {
    perror("Cannot create child process.");
    exit(1);
  }
  
  if (childId == 0) {
    child();
  } else {
    status = parent(childId);
    printf("Return status of child: %i\n", status);
  }

  unlink(questionFIFO);
  unlink(resultsFIFO);

  return 0;
}
