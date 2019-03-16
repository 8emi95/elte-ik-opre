#ifndef OPREBEADANDO1_H
#define OPREBEADANDO1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// stored in headed chained list.
struct Answer_list{
	char*					answer;
	struct Answer_list*		next;
};

// stored in headed chained list.
struct Question_list{
	time_t					time;
	char 					*question;
	int						answerNumb;
	struct Answer_list*		answers;
	struct Question_list*	next;
};

// The number of questions.
int questionNumb = 0;

// shorting of typenames.
typedef struct Question_list Question_list;
typedef struct Answer_list Answer_list;

// FUNCTIONS -------------------------------------


// stepping to the end of the buffer to ignore the wasted enters by scanf().
void stepEnd(void) {
  
}

// it reads in strings dynamically.
char *inputString(FILE* fp, size_t size){
	fseek(fp,0,SEEK_END);
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size); //size is the starting size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

// it writes out the main menu.
void Menu(){
	printf("\n\nWritten by: Olay Bence\n");
	printf("Main menu:\n");
	printf("\t1. Modifying survey\n");
	printf("\t2. Listing the survey\n");
	printf("\t3. Finalization\n");
	printf("\t4. Exit\n\n");
	printf("\t----------------------\n");
	printf("\tChoose a menupoint(1-4): ");
}

// it writes out the modifying menu.
void modifyingMenu(){
	printf("\nModifying menu:\n");
	printf("\t1. Add question\n");
	printf("\t2. Modify question\n");
	printf("\t3. Delete question\n");
	printf("\t4. Back to the main menu\n\n");
	printf("\t----------------------\n");
	printf("\tChoose a menupoint(1-4): ");
}


// listing questions.
void listingQuestions(Question_list *survey){
	Question_list* p = survey->next;
	Answer_list* ans;
	int i = 0;
	int j = 0;
	if(survey->next == NULL) {
		printf("Nincs egy kerdes se.\n");
	} else {
		while(p != NULL){
			printf("\n");
			struct tm *tmp;
			tmp = gmtime(&p->time);
			printf("%d-%02d-%02d %02d:%02d:%02d\n",tmp->tm_year+1900,tmp->tm_mon,tmp->tm_mday, tmp->tm_hour+2, tmp->tm_min, tmp->tm_sec);
			printf("%d. Question:\n\t", i+1);
			printf(p->question);
			printf("\n");
			ans = p->answers->next;
			j = 0;
			while(ans != NULL) {
				printf("\t%d.) ", j+1);
				printf(ans->answer);
				printf("\n");
				ans = ans->next;
				j++;
			}
			p = p->next;
			i++;
		}
	}
}

// finilazing questions and writting into file.
void writeIntoFile(Question_list *survey){
	char fileName[1024];
	printf("File name (without expansion(.txt)): ");
	fseek(stdin,0,SEEK_END);
	scanf("%s",&fileName);
	strcat(fileName, ".txt");
	FILE* f = fopen(fileName, "w+");
	
	Question_list* q = survey->next;
	Answer_list* a;
	int i = 0;
	int j = 0;
	int k = 0;
	char c;
	
	if( q == NULL ) {
		printf("There isn't any question!\n");
	} else {
		while( q != NULL ){
				printf("\n%d. ", i+1);
				printf(q->question);
				printf("\n");
				a = q->answers->next;
				j = 0;
				while( a != NULL ) {
					printf("\t%d) ", j+1);
					printf(a->answer);
					printf("\n");
					a = a->next;
					j++;
				}
				printf("Do you want to save this question? (Y/N): ");
				fseek(stdin,0,SEEK_END);
				scanf("%c", &c);
				if(c == 'Y' || c == 'y'){
					k++;
					fprintf(f, "%d:", k);
					fprintf(f, "%s\n", q->question);
					a = q->answers->next;
					j = 0;
					while(a != NULL) {
						fprintf(f,"\t%d.) ", j+1);
						fprintf(f,a->answer);
						fprintf(f,"\n");
						a = a->next;
						j++;
					}
				}
				q = q->next;
				i++;
		}
	}
	
	fclose(f);
}

#endif