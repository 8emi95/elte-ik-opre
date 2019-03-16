#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BUFF_SIZE 128

void menu();

void list();

void add();

void edit();

void delete();

void finalize();

int main(int argc, char **argv) {
    printf("Operacios Rendszerek | 1. beadando\r\n");
    printf("Randjelovic Igor     | WW9STB\r\n");
    printf("----------------------------------\r\n");

    menu();

    return 0;
}

void menu() {
    char sel[BUFF_SIZE];
    printf("Type command! (? for help)\r\n");

    while (1) {
        printf("> ");
        scanf("%s", sel);

        if (strcmp(sel, "help") == 0 || strcmp(sel, "h") == 0 || strcmp(sel, "?") == 0) {
            printf("Available Commands:\r\n");
            printf("  - list (ls, l) - List questions\r\n");
            printf("  - add (a) - Add question\r\n");
            printf("  - edit (e) - Edit question\r\n");
            printf("  - delete (del, d) - Delete question\r\n");
            printf("  - finalize (fin, f) - Finalize questions\r\n");
            printf("  - quit (exit, q) - Exit program");
        } else if (strcmp(sel, "list") == 0 || strcmp(sel, "ls") == 0 || strcmp(sel, "l") == 0) {
            list();
        } else if (strcmp(sel, "add") == 0 || strcmp(sel, "a") == 0) {
            add();
        } else if (strcmp(sel, "edit") == 0 || strcmp(sel, "e") == 0) {
            edit();
        } else if (strcmp(sel, "delete") == 0 || strcmp(sel, "del") == 0 || strcmp(sel, "d") == 0) {
            delete();
        } else if (strcmp(sel, "finalize") == 0 || strcmp(sel, "fin") == 0 || strcmp(sel, "f") == 0) {
            finalize();
        } else if (strcmp(sel, "quit") == 0 || strcmp(sel, "exit") == 0 || strcmp(sel, "q") == 0) {
            break;
        } else {
            printf("Unknown command.");
        }

        printf("\r\n");
    }
}

void list() {
    printf("List of questions\r\n");
    FILE *questions;
    questions = fopen("questions.txt", "r");

    if(questions == NULL) {
        printf("No questions yet.");
        return;
    }

    char sor[BUFF_SIZE];
    char final[BUFF_SIZE];
    char question[BUFF_SIZE];
    char date[BUFF_SIZE];
    char answers[BUFF_SIZE];
    while (fgets(sor, 100, questions) != NULL) {
        sscanf(sor, "%[^|]|%[^|]|%[^|]|%[^\n]", final, question, date, answers);

        printf("---------------\r\nFinal: %s\r\nDate created: %s\r\nQuestion: %s\r\nAnswers:\r\n", final, date,
               question);

        char *token;
        token = strtok(answers, ";\n");
        while (token != NULL) {
            printf("  - %s\r\n", token);
            token = strtok(NULL, ";\n");
        }

        printf("---------------");
    }
}

