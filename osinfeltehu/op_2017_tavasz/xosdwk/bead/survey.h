#ifndef _SURVEY_H_
#define _SURVEY_H_

#define BUFFER_SIZE (80)

struct Question {
    int id;
    char* text;
    int ansc;
    char** ans;
    int date[6];
    struct Question * next;
    struct Question * prev;
};
typedef struct Question Question;

void qstcpy( Question *dest, Question *source ){
    // may leak a bit, teehee ^^
    //free(dest->ans);
    dest->ans = (char**) malloc(sizeof(char*)*source->ansc);

    //dest->id = source->id;
    strcpy(dest->text,source->text);
    dest->ansc = source->ansc;
    //int i;
    //for (i=0; i<source->ansc; ++i){
    //    strcpy(dest->ans[i],source->ans[i]);
    //}
    //for (i=0; i<6; ++i) {
    //    dest->date[i] = source->date[i];
    //}
}

void qstswap( Question *dest, Question *source ){
    Question tmp;
    qstcpy(&tmp,dest);
    qstcpy(dest,source);
    qstcpy(source,&tmp);
}

struct Survey {
    int qnum;
    int max;
    Question *head;
};
typedef struct Survey Survey;

void addQuestionMenu(Survey *s) {
    if ( s->qnum == s->max ) {
        //should allocate a bigger array
        //TODO: actually implementing it :)
    }
    getchar();
    printf("A kerdes szovege: ");
    Question *tmp = &(s->q[s->qnum]);
    size_t buff = BUFFER_SIZE;
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
        getline(&tmp->ans[i],&buff,stdin);
    }
    //TODO: adding date to the question
    if ( s->qnum == 0 ) {
        tmp->id = 1;
    } 
    else {
        tmp->id = s->q[s->qnum-1].id+1;
    }
    (s->qnum)++;
}

void printQuestions(Survey *s) {
    int i;
    for ( i = 0 ; i < s->qnum ; ++i) {
        printf("\n%d. ",s->q[i].id);
        printf(s->q[i].text);
        int j;
        char c;
        for ( j = 0, c = 'a' ; j < s->q[i].ansc ; ++j , ++c) {
            printf(" %c) ",c);
            printf(s->q[i].ans[j]);
        }  
    }
}

#endif
