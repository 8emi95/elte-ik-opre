
#ifndef OSHW2_SURVEY_SYSTEM_H
#define OSHW2_SURVEY_SYSTEM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#endif //OSHW2_SURVEY_SYSTEM_H

#define strsize 256

typedef struct DS {
    //id
    char id[20];
    //date
    char date[strsize];
    //question
    char question[strsize];
    //answers
    char answer1[strsize];
    char answer2[strsize];
    char answer3[strsize];
    char answer4[strsize];
} DS;

void reorder_id() {
    FILE *file1 = fopen("questionbank.txt", "r");
    FILE *file2 = fopen("qbkup.txt", "w");

    char tmp[20];

    int n = 1;

    DS *ds = malloc(sizeof(DS));

    while (fread(ds, sizeof(DS), 1, file1)) {
        sprintf(tmp, "%d\n", n++);
        strcpy(ds->id, tmp);
        fwrite(ds, sizeof(DS), 1, file2);
    }

    remove("questionbank.txt");
    rename("qbkup.txt", "questionbank.txt");

    free(ds);

    fclose(file1);
    fclose(file2);

}

void add_ds_to_file(DS *ds) {
    FILE *file = fopen("questionbank.txt", "a");

    if (file != NULL) {
        fwrite(ds, sizeof(DS), 1, file);
    }
    fclose(file);

}

void delete_from_qb(char n[20]) {
    FILE *file1 = fopen("questionbank.txt", "r");
    FILE *file2 = fopen("qbkup.txt", "w");

    DS *ds = malloc(sizeof(DS));

    if (file1 != NULL) {
        while (fread(ds, sizeof(DS), 1, file1)) {
            if (atoi(n) != atoi(ds->id)) {
                fwrite(ds, sizeof(DS), 1, file2);
            }
        }
    }

    remove("questionbank.txt");
    rename("qbkup.txt", "questionbank.txt");

    free(ds);

    fclose(file1);
    fclose(file2);

    reorder_id();

}

void modify_qb(DS *para, char n[20]) {
    FILE *file1 = fopen("questionbank.txt", "r");
    FILE *file2 = fopen("qbkup.txt", "w");

    DS *ds = malloc(sizeof(DS));


    if (file1 != NULL) {
        while (fread(ds, sizeof(DS), 1, file1)) {
            if (atoi(n) == atoi(ds->id)) {
                strcpy(para->id, ds->id);
                fwrite(para, sizeof(DS), 1, file2);
            } else {
                fwrite(ds, sizeof(DS), 1, file2);
            }
        }
    }
    remove("questionbank.txt");
    rename("qbkup.txt", "questionbank.txt");

    free(ds);

    fclose(file1);
    fclose(file2);

}

DS *ds_creation() {
    char id_string[20] = "0\n";
    DS *ds = malloc(sizeof(DS));
    int n;
    char c[5];

    time_t current_time;
    char *s_time;
    current_time = time(NULL);

    s_time = ctime(&current_time);
    strcpy(ds->date, s_time);

    printf("Please enter question title:\n");
    fgets(ds->question, strsize, stdin);

    printf("Please enter how many answers you would like to have:\n");
    fgets(c, 5, stdin);
    n = atoi(c);

    strcpy(ds->id, id_string);

    switch (n) {
        case 1:
            fgets(ds->answer1, strsize, stdin);
            strcpy(ds->answer2, "\n");
            strcpy(ds->answer3, "\n");
            strcpy(ds->answer4, "\n");
            break;
        case 2:
            fgets(ds->answer1, strsize, stdin);
            fgets(ds->answer2, strsize, stdin);
            strcpy(ds->answer3, "\n");
            strcpy(ds->answer4, "\n");
            break;
        case 3:
            fgets(ds->answer1, strsize, stdin);
            fgets(ds->answer2, strsize, stdin);
            fgets(ds->answer3, strsize, stdin);
            strcpy(ds->answer4, "\n");
            break;
        case 4:
            fgets(ds->answer1, strsize, stdin);
            fgets(ds->answer2, strsize, stdin);
            fgets(ds->answer3, strsize, stdin);
            fgets(ds->answer4, strsize, stdin);
            break;
        default:
            printf("invalid input\n");
    }

    return ds;
}

void create_survey(char *filename) {
    char buffer[strsize];

    FILE *file2 = fopen(filename, "a");

    DS *ds = malloc(sizeof(DS));

    char exit[] = "exit\n";

    while (fgets(buffer, strsize, stdin)) {


        if (strcmp(buffer, exit) == 0) {
            break;
        }
        FILE *file1 = fopen("questionbank.txt", "r");
        if (file1 != NULL) {
            while (fread(ds, sizeof(DS), 1, file1)) {
                if (atoi(buffer) == atoi(ds->id)) {
                    fwrite(ds, sizeof(DS), 1, file2);
                }
            }
        }
        printf("Added ID: %s to the survey!\n", buffer);
        fclose(file1);
    }

    free(ds);

    fclose(file2);
}

int total_q() {
    DS *ds = malloc(sizeof(DS));
    FILE *file1 = fopen("questionbank.txt", "r");

    int cnt = 0;

    if (file1 != NULL) {
        while (fread(ds, sizeof(DS), 1, file1)) {
            cnt++;
        }
    }
    free(ds);
    fclose(file1);
    return cnt;
}

int count_alt(DS ds) {
    int cnt = 4;
    if(strcmp(ds.answer1,"\n") == 0) {
        cnt--;
    }
    if(strcmp(ds.answer2,"\n") == 0) {
        cnt--;
    }
    if(strcmp(ds.answer3,"\n") == 0) {
        cnt--;
    }
    if(strcmp(ds.answer4,"\n") == 0) {
        cnt--;
    }
    return cnt;
}

DS pick_question(int index) {
    DS to_return;
    DS *ds = malloc(sizeof(DS));
    FILE *file1 = fopen("questionbank.txt", "r");
    if (file1 != NULL) {
        while (fread(ds, sizeof(DS), 1, file1)) {
            if ((index) == atoi(ds->id)) {
                break;
            }
        }
    }
    to_return = *ds;
    free(ds);
    fclose(file1);
    return to_return;
}


void list_questions(int mode) {
    FILE *file1 = fopen("questionbank.txt", "r");

    DS *ds = malloc(sizeof(DS));


    if (file1 != NULL && mode == 0) {
        printf("---------------\n");
        while (fread(ds, sizeof(DS), 1, file1)) {
            printf("Title: %sID: %sDate: %sA1: %sA2: %sA3: %sA4: %s", ds->question, ds->id, ds->date, ds->answer1,
                   ds->answer2, ds->answer3, ds->answer4);
            printf("---------------\n");
        }

    }

    if (file1 != NULL && mode == 1) {
        printf("---------------\n");
        while (fread(ds, sizeof(DS), 1, file1)) {
            printf("Title: %sID: %sDate: %s", ds->question, ds->id, ds->date);
            printf("---------------\n");
        }
    }

    free(ds);

    fclose(file1);
}


