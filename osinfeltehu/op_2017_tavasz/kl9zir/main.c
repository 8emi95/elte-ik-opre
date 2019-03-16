#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STARTING_CAP (15)


struct Question {
	char* date;
	char* text;
	int number;
	char** answer;
};
typedef struct Question Question;

struct Survey {
	int maxnumber;
	int number;
	int finalized;
    Question* question;
};
typedef struct Survey Survey;

void addQuestion(Survey *s) {
	if(s->finalized==false){
		if ( s->number == s->max ) {// allocate bigger array
			Question* newq = (Question*) malloc(sizeof(Question)*s.maxnumber*2);
			int i;
			for(i=0; i<s->max; ++i){
				newq[i]=s->question[i];
			}
			s->question=&newq;
		}
		getchar();
		printf("A kerdes: (max 100 karakter)");
		Question *question = &(s->question[s->number]);
		size_t length = 100;
		getline(&question->text,&length,stdin);
		printf("Adja meg a valaszok szamat: ");
		scanf("%d",&question->number);
		if ( question->number < 2) {
			printf("Minimum 2 valasz!\n");
			question->number = 2;
		}
		else if ( question->number > 4 ){
			printf("Maximum 4 valasz!\n");
			question->number = 4;
		}
		question->answer = (char**) malloc ( sizeof(char*) * tmp->number );
		getchar();
		for ( i = 0 ; i<question->number ; ++i){
			printf("%d. valasz: ",(i+1));
			getline(&question->answer[i],&buff,stdin);
		}
		time_t rawtime;
		struct tm *info;
		time( &rawtime );
		info = localtime( &rawtime );
		strftime(&question->date,80,"%x - %I:%M%p", info);
		(s->number)++;
	}
	else{
		printf("A kerdessort veglegesitettek")
	}
}

void printQuestions(Survey *s) {
    int i;
    for ( i = 0 ; i < s->number ; ++i) {
        printf("\n%d. ",(i+1));
        printf(s->question[i].text);
        int j;
        char c;
        for ( j = 0 ; j < s->q[i].ansc ; ++j) {
            printf(" %d. valasz ",j+1);
            printf(s->question[i].answer[j]);
        }  
    }
}