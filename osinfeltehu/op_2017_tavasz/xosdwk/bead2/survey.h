#ifndef _SURVEY_H_
#define _SURVEY_H_

#include <time.h>

#define BUFFER_SIZE (80)

struct Question {
    int id;
    char* text;
    int ansc;
    char** ans;
    struct tm cDate;
    //int date[6];
    struct Question * next;
    struct Question * prev;
};
typedef struct Question Question;

struct Survey {
    int qnum;
    Question * head;
};
typedef struct Survey Survey;

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


void addQuestionMenu(Survey *s) {
    getchar();
    printf("A kerdes szovege: ");
    Question *tmp = (Question*) malloc(sizeof(Question));
    tmp->prev = s->head->prev;
    tmp->prev->next = tmp;
    tmp->next = s->head;
    s->head->prev = tmp;
    size_t buff = BUFFER_SIZE;
    tmp->text = NULL;
    getline(&tmp->text,&buff,stdin);
    printf("Valaszok szama: ");
    scanf("%d",&tmp->ansc);
    if ( tmp->ansc < 2) {
        printf("Minimum 2 valasz kell!\n");
        tmp->ansc = 2;
    }
    else if ( tmp->ansc > 4 ){
        printf("Maximum 4 valasz lehet!\n");
        tmp->ansc = 4;
    }
    tmp->ans = (char**) malloc ( sizeof(char*) * tmp->ansc );
    getchar();
    int i;
    for ( i = 0 ; i<tmp->ansc ; ++i){
        printf("%d. valasz: ",(i+1));
        tmp->ans[i] = NULL;
        getline(&tmp->ans[i],&buff,stdin);
    }
    (s->qnum)++;
    time_t t = time(NULL);
    tmp->cDate = *localtime(&t);
}

void printQuestions(Survey *s) {
    int i = 0;
    Question * p = s->head->next;
    while ( p != s->head ) {
        printf("\n");
        char buf[255];
        strftime(buf, sizeof(buf), "%Y-%m-%d. %H:%M:%S", &p->cDate);
        printf("Letrehozva: ");
        printf(buf);
        printf("\n%d. " , i+1);
        printf(p->text);
        int j; char c;
        for (j = 0, c = 'a' ; j<p->ansc ; ++j , ++c) {
            printf(" %c) ",c);
            printf(p->ans[j]);
        }
        p = p->next;
        ++i;
    }
}


void changeQuestionMenu(Survey *s) {
    printf("Modositando kerdes sorszama: ");
    int n;
    scanf("%d",&n);
    //n-=1;
    if ( n <= s->qnum ){
        Question * p = s->head;
        for (n ; n>0; --n) {
            p = p->next;
        }
        printSubMenu(p,"modositasa");
        int good = 0;
        while ( !good ){
            good = 1;
            printf("\n > ");
            int ch;
            scanf("%d",&ch);
            if ( ch == 1 ) {
                // Wants to change the question
                printf("Az uj kerdes: ");
                free(p->text);
                p->text = NULL;
                getchar();
                size_t buff = BUFFER_SIZE;
                getline(&p->text, &buff, stdin);
            }
            else if ( ch > 1 && ch < p->ansc+2 ){
                // Wants to change an answer
                printf("Az uj valasz: ");
                free(p->ans[ch-2]);
                p->ans[ch-2] = NULL;
                getchar();
                size_t buff = BUFFER_SIZE;
                getline(&p->ans[ch-2], &buff, stdin);
            }
            else if ( ch == p->ansc+2 ) {
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
    //n-=1;
    if ( n <= s->qnum ){
        Question * p = s->head;
        for (n ; n>0; --n) {
            p = p->next;
        }
        printSubMenu(p,"torlese");
        int good = 0;
        while ( !good ){
            good = 1;
            printf("\n > ");
            int ch;
            scanf("%d",&ch);
            if ( ch == 1 ) {
                // Wants to delete the question
                p->prev->next = p->next;
                p->next->prev = p->prev;
                p->next = NULL;
                p->prev = NULL;
                int i;
                for (i = 0; i < p->ansc; ++i) {
                    free(p->ans[i]);
                }
                free(p->ans);
                free(p->text);
                free(p);
            }
            else if ( ch > 1 && ch < p->ansc+2 ){
                // Wants to delete an answer
                if (p->ansc > 2) {
                    int i;
                    for (i = ch-2 ; i< p->ansc-1 ;++i) {
                        strcpy(p->ans[i], p->ans[i+1]);
                    }
                    --p->ansc;
                    free(p->ans[p->ansc]);
                }
                else {
                    printf("Legalabb 2 valasznak lennie kell!\n");
                }
            }
            else if ( ch == p->ansc+2 ) {
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

void finalizeQuestions(Survey * s, char * filename, Survey * fin) {

    Question * p = s->head->next;
    while (p != s->head) {
        printf("\n");
        char buf[255];
        strftime(buf, sizeof(buf), "%Y-%m-%d. %H:%M:%S", &p->cDate);
        printf("Letrehozva: ");
        printf(buf);
        printf("\n");
        //printf("\n%d. " , i+1);
        printf(p->text);
        int j; char c;
        for (j = 0, c = 'a' ; j<p->ansc ; ++j , ++c) {
            printf(" %c) ",c);
            printf(p->ans[j]);
        }
        getchar();
        int good = 0;
        while (!good) {
            good = 1;
            printf("Szerepeljen a kerdes a vegso konzultacios kerdoiven? (i/n) ");
            char a;
            scanf("%c",&a);
            if ( a == 'i' || a == 'I' ) {
                Question * cp = (Question*) malloc(sizeof(Question));
                cp->text = strdup(p->text);
                cp->ansc = p->ansc;
                cp->ans = (char**) malloc(sizeof(char*) * cp->ansc);
                int i;
                for (i = 0; i<cp->ansc; ++i ) {
                    cp->ans[i] = strdup(p->ans[i]);
                }
                cp->prev = fin->head->prev;
                cp->next = fin->head;
                fin->head->prev->next = cp;
                fin->head->prev = cp;
                ++fin->qnum;
            }
            else if (a == 'n' || a == 'N') {
            }
            else {
                good = 0;
            }
        }
        p = p->next;
    }
    if ( fin->qnum > 0) {
        FILE *f = fopen(filename,"w");
        if (f == NULL) {
            printf("Nem tudtam megnyitni a fajlt!");
        }
        else {
            int i=1;
            Question * p = fin->head->next;
            while (p != fin->head) {
                if (i > 1) {
                    fprintf(f,"\n");
                }
                fprintf(f,"%d. ", i);
                fprintf(f,p->text);
                //fprintf(f,"\n");
                int j; char c;
                for ( j = 0, c = 'a'; j< p->ansc; ++j, ++c) {
                    fprintf(f," %c) ",c);
                    fprintf(f,p->ans[j]);
                    //fprintf(f,"\n");
                }
                ++i;
                p = p->next;
            }
        }
        fclose(f);
    }
}


#endif
