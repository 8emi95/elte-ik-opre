// Kadlecsik Csaba - XA1DSS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <signal.h>

#define SAFE_READ_INT_BUFFER 16
#define QUESTIONS_MAX 64
#define QUESTION_TEXT_SIZE 512
#define ANSWER_TEXT_SIZE 128
#define ANSWERS 4

typedef struct Kerdes {
    char kerdes[QUESTION_TEXT_SIZE];
    char valaszok[ANSWERS][ANSWER_TEXT_SIZE];
    int vegleges;
    time_t timestamp;
} Kerdes;

typedef struct Konzultacio {
    Kerdes kerdesek[3];
} Konzultacio;

typedef struct Valaszok {
    int valaszolokSzama;
    int valaszok[20][3];
} Valaszok;

int kerdesekSzama = 0;
Kerdes kerdesek[QUESTIONS_MAX];
char *kulcs;

int safeReadInt(int lowerBound, int upperBound) {
    char *buf;
    int integer;
    int ok;

    do {
        buf = malloc(SAFE_READ_INT_BUFFER);
        ok = 1;
        printf("> ");
        fgets(buf, SAFE_READ_INT_BUFFER, stdin);
        fflush(stdin);
        integer = atoi(buf);
        if ((integer == 0 && strcmp(buf, "0") == 0) || (integer < lowerBound || integer >= upperBound)) {
            ok = 0;
        }
        free(buf);
    } while (ok != 1);

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
    printf("7 - konzultacio inditasa\n");
    printf("8 - kilepes\n");
}

void addQuestion(int index) {
    char *buf;
    int i;

    Kerdes kerdes;
    printf("Kerdes szovege: ");
    buf = malloc(QUESTION_TEXT_SIZE);
    fgets(buf, QUESTION_TEXT_SIZE, stdin);
    strcpy(kerdes.kerdes, buf);
    fflush(stdin);
    free(buf);
    for (i = 0; i < 4; ++i) {
        buf = malloc(ANSWER_TEXT_SIZE);
        printf("%d. valasz szovege: ", i + 1);
        fgets(buf, ANSWER_TEXT_SIZE, stdin);
        strcpy(kerdes.valaszok[i], buf);
        fflush(stdin);
        free(buf);
    }
    kerdes.vegleges = 0;
    time(&kerdes.timestamp);
    kerdesek[index] = kerdes;

}

void deleteQuestion(int index) {
    int i;
    for (i = index + 1; i < kerdesekSzama; ++i) {
        kerdesek[i - 1] = kerdesek[i];
    }
}

void veglegesites() {
    int i;
    for (i = 0; i < kerdesekSzama; ++i) {
        printf("%s\nVeglegesites?\n 0 - igen\n 1 - nem", kerdesek[i].kerdes);
        kerdesek[i].vegleges = safeReadInt(0, 2);
    }
}