void add() {
    printf("Adding a new question...\r\n");
    FILE *questions;
    questions = fopen("questions.txt", "a+");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char question[BUFF_SIZE];
    char date[BUFF_SIZE];

    printf("Please type the question\r\n>> ");

    setbuf(stdin, NULL);
    fgets(question, sizeof question, stdin);
    if ((strlen(question) > 0) && (question[strlen(question) - 1] == '\n'))
        question[strlen(question) - 1] = '\0';

    printf("Please type the answers (type - to end answers)\r\n");

    char answers[4][BUFF_SIZE];
    int ansNo;
    int i;
    for (i = 0; i < 4; ++i) {
        do {
            printf("[%d]>> ", i + 1);
            setbuf(stdin, NULL);
            fgets(answers[i], sizeof answers[i], stdin);
            if ((strlen(answers[i]) > 0) && (answers[i][strlen(answers[i]) - 1] == '\n'))
                answers[i][strlen(answers[i]) - 1] = '\0';
            ansNo = i;

            if ((strlen(answers[i]) > 0) &&
                (
                        (strcmp(answers[i], "-") != 0)
                        ||
                        (ansNo >= 2 && strcmp(answers[i], "-") == 0 && ansNo--)
                )) {
                break;
            }

            printf("Err! Try again...\r\n");
        } while (1);

        if (i >= 2 && strcmp(answers[i], "-") == 0) {
            break;
        }
    }

    sprintf(date, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    char answersStr[1000];
    strcpy(answersStr, "");
    int a;
    for (a = 0; a <= ansNo; a++) {
        strcat(answersStr, answers[a]);
        strcat(answersStr, ";");
    }

    fprintf(questions, "false|%s|%s|%s\n", question, date, answersStr);
    printf("Added question!");

    fclose(questions);
}

void edit() {
    printf("Edit questions\r\n");
    FILE *questions;
    FILE *out;

    questions = fopen("questions.txt", "r");
    out = fopen("temp.txt", "w");

    char sor[BUFF_SIZE];
    char final[BUFF_SIZE];
    char question[BUFF_SIZE];
    char date[BUFF_SIZE];
    char answers[BUFF_SIZE];
    int q_id = 1;
    while (fgets(sor, 100, questions) != NULL) {
        sscanf(sor, "%*[^|]|%[^|]", question);
        printf("%d) %s\r\n", q_id++, question);
    }

    char sel[BUFF_SIZE];
    printf("\r\nEnter the number of the question you would like to edit");

    printf("\r\n> ");
    scanf("%s", sel);

    int s_q_id;
    s_q_id = atoi(sel);
    if (s_q_id < 0 || s_q_id >= q_id) {
        printf("Invalid question selected...");
        return;
    }
    printf("You selected: %d\r\n", s_q_id);

    q_id = 1;
    rewind(questions);
    while (fgets(sor, 100, questions) != NULL) {
        sscanf(sor, "%[^|]|%[^|]|%[^|]|%[^\n]", final, question, date, answers);
        if (q_id++ == s_q_id) {

            printf("---------------\r\nFinal: %s\r\nDate created: %s\r\nQuestion: %s\r\nAnswers:\r\n", final, date,
                   question);

            char p_answers[3][BUFF_SIZE];
            char *token;
            int ans_i = 0;
            token = strtok(answers, ";\n");
            while (token != NULL) {
                printf("  - %s\r\n", token);
                strcpy(p_answers[ans_i++], token);
                token = strtok(NULL, ";\n");
            }

            printf("---------------\r\n");

            printf("Please type the edited question\r\n>> ");

            setbuf(stdin, NULL);
            fgets(question, sizeof question, stdin);
            if ((strlen(question) > 0) && (question[strlen(question) - 1] == '\n'))
                question[strlen(question) - 1] = '\0';


            printf("Please type the answers (type - to end answers)\r\n");

            char new_answers[3][BUFF_SIZE];
            int ansNo;
            int i;
            for (i = 0; i < 4; ++i) {
                do {
                    printf("[%d] [%s]>> ", i + 1, (ans_i < i + 1 ? "-" : p_answers[i]));
                    setbuf(stdin, NULL);
                    fgets(new_answers[i], sizeof new_answers[i], stdin);
                    if ((strlen(new_answers[i]) > 0) && (new_answers[i][strlen(new_answers[i]) - 1] == '\n'))
                        new_answers[i][strlen(new_answers[i]) - 1] = '\0';
                    ansNo = i;

                    if ((strlen(new_answers[i]) > 0) &&
                        (
                                (strcmp(new_answers[i], "-") != 0)
                                ||
                                (ansNo >= 2 && strcmp(new_answers[i], "-") == 0 && ansNo--)
                        )) {
                        break;
                    }

                    printf("Err! Try again...\r\n");
                } while (1);

                if (i >= 2 && strcmp(new_answers[i], "-") == 0) {
                    break;
                }
            }

            char answersStr[1000];
            strcpy(answersStr, "");
            int a;
            for (a = 0; a <= ansNo; a++) {
                strcat(answersStr, new_answers[a]);
                strcat(answersStr, ";");
            }

            rewind(questions);
            int l_id = 1;
            while (fgets(sor, 100, questions) != NULL) {
                if (l_id == s_q_id) {
                    fprintf(out, "false|%s|%s|%s\n", question, date, answersStr);
                } else {
                    fprintf(out, "%s", sor);
                }
                l_id++;
            }

            printf("Edited question!");
            fclose(questions);
            fclose(out);

            remove("questions.txt");
            rename("temp.txt", "questions.txt");

            break;
        }
    }
}


void delete() {
    printf("Delete questions\r\n");
    FILE *questions;
    FILE *out;

    questions = fopen("questions.txt", "r");
    out = fopen("temp.txt", "w");

    char sor[BUFF_SIZE];
    char question[BUFF_SIZE];
    int q_id = 1;
    while (fgets(sor, 100, questions) != NULL) {
        sscanf(sor, "%*[^|]|%[^|]", question);
        printf("%d) %s\r\n", q_id++, question);
    }

    char sel[BUFF_SIZE];
    printf("\r\nEnter the number of the question you would like to edit");

    printf("\r\n> ");
    scanf("%s", sel);

    int s_q_id;
    s_q_id = atoi(sel);
    if (s_q_id < 0 || s_q_id >= q_id) {
        printf("Invalid question selected...");
        return;
    }
    printf("You selected: %d\r\n", s_q_id);

    rewind(questions);
    int l_id = 1;
    while (fgets(sor, 100, questions) != NULL) {
        if (l_id != s_q_id) {
            fprintf(out, "%s", sor);
        }
        l_id++;
    }

    printf("Deleted question!");
    fclose(questions);
    fclose(out);

    remove("questions.txt");
    rename("temp.txt", "questions.txt");
}

void finalize() {
    printf("Finalize question\r\n");
    FILE *questions;
    FILE *out;

    questions = fopen("questions.txt", "r");
    out = fopen("temp.txt", "w");

    char sor[BUFF_SIZE];
    char question[BUFF_SIZE];
    int q_id = 1;
    while (fgets(sor, 100, questions) != NULL) {
        sscanf(sor, "%*[^|]|%[^|]", question);
        printf("%d) %s\r\n", q_id++, question);
    }

    char sel[BUFF_SIZE];
    printf("\r\nEnter the number of the question you would like to finalize");

    printf("\r\n> ");
    scanf("%s", sel);

    int s_q_id;
    s_q_id = atoi(sel);
    if (s_q_id < 0 || s_q_id >= q_id) {
        printf("Invalid question selected...");
        return;
    }
    printf("You selected: %d\r\n", s_q_id);


    rewind(questions);
    int l_id = 1;
    while (fgets(sor, 100, questions) != NULL) {
        if (l_id == s_q_id) {
            sscanf(sor, "%*[^|]|%[^\n]", question);

            fprintf(out, "true|%s\n", question);
        } else {
            fprintf(out, "%s", sor);
        }
        l_id++;
    }

    printf("Finalized question!");
    fclose(questions);
    fclose(out);

    remove("questions.txt");
    rename("temp.txt", "questions.txt");
}