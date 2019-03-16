#ifndef _SURVEY_H_
#define _SURVEY_H_

#define BUFFER_SIZE (80)

struct Question {
    char* text;
    int ansc;
    char** ans;
    int date[6];
};
typedef struct Question Question;

struct Survey {
    int final_b;
    int qnum;
    int max;
    Question *q;
};
typedef struct Survey Survey;

void addQuestionMenu(Survey *s) {
    if(s->final_b==0)
    {
        if ( s->qnum == s->max ) {
                //should allocate a bigger array
                //TODO: actually implement it :)
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
            (s->qnum)++;
    }else
    {
        printf("A kerdoiv veglegesitett!\n");
    }
}

void printQuestions(Survey *s) {
    int i;
    for ( i = 0 ; i < s->qnum ; ++i) {
        printf("\n%d. ",(i+1));
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