void listQuestions() {
    int i;



    for (i = 0; i < kerdesekSzama; ++i) {
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
    int i;
    if (f == NULL) {
        printf("Nem nyithato meg a file!\n");
        exit(1);
    }

    fprintf(f, "%d\n", kerdesekSzama);

    for (i = 0; i < kerdesekSzama; ++i) {
        fprintf(f, "%s%s%s%s%s%d\n%d\n",
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
    int i;
    char *buf;
    FILE *f = fopen("out.dat", "r");
    if (f == NULL) {
        printf("Nem nyithato meg a file!\nNincs adat betoltve\n");
    } else {
        fscanf(f, "%d\n", &kerdesekSzama);
        buf = malloc(QUESTION_TEXT_SIZE);

        for (i = 0; i < kerdesekSzama; ++i) {
            fgets(buf, QUESTION_TEXT_SIZE, f);
            strcpy(&kerdesek[i].kerdes,buf);
            fgets(buf, QUESTION_TEXT_SIZE, f);
            strcpy(&kerdesek[i].valaszok[0],buf);
            fgets(buf, QUESTION_TEXT_SIZE, f);
            strcpy(&kerdesek[i].valaszok[1],buf);
            fgets(buf, QUESTION_TEXT_SIZE, f);
            strcpy(&kerdesek[i].valaszok[2],buf);
            fgets(buf, QUESTION_TEXT_SIZE, f);
            strcpy(&kerdesek[i].valaszok[3],buf);

            fscanf(f, "%d\n%d\n",
                   &kerdesek[i].vegleges,
                   &kerdesek[i].timestamp
            );

        }
        fclose(f);
        free(buf);
    }


}

void startConsultation() {
    printf("Konzultacio inditasa...\n");
    int numberOfFinalQuestions = 0;
    int i, j;

    for (i = 0; i < kerdesekSzama; ++i) {
        if (kerdesek[i].vegleges != 1) {
            ++numberOfFinalQuestions;
        }
    }
    if (numberOfFinalQuestions < 3) {
        printf("Nincs 3 veglegesitett kerdes.\n");
        return;
    }


    Konzultacio konzultacio;
    Valaszok valaszok;

    //Csővezeték
    int pipe1[2];
    int pipe2[2];
    int mqid;
    key_t key = ftok(kulcs, 1);
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
        read(pipe1[0], &konzultacio, sizeof(konzultacio));
        close(pipe1[0]);
        printf("Fogadott kerdesek:\n%s\n%s\n%s\n",
               konzultacio.kerdesek[0].kerdes,
               konzultacio.kerdesek[1].kerdes,
               konzultacio.kerdesek[2].kerdes);

        valaszok.valaszolokSzama = 10 + rand() % 11;

        int valaszlehetosegek[3];

        for (i = 0; i < 3; ++i) {
            int valaszlehetoseg = 0;
            for (j = 0; j < 4; ++j) {
                if (strcmp("\n", konzultacio.kerdesek[i].valaszok[j]) != 0) {
                    ++valaszlehetoseg;
                }
            }
            valaszlehetosegek[i] = valaszlehetoseg;
        }

        for (i = 0; i < valaszok.valaszolokSzama; ++i) {
            for(j=0;j<3;++j) {
               valaszok.valaszok[i][j] = rand() % valaszlehetosegek[j];
            }
        }

        sleep(2);
        close(pipe2[0]);
        write(pipe2[1],&valaszok,sizeof(valaszok));
        close(pipe2[1]);
        fflush(NULL);
        pause();


    } else {
        int seed = time(NULL);
        srand(seed);
        int addedQuestions = 0;
        for(i=0;i<kerdesekSzama;++i) {
            if(kerdesek[i].vegleges != 1) {
                if(addedQuestions < 3) {
                    konzultacio.kerdesek[addedQuestions] = kerdesek[i];
                    ++addedQuestions;
                } else {
                    if(rand() % 100 < 75) {
                        konzultacio.kerdesek[rand() % 3] = kerdesek[i];
                    }
                }
            }
        }
        close(pipe1[0]);
        write(pipe1[1], &konzultacio, sizeof(konzultacio));
        close(pipe1[1]);
        fflush(NULL);
        sleep(4);
        close(pipe2[1]);
        read(pipe2[0],&valaszok,sizeof(valaszok));
        close(pipe2[0]);


        printf("Valaszok:\n");
        for(i = 0;i<3;++i) {
            printf("%s:\n",konzultacio.kerdesek[i].kerdes);
            for(j = 0;j<valaszok.valaszolokSzama;++j) {
                printf("%d ",valaszok.valaszok[j][i]);
            }
            printf("\n");
        }
        kill(pid,SIGKILL);
        exit(EXIT_SUCCESS);
    }

}

void start() {
    int i;
    while (1) {
        printMenu();
        int choice = safeReadInt(0, 9);
        switch (choice) {
            case 0:
                readData();
                break;
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
                    for (i = 0; i < kerdesekSzama; ++i) {
                        printf("%d: %s\n", i, kerdesek[i].kerdes);
                    }
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
            case 7:
                startConsultation();
                break;
            default:
                return;
        }
    }
}


int main(int argc, char *argv[]) {
    kulcs = argv[0];
    start();
    return 0;
}

