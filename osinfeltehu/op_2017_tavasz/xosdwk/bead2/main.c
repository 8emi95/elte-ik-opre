#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "survey.h"
#include "consultation.h"

int main(int argc, char ** argv) {
    Survey s;
    s.head = (Question*) malloc(sizeof(Question));
    s.head->next = s.head;
    s.head->prev = s.head;
    s.qnum = 0;

    Survey fin;
    fin.head = (Question*) malloc(sizeof(Question));
    fin.head->next = fin.head;
    fin.head->prev = fin.head;
    fin.qnum = 0;

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
            case 5: {
                if (argc > 1) {
                    finalizeQuestions(&s, argv[1], &fin);
                }
                else {
                    finalizeQuestions(&s, "survey.txt", &fin);
                }
                if ( argc > 2 ) {
                    beginConsultation(&fin, argv[2]);
                }
                else {
                    printf( "Nincs megadva varos parameternek!\n" );
                    printf( "Adjon meg egy varost: " );
                    getchar();
                    char * city;
                    size_t buff = BUFFER_SIZE;
                    getline(&city, &buff, stdin);
                    beginConsultation(&fin, city);
                }

                break;
            }
        }
    }

    Question * r = s.head->next;
    while ( r != s.head ) {
        Question * p = r;
        r = r->next;
        p->prev->next = p->next;
        p->next->prev = p->prev;
        p->next = NULL;
        p->prev = NULL;
        int i;
        for (i = 0; i < p->ansc; ++i) {
            free(p->ans[i]);
        }
        free(p->text);
        free(p);
    }
    free(s.head);
    return 0;
}

