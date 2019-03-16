#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SAFE_READ_INT_BUFFER 16
#define QUESTIONS_MAX 64
#define QUESTION_TEXT_SIZE 512
#define ANSWER_TEXT_SIZE 128
#define ANSWERS 4

typedef struct Kerdes {
    char kerdes[QUESTION_TEXT_SIZE];
    char valaszok[ANSWER_TEXT_SIZE][ANSWERS];
    int vegleges;
    time_t timestamp;
} Kerdes;

int kerdesekSzama = 0;
Kerdes kerdesek[QUESTIONS_MAX];

int safeReadInt(int lowerBound, int upperBound) {
    char *buf = malloc(SAFE_READ_INT_BUFFER);
    int integer;
    int ok;

    do {
        ok = 1;
        printf("> ");
        fgets(buf, SAFE_READ_INT_BUFFER, stdin);
        integer = atoi(buf);
        if ((integer == 0 && strcmp(buf,"0") == 0) || (integer < lowerBound || integer >= upperBound)) {
            ok = 0;
        }

    } while (ok != 1);
    free(buf);
    return integer;
}

void printMenu() {
    printf("0 - adatok betoltese\n");
    printf("1 - adatok mentese\n");
    printf("2 - kerdes felvetele\n");
    printf("3 - kerdes torlese\n");
    printf("4 - kerdes modositasa\n");
    printf("5 - kerdesek listazasa\n");
    printf("6 - vegleges kerdesek megadasa\n");
    printf("7 - kilepes\n");
}

void addQuestion(int index) {
    char *buf = malloc(QUESTION_TEXT_SIZE);

    Kerdes kerdes;
    printf("Kerdes szovege: ");
    fgets(buf, QUESTION_TEXT_SIZE, stdin);
    strcpy(kerdes.kerdes, buf);
    for (int i = 0; i < 4; ++i) {
        printf("%d. valasz szovege: ", i + 1);
        fgets(buf, ANSWER_TEXT_SIZE, stdin);
        strcpy(kerdes.valaszok[i], buf);
    }
    kerdes.vegleges = 0;
    kerdesek[index] = kerdes;
    time(&kerdesek[index].timestamp);
}

void deleteQuestion(int index) {
    for (int i = index + 1; i < kerdesekSzama; ++i) {
        kerdesek[i - 1] = kerdesek[i];
    }
}

void veglegesites() {
    for (int i = 0; i < kerdesekSzama; ++i) {
        printf("%s\nVeglegesites?\n 0 - igen\n 1 - nem", kerdesek[i].kerdes);
        kerdesek[i].vegleges = safeReadInt(0, 2);
    }
}

void listQuestions() {
    for (int i = 0; i < kerdesekSzama; ++i) {
        printf("%s\n1:%s\n2:%s\n3:%s\n4:%s\n",
               kerdesek[i].kerdes,
               kerdesek[i].valaszok[0],
               kerdesek[i].valaszok[1],
               kerdesek[i].valaszok[2],
               kerdesek[i].valaszok[3]
        );
    }
}

void writeData() {
    FILE *f = fopen("out.dat", "w");
    if (f == NULL) {
        printf("Nem nyithato meg a file!\n");
        exit(1);
    }

    fprintf(f, "%d\n", kerdesekSzama);

    for (int i = 0; i < kerdesekSzama; ++i) {
        fprintf(f, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n",
                kerdesek[i].kerdes,
                kerdesek[i].valaszok[0],
                kerdesek[i].valaszok[1],
                kerdesek[i].valaszok[2],
                kerdesek[i].valaszok[3],
                kerdesek[i].vegleges,
                kerdesek[i].timestamp
        );
    }

    fclose(f);
}

void readData() {
    FILE *f = fopen("out.dat", "r");
    if (f == NULL) {
        printf("Nem nyithato meg a file!\n");
        exit(1);
    }
    fscanf(f, "%d", &kerdesekSzama);

    for (int i = 0; i < kerdesekSzama; ++i) {
        fscanf(f, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n",
                kerdesek[i].kerdes,
                kerdesek[i].valaszok[0],
                kerdesek[i].valaszok[1],
                kerdesek[i].valaszok[2],
                kerdesek[i].valaszok[3],
&kerdesek[i].vegleges,
                &kerdesek[i].timestamp
        );

    }

    fclose(f);


}

void start() {
    while (1) {
        printMenu();
        int choice = safeReadInt(0, 8);
        switch (choice) {
            case 0:
                readData();
            case 1:
                writeData();
                break;
            case 2:
                addQuestion(kerdesekSzama);
                kerdesekSzama++;
                break;
            case 3:
            case 4:
                if (kerdesekSzama == 0) {
                    printf("Nincs kerdes\n");
                } else {
                    printf("Melyik kerdest?\n");
                    int c = safeReadInt(0, kerdesekSzama);
                    if (choice == 3) {
                        deleteQuestion(c);
                        --kerdesekSzama;
                    } else addQuestion(c);
                }
                break;
            case 5:
                listQuestions();
                break;
            case 6:
                veglegesites();
                break;
            default:
                return;
        }
    }
}

int main() {
    start();
    return 0;
}