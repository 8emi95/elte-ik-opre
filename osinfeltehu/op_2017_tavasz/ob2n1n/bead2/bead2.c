#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define MAX_SIZE 512

char* ch_key;
typedef struct Question 
{
    char question[MAX_SIZE];
    char answers[MAX_SIZE][MAX_SIZE];
    time_t date;
	int final;
   
} 
Question;

typedef struct Consultation {
    Question questions[3];
} 
Consultation;

typedef struct Answers {
    int numberOfRespondent;
    int answers[20][3];
} 
Answers;

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
	printf("7 - konzultalas\n");
    printf("8 - kilepes\n");
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

void readData() {
    int i;
    char *buffer;
    FILE *f = fopen("out.dat", "r");
    if (f == NULL) {
        printf("Nem nyithato meg a file!\nNincs adat betoltve\n");
    } else {
        fscanf(f, "%d\n", &numberOfQuestions);
        buffer = malloc(MAX_SIZE);

        for (i = 0; i < numberOfQuestions; ++i) {
            fgets(buffer, MAX_SIZE, f);
            strcpy(&questions[i].question,buffer);
            fgets(buffer, MAX_SIZE, f);
            strcpy(&questions[i].answers[0],buffer);
            fgets(buffer, MAX_SIZE, f);
            strcpy(&questions[i].answers[1],buffer);
            fgets(buffer, MAX_SIZE, f);
            strcpy(&questions[i].answers[2],buffer);
            fgets(buffer, MAX_SIZE, f);
            strcpy(&questions[i].answers[3],buffer);

            fscanf(f, "%d\n%d\n",
                   &questions[i].final,
                   &questions[i].date
            );

        }
        fclose(f);
        free(buffer);
    }


}

void goConsultation() {
    printf("Konzultacio inditasa...\n");
    int numberOfFinalQuestions = 0;
    int i, j;

    for (i = 0; i < numberOfQuestions; ++i) {
        if (questions[i].final != 1) {
            ++numberOfFinalQuestions;
        }
    }
    if (numberOfFinalQuestions < 3) {
        printf("Nincs 3 veglegesitett kerdes.\n");
        return;
    }


    Consultation consultation;
    Answers answers;

    //Csõvezeték
    int pipe1[2];
    int pipe2[2];
    int mqid;
    key_t key = ftok(ch_key, 1);
    mqid = msgget(key, 0644 | IPC_CREAT);

    if (mqid < 0) {
        perror("msgget");
        return;
    }

    pid_t pid;
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {

        //Child
        sleep(1);
        int seed = time(NULL);
        srand(seed);
        printf("Konzultacios kerdesek fogadasa!\n");
        close(pipe1[1]);
        read(pipe1[0], &consultation, sizeof(consultation));
        close(pipe1[0]);
        printf("Fogadott questions:\n%s\n%s\n%s\n",
               consultation.questions[0].question,
               consultation.questions[1].question,
               consultation.questions[2].question);

        answers.numberOfRespondent = 10 + rand() % 11;

        int answersArray[3];

        for (i = 0; i < 3; ++i) {
            int potentialAnswer = 0;
            for (j = 0; j < 4; ++j) {
                if (strcmp("0", consultation.questions[i].answers[j]) != 0) {
                    ++potentialAnswer;
                }
            }
            answersArray[i] = potentialAnswer;
        }

        for (i = 0; i < answers.numberOfRespondent; ++i) {
            for(j=0;j<3;++j) {
               answers.answers[i][j] = rand() % answersArray[j];
            }
        }

        sleep(2);
        close(pipe2[0]);
        write(pipe2[1],&answers,sizeof(answers));
        close(pipe2[1]);
        fflush(NULL);
        pause();


    } else {
        int seed = time(NULL);
        srand(seed);
        int addedQuestions = 0;
        for(i=0;i<numberOfQuestions;++i) {
            if(questions[i].final != 1) {
                if(addedQuestions < 3) {
                    consultation.questions[addedQuestions] = questions[i];
                    ++addedQuestions;
                } else {
                    if(rand() % 100 < 75) {
                        consultation.questions[rand() % 3] = questions[i];
                    }
                }
            }
        }
        close(pipe1[0]);
        write(pipe1[1], &consultation, sizeof(consultation));
        close(pipe1[1]);
        fflush(NULL);
        sleep(4);
        close(pipe2[1]);
        read(pipe2[0],&answers,sizeof(answers));
        close(pipe2[0]);


        printf("Answers:\n");
        for(i = 0;i<3;++i) {
            printf("%s:\n",consultation.questions[i].question);
            for(j = 0;j<answers.numberOfRespondent;++j) {
                printf("%d ",answers.answers[j][i]);
            }
            printf("\n");
        }
        kill(pid,SIGKILL);
        exit(EXIT_SUCCESS);
    }

}

int main(int argc, char *argv[]) {
	ch_key = argv[0];
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
			printf("Kérem adja meg a törlendõ kérdés számát!\n");
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
		}else if(num==7){
			goConsultation();
		}
		else{
			break;
		}
    }
    return 0;
}