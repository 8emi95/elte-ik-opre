#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_SIZE 512

typedef struct Question 
{
    char question[MAX_SIZE];
    char answers[MAX_SIZE][MAX_SIZE];
    time_t date;
	int final;
   
} 
Question;

int numberOfQuestions = 0;
Question questions[MAX_SIZE];

int readFromStdInput(int min, int max) {
    char *buffer = malloc(MAX_SIZE);
    int inputNumber;
    int isGoodNumber;
	
	for(;;){
		isGoodNumber = 1;
        printf("kérem válasszon a felsorolt funkciók közül: ");
        fgets(buffer, MAX_SIZE, stdin);
        inputNumber = atoi(buffer);
        if ((strcmp(buffer,"0") == 0) 
		|| (inputNumber < min || inputNumber >= max)) {
            isGoodNumber = 0;
        }
		
		if(isGoodNumber==1){
			free(buffer);
			return inputNumber;
		}
	}
}

void getMenu() {
    printf("0 - adatokat tartalmazo file betoltese\n");
    printf("1 - mentes\n");
    printf("2 - uj kerdes hozzaadasa\n");
    printf("3 - kerdes torlese\n");
    printf("4 - kerdes modositasa\n");
    printf("5 - osszes kerdes listazasa\n");
    printf("6 - veglegesites\n");
    printf("7 - kilepes\n");
}

void deleteQuestion(int index) {
    for (int i = index + 1; i < numberOfQuestions; ++i) {
        questions[i - 1] = questions[i];
    }
}

void finalize() {
    for (int i = 0; i < numberOfQuestions; ++i) {
        printf("%s\nVeglegesiti ezt a kerdest?\n 0 - igen\n 1 - nem", questions[i].question);
        questions[i].final = readFromStdInput(0, 2);
    }
}

void listQuestions() {
    for (int i = 0; i < numberOfQuestions; ++i) {
        printf("%s\n1:%s\n2:%s\n3:%s\n4:%s\n",
               questions[i].question,
               questions[i].answers[0],
               questions[i].answers[1],
               questions[i].answers[2],
               questions[i].answers[3]
        );
    }
}

void writeIntoFile() {
    FILE *f = fopen("file.dat", "w");
    if (f == NULL) {
        printf("A file-t nem lehet megnyitni!\n");
        exit(1);
    }

    fprintf(f, "%d\n", numberOfQuestions);

    for (int i = 0; i < numberOfQuestions; ++i) {
        fprintf(f, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n",
                questions[i].question,
				questions[i].answers[0],
				questions[i].answers[1],
				questions[i].answers[2],
				questions[i].answers[3],
                questions[i].final,
                questions[i].date
        );
    }

    fclose(f);
}
void addQuestion(int index) {
    char *buffer = malloc(MAX_SIZE);

    Question question;
    printf("Kerdes : ");
    fgets(buffer, MAX_SIZE, stdin);
    strcpy(question.question, buffer);
    for (int i = 0; i < 4; ++i) {
        printf("%d. valasz: ", i + 1);
        fgets(buffer, MAX_SIZE, stdin);
        strcpy(question.answers[i], buffer);
    }
    question.final = 0;
    questions[index] = question;
    time(&questions[index].date);
	free(buffer);
}

void readFromFile() {
    FILE *f = fopen("file.dat", "r");
    if (f == NULL) {
        printf("A file-t nem lehet megnyitni!\n");
        exit(1);
    }
    fscanf(f, "%d", &numberOfQuestions);

    for (int i = 0; i < numberOfQuestions; ++i) {
        fscanf(f, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n",
                questions[i].question,
				questions[i].answers[0],
				questions[i].answers[1],
				questions[i].answers[2],
				questions[i].answers[3],
				&questions[i].final,
                &questions[i].date
        );

    }

    fclose(f);
}

int main() {
    for (;;){
        getMenu();
        int num = readFromStdInput(0, 8);
		if(num == 0){
			readFromFile();
		}
		else if(num == 1){
			writeIntoFile();
		}
		else if(num == 2){
			addQuestion(numberOfQuestions);
                numberOfQuestions++;
                //break;
		}
		else if(num == 3){
		if (numberOfQuestions == 0) {
                    printf("Nincs kerdes\n");
			}
			printf("Kérem adja meg a törlendő kérdés számát!\n");
                    int c = readFromStdInput(0, numberOfQuestions);
					deleteQuestion(c);
                    --numberOfQuestions;
		}
		else if(num == 4){
		if (numberOfQuestions == 0) {
                    printf("Nincs kerdes\n");
			}
			printf("Kérem adja meg a módosítandó kérdés számát!\n");
                    int c = readFromStdInput(0, numberOfQuestions);
					addQuestion(c);
		}
		else if(num == 5){
			listQuestions();
		}
		else if(num == 6){
                finalize();
		}
		else{
			break;
		}
    }
    return 0;
}