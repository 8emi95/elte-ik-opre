//Read and print out the file given in the parameter
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>

struct Question {
    char q[300];
    char a[300];
    char b[300];
    char c[300];
    char d[300];
    char date[18];
};

struct Question questions[100];
int q_num = 0;

void list_questions();
void read_questions();
void write_questions();
void add_question();
void modify_question();
void delete_question(int i);
void delete_question_ask();
void confirm_questions();

void menu();
char* get_console_line(void);
void ask_for_enter();


int main(int argc, char** argv) {
    FILE* ff = fopen("questions.txt", "ab+");
    fclose(ff);

    read_questions();

    menu();
    return 0;
}

void menu() {
    int again=1;
    do {
        /**/
        printf(
            "\n***\n1 - Kérdések listazasa\n2 - Kérdés hozzáadása\n3 - Kérdés módosítása\n4 - Kérdés törlése\n5 - Kérdőív véglegesítése\n0 kilepes\n***\n\nKivalasztott: "
        );
        char input = getchar();
        getchar(); // felsleges sorvégi entert eltuntetjuk
        /**/

        /** /
        char input='3';
        /**/
        switch(input) {
            case '1' : list_questions(); break;
            case '2' : add_question(); break;
            case '3' : modify_question(); break;
            case '4' : delete_question_ask(); break;
            case '5' : confirm_questions(); break;
            case '0' : again = 0; break;
            default  : puts("Nincs ilyen parancs!"); get_console_line(); break;
        }
    } while (1==again);
}



void list_question(int i) {
    printf("%d. - %s\n   a) %s\n   b) %s\n", i+1, questions[i].q, questions[i].a, questions[i].b);
    if(strlen(questions[i].c) > 0){
        printf("   c) %s\n", questions[i].c);
    }
    if(strlen(questions[i].d) > 0){
        printf("   d) %s\n", questions[i].d);
    }
    printf("felvétel időpontja: %s\n", questions[i].date);
    puts("");
}



void list_questions() {
    read_questions();

    printf("\njelenlegi kérdések:\n");

    int i;
    for(i=0; i<q_num; ++i) {
        list_question(i);
    }

    puts("");
    ask_for_enter();
}



void read_questions() {
    FILE * ff = fopen("questions.txt","rb");
    if (ff==NULL){ perror("File opening error\n"); exit(1); }

    fread(&q_num, sizeof(int), 1, ff);

    // handle each line of input
    int i;
    for(i=0; i<q_num; ++i) {
        fread(&questions[i], sizeof(struct Question), 1, ff);
    }

    fclose(ff);
}



