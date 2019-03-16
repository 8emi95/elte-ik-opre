#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>

#define LENGTH 100
#define INPUT_LENGTH 100
#define MAX_ANSWER 4
#define QUESTION_NUMBER 3

typedef struct Question {
	char question[LENGTH];
	char answers[MAX_ANSWER][LENGTH];
	int answerCount;
	char date[LENGTH];
	int votes[MAX_ANSWER];
} Question;

Question questions[LENGTH];
int questionCount = 0;

void swap (int a, int b){
	Question temp = questions[a];
	questions[a] = questions[b];
	questions[b] = temp;
}

void printQuestion(int i){
  printf("Question %i: %s", i+1, questions[i].question);

  int j;
  for (j = 0; j < questions[i].answerCount; j++) {
	printf(
      "votes: %i \t -Answer #%i: %s",
      questions[i].votes[j], j+1, questions[i].answers[j]
    );
    //printf("Answer %i: %s", j+1, questions[i].answers[j]);
  }

  printf("Date: %s", questions[i].date);
}

void listQuestions(){
  printf("\n");

  int i;
  for (i = 0; i < QUESTION_NUMBER; i++) {
    printQuestion(i);
    printf("\n");
  }
}

char *questionFIFO = "questionFIFO";
char *resultsFIFO = "resultsFIFO";

void child (){
	//sleep for 1 second
	sleep(1);
	//parent process id
	pid_t parentID = getppid();
	printf("PID of parent: %i\n", parentID);
	//parent sends a signal
	kill(parentID, SIGUSR1);
	  
	Question childQuestions[3];
	//open question file, read only
	int questionsPipe = open(questionFIFO, O_RDONLY);
	//read from pipe to store in parentQuestions
	read(questionsPipe, childQuestions, sizeof(childQuestions));

	int citizenCount = rand() % 10 + 11;

	int i;
	for (i = 0; i < citizenCount; i++) {
		int j;
		for (j = 0; j < QUESTION_NUMBER; j++) {
			int currentAnswer = rand() % childQuestions[j].answerCount;
			childQuestions[j].votes[currentAnswer]++;
		}
	}
	//open results file, write only
	int resultsPipe = open(resultsFIFO, O_WRONLY);
	//write to pipe from childQuestions
	write(resultsPipe, childQuestions, sizeof(childQuestions));
}

void writeQuestions(){
	//open question file, write only
	int pipe = open(questionFIFO, O_WRONLY);
	
	Question parentQuestions[QUESTION_NUMBER];
	int i;
	for(i=0; i<QUESTION_NUMBER; i++){
		parentQuestions[i] = questions[i];
	}
	
	//write to pipe from parentQuestions
	write(pipe, parentQuestions, sizeof(parentQuestions));
}

int parent(pid_t childID){
	//set writeQuestions() to handle SIGUSR1
	signal(SIGUSR1, writeQuestions);

	Question parentQuestions[QUESTION_NUMBER];
	//open result file, read only
	int pipe = open(resultsFIFO, O_RDONLY);
	//read from pipe to store in parentQuestions
	read(pipe, parentQuestions, sizeof(parentQuestions));

	int i;
	for(i=0; i<QUESTION_NUMBER; i++){
		questions[i] = parentQuestions[i];
	}

	printf("The results returned by the child for the random questions: \n");
	listQuestions();

	int state;
	//stops current process until child has changed state
	waitpid(childID, &state, 0);
	return state;
}

int readFromFile(){
  printf("Name of file:\n");

  char filename[INPUT_LENGTH];
  scanf("%s", filename);

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    return -1;
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
  return 0;
}

int main(){
	int f = readFromFile();
	if(f == -1){
		return -1;
	}
	
	//randomize seed for rand
	srand(time(NULL));
	
	int q;
	int i;
	for(i=0; i<QUESTION_NUMBER; i++){
		q = rand() % (questionCount - i) + i;
		swap(i, q);
	}
	
	//read, write, execute/search by owner
	//read, write, execute/search by group
	//read, write, execute/search by others
	mkfifo(questionFIFO, S_IRWXU | S_IRWXG | S_IRWXO);
	mkfifo(resultsFIFO, S_IRWXU | S_IRWXG | S_IRWXO);
	
	int state; 
	//create new process
	pid_t childID = fork();
	if (childID < 0) {
		perror("Cannot create child process.");
		exit(1);
	}
	else if (childID == 0) {
		child();
	} else {
		state = parent(childID);
		printf("Return state of child: %i\n", state);
	}

	//remove questionFIFO, resultsFIFO
	unlink(questionFIFO);
	unlink(resultsFIFO);
	
  return 0;
}
