#include <stdio.h>
#include <time.h>
#include "string.h"
#include <string.h>
#include <stdlib.h>

int menu();
void addQuestion();
void listQuestions();
void changeQuestion();
void deleteQuestion();
void validate();

void writeToFile(char *question,
                 char *answer1,
                 char *answer2,
                 char *answer3,
                 char *answer4,
                 char *filename);
int countLines(char *filename);

int main()
{
    int choice;

    choice = menu();

    while (choice != 0)
    {
        switch (choice)
        {
        case 1:
            addQuestion();
            break;
        case 2:
            listQuestions();
            break;
        case 3:
            changeQuestion();
            break;
        case 4:
            deleteQuestion();
            break;
        case 5:
            validate();
            break;
        case 0:
            break;
        default:
            printf("Hibas input. ");
            printf("Valassz mast!\n");
        }
        choice = menu();
    }
    return 0;
}

int menu(void)
{
    int option;

    printf("\nMLSZ kerdoiv\n\n");
    printf("\t1. Kerdes hozzaadasa\n");
    printf("\t2. Kerdesek listazasa\n");
    printf("\t3. Kerdes modositasa\n");
    printf("\t4. Kerdes torlese\n");
    printf("\t5. Kerdessor veglegesitese\n");
    printf("\t0. Kilepes\n\n");
    printf("Valassz valamit! ");

    scanf("%i", &option);

    return option;
}

void addQuestion()
{
    char question[100];
    char answer1[30];
    char answer2[30];
    char answer3[30];
    char answer4[30];

    answer3[0] = 0;
    answer4[0] = 0;

    printf("Kerdes: ");
    fgets(question, 100, stdin);
    scanf("%[^\n]%*c", question);

    printf("1. valasz: ");
    scanf("%[^\n]%*c", answer1);

    printf("2. valasz: ");
    scanf("%[^\n]%*c", answer2);

    printf("3. valasz: ");
    scanf("%[^\n]%*c", answer3);
    if (answer3[0] == 0)
    {
        strcpy(answer3, "00");
    }

    printf("4. valasz: ");
    scanf("%[^\n]%*c", answer4);
    if (answer4[0] == 0)
    {
        strcpy(answer4, "00");
    }

    char *filename = "questions";
    FILE *fp = fopen(filename, "a");

    int questnumber = countLines(filename);
    writeToFile(question, answer1, answer2, answer3, answer4, filename);

    fclose(fp);
}

void listQuestions()
{
    char line[200];
    FILE *fp;

    if ((fp = fopen("questions", "r")) == NULL)
    {
        printf("\nHiba a fajl megnyitasakor\n");
        exit(1);
    };

    int i = 0;
    while (fgets(line, sizeof(line), fp))
    {
        ++i;
        if (i % 6 == 1)
        {
            int number = (i / 6) + 1;
            printf("%i. kerdes: ", number);
        }
        if (strcmp(line, "00\n") != 0)
        {
            printf("%s", line);
        }
    }

    fclose(fp);
}

