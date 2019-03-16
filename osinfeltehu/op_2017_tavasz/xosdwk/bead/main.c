#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "survey.h"

#define STARTING_CAP (15)

void printMenu(void);
void printSubMenu(Question *q, char *text);

void changeString( char* str ) {
    printf("Az uj szoveg: ");
    char *tmp;
    size_t buff = BUFFER_SIZE;
    getchar();
    getline(&tmp, &buff, stdin);
    strcpy(str, tmp);
}

void changeQuestionMenu(Survey *s) {
    printf("Modositando kerdes sorszama: ");
    int n;
    scanf("%d",&n);
    n-=1;
    if ( n < s->qnum ){
        Question *q = &(s->q[n]);
        printSubMenu(q,"modositasa");
        int good = 0;
        while ( !good ){
            good = 1;
            printf("\n > ");
            int ch;
            scanf("%d",&ch);
            if ( ch == 1 ) {
                // Wants to change the question
                changeString(q->text);
            }
            else if ( ch > 1 && ch < q->ansc+2 ){
                // Wants to change an answer
                changeString(q->ans[ch-2]);
            }
            else if ( ch == q->ansc+2 ) {
                // Do nothing, wants to go back
            }
            else {
                good = 0;
            }
        }
    }
    else {
        printf("Nincs ilyen kerdes!\n");
    }
} 

void deleteQuestionMenu(Survey *s) {
    printf("Torlendo kerdes sorszama: ");
    int n;
    scanf("%d",&n);
    n-=1;
    while ( s->q[n].id < 0 && n < s->qnum) {
        ++n;
    }
    if ( n < s->qnum ){
        Question *q = &(s->q[n]);
        printSubMenu(q,"torlese");
        int good = 0;
        while ( !good ){
            good = 1;
            printf("\n > ");
            int ch;
            scanf("%d",&ch);
            if ( ch == 1 ) {
                // Wants to delete the question
                int cid = s->q[n].id;
                s->q[n].id = -1;
                int i;
                for (i = n+1 ; i < s->qnum ; ++i) {
                    if ( s->q[i].id != -1 ) {
                        s->q[i].id = cid;
                        ++cid;
                    }
                }

            }
            else if ( ch > 1 && ch < q->ansc+2 ){
                // Wants to delete an answer
                if ( q->ansc == 2 ) {
                    printf("Legalabb 2 valasznak lennie kell!\n");
                }
                else {
                    int ix;
                    for (ix = ch-2 ; ix < q->ansc-1 ; ++ix) {
                        strcpy(q->ans[ix],q->ans[ix+1]);
                    }
                    q->ansc--;
                }
            }
            else if ( ch == q->ansc+2 ) {
                // Do nothing, wants to go back
            }
            else {
                good = 0;
            }
        }
    }
    else {
        printf("Nincs ilyen kerdes!\n");
    }
}

int main(void) {
    Survey s;
    s.qnum = 0;
    s.max = STARTING_CAP;
    s.q = (Question*) malloc(sizeof(Question)*s.max);

    int choice = 0;
    while (choice != 9) {
        printMenu();
        printf(" > ");
        scanf("%d",&choice);
        switch (choice) {
            case 1:
                addQuestionMenu(&s);
                break;
            case 2:
                changeQuestionMenu(&s);
                break;
            case 3:
                deleteQuestionMenu(&s);
                break;
            case 4:
                printQuestions(&s);
                break;
            case 5:
                break;
        }
    }
    int i;
    for (i = 0 ; i< s.qnum ; ++i) {
        free (s.q[i].ans);
    }
    free(s.q);
    return 0;
}

void printMenu(void) {
    printf("\n");
    printf("1 - Kerdes felvetele\n");
    printf("2 - Kerdes modositasa\n");
    printf("3 - Kerdes torlese\n");
    printf("4 - Kerdesek listazasa \n");
    printf("5 - Kerdessor veglegesitese\n");
    printf("9 - Kilepes\n");
}

void printSubMenu(Question *q, char *text) {
    printf("1 - A kerdes ");
    printf(text);printf("\n");
    int i;
    for ( i = 0; i < q->ansc ; ++i) {
        printf("%d - A(z) %d. valasz ",(i+2), (i+1));
        printf(text);printf("\n");
    }
    printf("%d - Vissza",(i+2));
}
