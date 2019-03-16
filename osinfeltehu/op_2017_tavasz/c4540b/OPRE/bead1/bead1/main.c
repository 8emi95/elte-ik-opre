#include <stdio.h>
#include <time.h>
#include "string.h"
#include <string.h>
#include <stdlib.h>


void create(char name[100]);
void listaz(char name[100], char azon[100]);
void listDatas(char name[100]);
int get_line(char *buffer, size_t max);
void add(char name[100]);
void change(char name[100]);
void del(char name[100]);
void writeToFile(char* name, char* question);
void writeToFile2(char* name, char* question);
void writeTheAzon(char* name, int azon);
void writeTheDate(char* name);
void validate(char * name);


int main(){
    printf("Do you want to create new file?\n");
    char ans;
    scanf("%c", &ans);
    printf("Give the name of your file!\n");
    char name[100];
    scanf("%s", &name);
    if (ans=='y')
    {
        create(name);
    }
    int choice = menu();
    while(choice != 6)
    {
        switch(choice)
        {
            case 1:
                add(name);
                break;
            case 2:
                change(name);
                break;
            case 3:
                del(name);
                break;
            case 4:
                listDatas(name);
                break;
            case 5:
                validate(name);
            case 6:
                exit(1);
        }
        choice = menu();
    }
    return 0;
}

int menu(){
    int option;

    printf("\nMenu\n");

    printf("1. Add question\n");
    printf("2. Change question\n");
    printf("3. Delete question\n");
    printf("4. List question\n");
    printf("5. Validate question\n");
    printf("6. Quit\n");

    scanf("%i", &option);

    return option;
}

void create(char name[100]){
    FILE* fp = fopen(name, "a+");
    fclose(fp);
}