void changeQuestion()
{
    listQuestions();

    char newQuestion[100];
    char newAnswer1[30];
    char newAnswer2[30];
    char newAnswer3[30];
    char newAnswer4[30];

    newAnswer3[0] = 0;
    newAnswer4[0] = 0;

    int n;

    printf("Add meg a kerdes sorszamat: ");
    scanf("%d", &n);

    int changeFrom = n + (n - 1) * 6 - (n - 1);

    printf("Uj kerdes: ");
    fgets(newQuestion, 100, stdin);
    scanf("%[^\n]%*c", newQuestion);

    printf("Uj 1. valasz: ");
    scanf("%[^\n]%*c", newAnswer1);

    printf("Uj 2. valasz: ");
    scanf("%[^\n]%*c", newAnswer2);

    printf("Uj 3. valasz: ");
    scanf("%[^\n]%*c", newAnswer3);
    if (newAnswer3[0] == 0)
    {
        strcpy(newAnswer3, "00");
    }

    printf("Uj 4. valasz: ");
    scanf("%[^\n]%*c", newAnswer4);
    if (newAnswer4[0] == 0)
    {
        strcpy(newAnswer4, "00");
    }

    FILE *fp1 = fopen("questions", "r");
    FILE *fp2 = fopen("changed", "w");

    if (fp1 == NULL)
    {
        printf("\nHiba a fajl megnyitasakor\n");
        exit(1);
    }

    char line[200];
    int i = 0;
    while (fgets(line, sizeof(line), fp1))
    {
        ++i;
        if (i == changeFrom)
        {
            fprintf(fp2, "%s\n", newQuestion);
            fprintf(fp2, "%s\n", newAnswer1);
            fprintf(fp2, "%s\n", newAnswer2);
            fprintf(fp2, "%s\n", newAnswer3);
            fprintf(fp2, "%s\n", newAnswer4);

            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fprintf(fp2, "%s", asctime(timeinfo));

            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
        }
        else
        {
            fprintf(fp2, "%s", line);
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove("questions");
    rename("changed", "questions");
}

void deleteQuestion()
{
    listQuestions();

    int n;

    printf("Add meg a kerdes sorszamat: ");
    scanf("%d", &n);

    int deleteFrom = n + (n - 1) * 6 - (n - 1);

    FILE *fp1 = fopen("questions", "r");
    FILE *fp2 = fopen("changed", "w");
    if (fp1 == NULL)
    {
        printf("\nHiba a fajl megnyitasakor\n");
        exit(1);
    }

    char line[200];
    int i = 0;
    while (fgets(line, sizeof(line), fp1))
    {
        ++i;
        if (i == deleteFrom)
        {
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
        }
        else
        {
            fprintf(fp2, "%s", line);
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove("questions");
    rename("changed", "questions");
    printf("\nA torles sikeresen megtortent\n");
}

void validate()
{
    listQuestions();

    FILE *fp2 = fopen("kerdoiv.txt", "w");

    int n;
    printf("Hany kerdest szeretnel hozzaadni? ");
    scanf("%d", &n);
    int questions[n];
    int i = 0;
    for (i; i < n; i++)
    {
        printf("%i. kerdes sorszama: ", (i + 1));
        scanf("%d", &questions[i]);
    }

    i = 0;
    for (i; i < n; i++)
    {
        FILE *fp1 = fopen("questions", "r");
        if (fp1 == NULL)
        {
            printf("\nHiba a fajl megnyitasakor\n");
            exit(1);
        }

        int copyFrom = questions[i] + (questions[i] - 1) * 6 - (questions[i] - 1);

        char line[200];
        int j = 0;
        while (fgets(line, sizeof(line), fp1) && j <= copyFrom)
        {
            ++j;
            if (j == copyFrom)
            {
                fprintf(fp2, "%s", line);
                fgets(line, sizeof(line), fp1);
                fprintf(fp2, "%s", line);
                fgets(line, sizeof(line), fp1);
                fprintf(fp2, "%s", line);
                fgets(line, sizeof(line), fp1);
                if (strcmp(line, "00\n") != 0)
                {
                    fprintf(fp2, "%s", line);
                }
                else
                {
                    fprintf(fp2, "%s", "");
                }
                fgets(line, sizeof(line), fp1);
                if (strcmp(line, "00\n") != 0)
                {
                    fprintf(fp2, "%s", line);
                }
                else
                {
                    fprintf(fp2, "%s", "");
                }
                fgets(line, sizeof(line), fp1);
                fprintf(fp2, "%s", "\n");
            }
        }
        fclose(fp1);
    }
    fclose(fp2);
    printf("A kerdoivet a 'kerdoiv.txt' fajlban talalod\n");
}

void writeToFile(char *question,
                 char *answer1,
                 char *answer2,
                 char *answer3,
                 char *answer4,
                 char *filename)
{
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        printf("\nHiba a fajl megnyitasakor\n");
        exit(1);
    }
    else
    {
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s", question, answer1, answer2, answer3, answer4, asctime(timeinfo));
    }

    fclose(fp);
}

int countLines(char *filename)
{
    FILE *fp = fopen(filename, "r");

    char c;
    int lines = 1;

    while ((c = fgetc(fp)))
    {
        if (c == EOF)
            break;
        if (c == '\n')
        {
            lines++;
        }
    }
    fclose(fp);

    return lines;
}