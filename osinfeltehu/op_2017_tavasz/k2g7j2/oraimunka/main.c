#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "survey.h"

#define STARTING_CAP (15)

void printMenu(void);

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

void finalize(Survey* s)
{
    s->final_b = 1;
    printf("Veglegesites sikeres! \n");
}

void changeString( char* str ) {
    printf("Az uj szoveg: ");
    char *tmp;
    size_t buff = BUFFER_SIZE;
    getchar();
    getline(&tmp, &buff, stdin);
    strcpy(str, tmp);
}

void changeQuestionMenu(Survey *s) {
    if (s->final_b==0)
    {
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
    }else
    {
        printf("A kerdoiv veglegesitett!\n");
    }
}


void deleteQuestionMenu(Survey *s) {
    if(s->final_b==0)
    {
    printf("Torlendo kerdes sorszama: ");
    int n;
    scanf("%d",&n);
    n-=1;
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
            }
            else if ( ch > 1 && ch < q->ansc+2 ){
                // Wants to delete an answer
                if ( q->ansc == 2 ) {
                    printf("Legalabb 2 valasznak lennie kell!\n");
                }
                else {

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
    else
    {
        printf("A kerdoiv veglegesitett!\n");
    }
}


int main(void) {
    Survey s;
    s.final_b = 0;
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
                finalize(&s);
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

