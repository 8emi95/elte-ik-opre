#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SAFE_READ_INT_BUFFER 16

typedef struct Question 
{
    char question[512];
    char aanswers[400][4];
    int definitive;
    time_t timestamp;
} Question;

int numOfQuestions = 0;
Question questions[50];
void start();
int safeReadInt(int lowerBound, int upperBound);
void printMenu();
void addQuestion(int index);
void deleteQuestion(int index);
void commit();
void listQuestions();
void writeData();
void readFile();



int main() {
    start();
    return 0;
}

void start()
 {
    while (1) 
    {
        printMenu();
        int choice = safeReadInt(0, 8);
        switch (choice) 
        {
            case 0:
                readFile();
            case 1:
                writeData();
                break;
            case 2:
                addQuestion(numOfQuestions);
                numOfQuestions++;
                break;
            case 3:
            case 4:
                if (numOfQuestions == 0) 
                {
                    printf("Nincs kérdés\n");
                } else 
                {
                    printf("Melyik kérdést??\n");
                    int c = safeReadInt(0, numOfQuestions);
                    if (choice == 3) 
                    {
                        deleteQuestion(c);
                        --numOfQuestions;
                    } else addQuestion(c);
                }
                break;
            case 5:
                listQuestions();
                break;
            case 6:
                commit();
                break;
            default:
                return;
        }
    }
}

int safeReadInt(int lowerBound, int upperBound) 
{
    char *buf = malloc(SAFE_READ_INT_BUFFER);
    int integer;
    int ok;

    do 
    {
        ok = 1;
        printf("> ");
        fgets(buf, SAFE_READ_INT_BUFFER, stdin);
        integer = atoi(buf);
        if ((integer == 0 && strcmp(buf,"0") == 0) || (integer < lowerBound || integer >= upperBound)) 
        {
            ok = 0;
        }

    } 
    while (ok != 1);
    free(buf);
    return integer;
}

void printMenu() 
{
    printf("( 0 ) Adatok betöltése\n");
    printf("( 1 ) Adatok mentése\n");
    printf("( 2 ) Kérdés felvétele\n");
    printf("( 3 ) Kérdés törlése\n");
    printf("( 4 ) Kérdés módosítása\n");
    printf("( 5 ) Kérdések listázása\n");
    printf("( 6 ) Végleges kérdések megadása\n");
    printf("( 7 ) Kilépés\n");
}

void addQuestion(int index) 
{
    char *buf = malloc(512);

    Question question;
    printf("Kérdés szövege: ");
    fgets(buf, 512, stdin);
    strcpy(question.question, buf);
    for (int i = 0; i < 4; ++i) {
        printf("%d. válasz szövege: ", i + 1);
        fgets(buf, 128, stdin);
        strcpy(question.aanswers[i], buf);
    }
    question.definitive = 0;
    questions[index] = question;
    time(&questions[index].timestamp);
}

void deleteQuestion(int index) 
{
    for (int i = index + 1; i < numOfQuestions; ++i) {
        questions[i - 1] = questions[i];
    }
}

void commit() 
{
    for (int i = 0; i < numOfQuestions; ++i) {
        printf("%s\nVéglegesites?\n 0 - igen\n 1 - nem", questions[i].question);
        questions[i].definitive = safeReadInt(0, 2);
    }
}

void listQuestions() 
{
    for (int i = 0; i < numOfQuestions; ++i) {
        printf("%s\n1:%s\n2:%s\n3:%s\n4:%s\n",
               questions[i].question,
               questions[i].aanswers[0],
               questions[i].aanswers[1],
               questions[i].aanswers[2],
               questions[i].aanswers[3]
        );
    }
}

void writeData() 
{
    FILE *f = fopen("kerdesek.txt", "w");
    if (f == NULL) 
    {
        printf("Nem nyitható meg a file!\n");
        exit(1);
    }

    fprintf(f, "%d\n", numOfQuestions);

    for (int i = 0; i < numOfQuestions; ++i) 
    {
        fprintf(f, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n",
                questions[i].question,
                questions[i].aanswers[0],
                questions[i].aanswers[1],
                questions[i].aanswers[2],
                questions[i].aanswers[3],
                questions[i].definitive,
                questions[i].timestamp
        );
    }

    fclose(f);
}

void readFile()
 {
    FILE *f = fopen("kerdesek.txt", "r");
    if (f == NULL) 
    {
        printf("Nem nyitható meg a file!\n");
        exit(1);
    }
    fscanf(f, "%d", &numOfQuestions);

    for (int i = 0; i < numOfQuestions; ++i) 
    {
        fscanf(f, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n",
                questions[i].question,
                questions[i].aanswers[0],
                questions[i].aanswers[1],
                questions[i].aanswers[2],
                questions[i].aanswers[3],
                &questions[i].definitive,
                &questions[i].timestamp
        );

    }

    fclose(f);


}