void write_questions() {
    FILE *ff = fopen("questions.txt", "w");
    if (ff == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fwrite(&q_num, sizeof(int), 1, ff);

    int i;
    for(i=0; i<q_num; ++i) {
        fwrite(&questions[i], sizeof(struct Question), 1, ff);
    }

    fclose(ff);
}



void add_question() {
    char* line;

    do {
        printf("Kérdés: ");
        line=get_console_line();
        line[strlen(line) - 1] = 0;
        strcpy(questions[q_num].q, line);
    } while(strlen(questions[q_num].q)==0);

    do {
        printf("a): ");
        line=get_console_line();
        line[strlen(line) - 1] = 0;
        //strtok(line, "\n"); ez is a sorégi newlinet tünteti el, de csak akkor működik, ha nem csak egy entert üt.
        strcpy(questions[q_num].a, line);
    } while(strlen(questions[q_num].a)==0);
    do {
        printf("b): ");
        line=get_console_line();
        line[strlen(line) - 1] = 0;
        strcpy(questions[q_num].b, line);
    } while(strlen(questions[q_num].b)==0);

    printf("c): ");
    line=get_console_line();
    line[strlen(line) - 1] = 0;
    strcpy(questions[q_num].c, line);

    if(strlen(questions[q_num].c)!=0) {
        printf("d): ");
        line=get_console_line();
        line[strlen(line) - 1] = 0;
        strcpy(questions[q_num].d, line);
    } else {
        strcpy(questions[q_num].d, "");
    }

    time_t now = time(NULL);
    strftime(questions[q_num].date, sizeof(questions[q_num].date), "%Y.%m.%d %H:%M", gmtime(&now));

    ++q_num;
    printf("\nSikeres kérdésfelvétel.\nKérdés sorszáma: %d\n\n", q_num);

    write_questions();
    ask_for_enter();
}


/**/
void modify_question() {
    char* line;
    char input;


    printf("Adja meg a módositando kérdés sorszámát: ");
    line=get_console_line();
    strtok(line, "\n");
    int i = atoi(line) - 1;

    if(q_num<=i){
        puts("\nIlyen roszámú kérdés nincs.\n");
    } else {
        do {
            // kérdés kiírása
            puts("");
            printf("q) %s\n   a) %s\n   b) %s\n", questions[i].q, questions[i].a, questions[i].b);
            printf("   c) %s\n", questions[i].c);
            if(strlen(questions[i].c) > 0) {
                printf("   d) %s\n", questions[i].d);
            }
            puts("");

            // hiányzik, h mi legyen ha rossz betűjelet ad meg
            do {
                printf("Mit szeretne modositani? (betujel): ");
                input = getchar();
                getchar();
            } while(strlen(questions[i].c)==0 && 'd'==input);

            printf("Uj mondat: ");
            line=get_console_line();
            line[strlen(line) - 1] = 0;

            // hiányzik, h mi legyen ha rossz betujelet ad
            switch(input) {
                case 'q' : strcpy(questions[i].q, line); break;
                case 'a' : strcpy(questions[i].a, line); break;
                case 'b' : strcpy(questions[i].b, line); break;
                case 'c' : strcpy(questions[i].c, line); break;
                case 'd' : strcpy(questions[i].d, line); break;
            }
            // ha törlés történt, feljebb hozza a válaszokat
            if(strlen(questions[i].a)==0 && strlen(questions[i].b)>0) {
                strcpy(questions[i].a, questions[i].b);
                strcpy(questions[i].b, "");
            }
            if(strlen(questions[i].b)==0 && strlen(questions[i].c)>0) {
                strcpy(questions[i].b, questions[i].c);
                strcpy(questions[i].c, "");
            }
            if(strlen(questions[i].c)==0 && strlen(questions[i].d)>0) {
                strcpy(questions[i].c, questions[i].d);
                strcpy(questions[i].d, "");
            }
            // ha nincs meg a két lehetőség, akkor 
            if(strlen(questions[i].b)==0) {
                puts("Legalább két válaszlehetőség kell.");
                do {
                    printf("b): ");
                    line = get_console_line();
                    line[strlen(line) - 1] = 0;
                    strcpy(questions[i].b, line);
                } while(strlen(questions[i].b)==0);
            }

            printf("Befejezte ezen kérdés módosítását? (i/n) : ");
            input = getchar();
            getchar();
        } while('n'==input);

        write_questions();
        puts("\nModositas megtortent.\n");
    }
    ask_for_enter();
}
/**/


void delete_question(int i){
    while(i<q_num-1) {
        strcpy(questions[i].q, questions[i+1].q);
        strcpy(questions[i].a, questions[i+1].a);
        strcpy(questions[i].b, questions[i+1].b);
        strcpy(questions[i].c, questions[i+1].c);
        strcpy(questions[i].d, questions[i+1].d);
        strcpy(questions[i].date, questions[i+1].date);
        ++i;
    }
    --q_num;

    write_questions();
    puts("\nTorles megtortent.\n");
}



void delete_question_ask() {
    char* line;
    // int q_id = get_question_id();
    printf("Adja meg a torlesre szant kérdés sorszámát: ");
    line = get_console_line();
    strtok(line, "\n");
    int i = atoi(line) - 1;

    if(q_num <= i) {
        puts("\nNincs kerdes ezzel a sorszammal.\n");
    } else {
        delete_question(i);
    }
    ask_for_enter();
}



void confirm_questions() {
    int i;
    char input;

    for(i=0; i<q_num; ++i) {
        puts("");
        list_question(i);

        printf("Akarja, hogy ez a kérdés szerepeljen a végleges kérdőívben? (i/n) : ");
        input = getchar();
        getchar();

        switch(input) {
            case 'i' : break;
            case 'n' : delete_question(i); break;
            default  : --i;
        }
    }

    printf("\nVéglegesítés befejeződött.\n");
    ask_for_enter();
}






void ask_for_enter() {
    printf("Tovabblepes enterrel");
    get_console_line();
}

char* get_console_line(void) {
    char * line = malloc(300), * linep = line;
    size_t lenmax = 300, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF) { break; }

        if(0 == --len) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n'){ break; }
    }
    *line = '\0';
    return linep;
}






char** str_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}