void listaz(char name[100], char azon[100]){
    char place[200];
    FILE *fp1;
    if ((fp1 = fopen(name, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    int igen=0;
    while(fgets(place, sizeof(place), fp1)){
        igen=0;
        if(atoi(azon)==atoi(place))
        {
            fgets(place, sizeof(place), fp1);
            int x = atoi(place)+1;
            if(x>0)
            {
                while(x>0 && fgets(place, sizeof(place), fp1))
                {
                    printf("%s", place);
                    memset(place, 0, sizeof(place));
                    --x;
                }
            }
            fgets(place, sizeof(place), fp1);
            igen=1;
        }
        if(igen==0)
        {
            fgets(place, sizeof(place), fp1);
            int x = atoi(place)+2;
            while(x>0 && fgets(place, sizeof(place), fp1))
            {
                --x;
            }
        }
    }

    fclose(fp1);
}

void listDatas(char name[100]){
    char line[200];
    FILE *fp;

    if ((fp = fopen(name, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    int i=0;
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
}

int get_line(char *buffer, size_t max){
  if(fgets(buffer, max, stdin) == buffer)
  {
    size_t len = strlen(buffer);
    if(len > 0 && buffer[len - 1] == '\0')
      buffer[--len] = '\0';
    return len;
  }
  return 0;
}

void add(char name[100]){
    FILE* fp = fopen(name, "a+");

    char azon[100];
    printf("Get the identifier of your question!\n");
    fgets (azon, 100, stdin);
    scanf("%[^\n]%*c", azon);
    //writeToFile(name, azon);

    int num;
    printf("How many answers do you need?\n");
    scanf("%i", &num);
    //writeTheAzon(name, num);

    char question[100];
    printf("Type the question!\n");
    fgets (question, 100, stdin);
    scanf ("%[^\n]%*c", question);
    //writeToFile(name, question);

    if(num<2 || num>4)
    {
        printf("Error! You need to choice between 2 and 4.");
    }else
    {
        writeToFile(name, azon);
        writeTheAzon(name, num);
        writeToFile(name, question);
        char answers[100];
        int i=0;
        for (i=0; i<num; ++i)
        {
            printf("Add the %i", i+1);
            printf(". answer: ");
            get_line(answers, 100);
            writeToFile2(name, answers);
        }
    }
    writeTheDate(name);
}

void change(char name[100]){
    char azon[100];
    printf("Get the identifier of your question!\n");
    fgets (azon, 100, stdin);
    scanf("%s", azon);

    listaz(name,azon);

    FILE *fp1 = fopen(name, "r");
    FILE *fp2 = fopen("changed", "w");
    if(fp1 == NULL)
    {
        printf("Error!");
        exit(1);
    }

    char place[100];
    int igen = 0;
    while(fgets(place, sizeof(place), fp1)){
        igen=0;
        if(atoi(azon)==atoi(place))
        {
            fprintf(fp2, "%s", place);
            fgets(place, sizeof(place), fp1);
            fprintf(fp2, "%s", place);
            int x = atoi(place);

            fgets(place, sizeof(place), fp1);
            char xy[100];
            printf("Type the new question!\n");
            fgets (xy, 100, stdin);
            scanf ("%[^\n]%*c", xy);
            fprintf(fp2, "%s\n", xy);
            memset(place, 0, sizeof(place));
            if(x>0)
            {
                while(x>0 && fgets(place, sizeof(place), fp1))
                {
                    char xz[100];
                    printf("Add the %i", x+1, ". answer!\n");
                    get_line(xz, 100);
                    fprintf(fp2, "%s", xz);
                    memset(place, 0, sizeof(place));
                    --x;
                }
            }
            fgets(place, sizeof(place), fp1);
            time_t rawtime;
            struct tm * timeinfo;
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            fprintf(fp2, "%s" ,asctime(timeinfo));
            igen=1;
        }
        if(igen==0)
        {
            fprintf(fp2, "%s", place);
            fgets(place, sizeof(place), fp1);
            fprintf(fp2, "%s", place);
            int x = atoi(place)+2;
            while(x>0 && fgets(place, sizeof(place), fp1))
            {
                fprintf(fp2, "%s", place);
                --x;
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove(name);
    rename("changed", name);
}

void del(char name[100]){
    char azon[100];
    printf("Get the identifier of your question!\n");
    fgets (azon, 100, stdin);
    scanf("%s", azon);

    listaz(name,azon);

    FILE *fp1 = fopen(name, "r");
    FILE *fp2 = fopen("changed", "w");
    if(fp1 == NULL)
    {
        printf("Error!");
        exit(1);
    }

    char place[100];
    int igen = 0;
    while(fgets(place, sizeof(place), fp1)){
        igen=0;
        if(atoi(azon)==atoi(place))
        {
            fgets(place, sizeof(place), fp1);
            int x = atoi(place)+1;
            fgets(place, sizeof(place), fp1);
            if(x>0)
            {
                while(x>0 && fgets(place, sizeof(place), fp1))
                {
                    memset(place, 0, sizeof(place));
                    --x;
                }
            }
            igen=1;
        }
        if(igen==0)
        {
            fprintf(fp2, "%s", place);
            fgets(place, sizeof(place), fp1);
            fprintf(fp2, "%s", place);
            int x = atoi(place)+2;
            while(x>0 && fgets(place, sizeof(place), fp1))
            {
                fprintf(fp2, "%s", place);
                --x;
            }
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove(name);
    rename("changed", name);
}

void writeToFile(char* name, char* question){
    FILE *fp = fopen(name, "a");
    if(fp == NULL)
    {
        printf("Error!");
        exit(1);
    }else{
        fprintf(fp, "%s\n", question);
    }

    fclose(fp);
}

void writeToFile2(char* name, char* question){
    FILE *fp = fopen(name, "a");
    if(fp == NULL)
    {
        printf("Error!");
        exit(1);
    }else{
        fprintf(fp, "%s", question);
    }

    fclose(fp);
}

void writeTheAzon(char* name, int azon){
    FILE *fp = fopen(name, "a");
    if(fp == NULL)
    {
        printf("Error!");
        exit(1);
    }else{
        fprintf(fp, "%i\n", azon);
    }

    fclose(fp);
}

void writeTheDate(char* name) {
    FILE *fp = fopen(name, "a");
    if(fp == NULL)
    {
        printf("Error!");
        exit(1);
    }else{
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        fprintf(fp, "%s" ,asctime(timeinfo));
    }

    fclose(fp);
}

void validate(char * name){
    char azon[100];
    printf("Get the identifier of your question!\n");
    fgets (azon, 100, stdin);
    scanf("%s", azon);

    FILE *fp1 = fopen(name, "r");
    FILE *fp2 = fopen("validated.txt", "a+");
    if(fp1 == NULL)
    {
        printf("Error!");
        exit(1);
    }

    char place[100];
    int igen = 0;
    while(fgets(place, sizeof(place), fp1)){
        igen=0;
        if(atoi(azon)==atoi(place))
        {
            fgets(place, sizeof(place), fp1);
            int x = atoi(place);

            fgets(place, sizeof(place), fp1);
            fprintf(fp2, "%s", place);
            if(x>0)
            {
                while(x>0 && fgets(place, sizeof(place), fp1))
                {
                    fprintf(fp2, "%s", place);
                    memset(place, 0, sizeof(place));
                    --x;
                }
            }
            fgets(place, sizeof(place), fp1);
            igen=1;
        }
        if(igen==0)
        {
            fgets(place, sizeof(place), fp1);
            int x = atoi(place)+2;
            while(x>0 && fgets(place, sizeof(place), fp1))
            {
                --x;
            }
        }
    }

    fclose(fp1);
    fclose(fp2);
}
