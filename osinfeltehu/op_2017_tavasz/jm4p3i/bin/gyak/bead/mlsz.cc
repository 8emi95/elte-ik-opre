#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> 
#include <unistd.h>
#include <time.h> 

#define QUESTIONS_MAX 20
#define QUESTION_LENGTH 200 
#define ANSWERS_MIN 2
#define ANSWERS_MAX 4
#define ANSWERS_LENGTH 200


typedef struct {
	char question[QUESTION_LENGTH];
	char answers[ANSWERS_MAX][ANSWERS_LENGTH];
	time_t created;
} questionElement;


void menu();

//Data-modifying functions
questionElement new_question();
void edit_question(questionElement * questions, int arrayCount);
void delete_question(questionElement questions[], int arrayCount);

//Printing
void list_questions(questionElement questions[], int arrayCount);
void printQuestionElement(questionElement element);

//File functions
void import_questions(questionElement * questions, int * arrayCount);
void export_questions(questionElement questions[], int arrayCount);

//Utility functions
void read_string(char *text, int maxlen);
void read_integer(int *num);




void printQuestionElement(questionElement element) {
	printf("%s", element.question);
	int i = 0; 
	char c = 'a';
	while(strlen(element.answers[i]) > 0 && i < ANSWERS_MAX) {
		printf("\n\t%c. %s", 'a'+i, element.answers[i]);
		i++;
	}

}

void read_string(char *text, int maxlen) {	
	char c;	
	int cnt = 0;

	scanf( "%s",  text);
  	cnt = strlen(text);

	while( (c=getchar())!=(int)'\n' && c != EOF ) {
		if (cnt < maxlen) {    
			text[cnt] = c;
			cnt++;
		}
	}
  
	if (maxlen != 0 && (int)strlen(text) > maxlen) {
		printf("Hiba: A szöveg hossza maximum %d karakter lehet \n", maxlen);
		read_string(text, maxlen);
	}
}

void read_integer(int *num) {
	if ( scanf("%d", num) == 1 ) {
       return;
	} else {
		scanf("%*s");
        printf("Hiba: Nem megfelelő számformátum!\n");
        read_integer(num);
    }
}


questionElement new_question() {
	questionElement question;

	printf("\nKérem adja meg a kérdést (max %d karakter):", QUESTION_LENGTH);
  	read_string(question.question, (int)QUESTION_LENGTH);

  	int answer_count = 0;
  	printf("\nKérem adja meg a válaszok számát: (%d-%d):", (int)ANSWERS_MIN, (int)ANSWERS_MAX);
  	do {
  		read_integer(&answer_count);
  	} while (answer_count < (int)ANSWERS_MIN || answer_count > (int)ANSWERS_MAX);

  	for(int i = 0; i < answer_count; i++) {
  		printf("\n%d. válasz: ", i+1);
  		read_string(question.answers[i], (int)ANSWERS_LENGTH);
  	}

  	question.created = time(0);

  	return question;
}

void edit_question(questionElement * questions, int arrayCount) {
	printf("\nKérem adja meg a módosítandó kérdés sorszámát (1-%d):", arrayCount);

	int elementToModify;
	read_integer(&elementToModify);

	questionElement question = new_question();
	questions[elementToModify-1] = question;
}

void delete_question(questionElement * questions, int arrayCount) {
	printf("\nKérem adja meg a törlendő kérdés sorszámát (1-%d):", arrayCount);

	int elementToRemove;
	read_integer(&elementToRemove);

	int j = 0;	
	for(int i = 0; i < arrayCount; i++) {
		if(elementToRemove != i+1) {
			questions[j] = questions[i];
			j++;
		}
	}
}

void list_questions(questionElement questions[], int arrayCount) {
	for(int i = 0; i < arrayCount; i++) {
		printf("\n%d. Létrehozva: %s", i+1, ctime(&questions[i].created));
		printQuestionElement(questions[i]);
	}
}

void export_questions(questionElement questions[], int arrayCount) {
	int fd; 
	fd= open("vegleges", O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);

	if ( fd < 0 ){ 
		perror("\nHiba a fájl megnyitása közben\n");
		return; 
	}

	write(fd, &arrayCount, sizeof(int));
	write(fd, questions, sizeof(questionElement) * (int)QUESTIONS_MAX);
	close(fd);

	printf("\nAz adatok fájlba írása megtörtént.\n");
}

void import_questions(questionElement * questions, int * arrayCount) {
	int fd; 
	fd= open("vegleges", O_RDONLY);

	read(fd, arrayCount, sizeof(int));
	read(fd, questions, sizeof(questionElement) * *arrayCount);
	list_questions(questions, *arrayCount);
}

void menu() {
	questionElement questions[QUESTIONS_MAX];
	int arrayCount = 0;
    int choice;

    do {
		printf("\nKérem válasszon az alábbi menüpontok közül\n\n");
		printf("1. Kérdés felvétele\n");
		printf("2. Kérdés módosítása\n");
		printf("3. Kérdés törlése\n");
		printf("4. Kérdések listázása\n");
		printf("5. Kérdések véglegesítése \n");
		printf("6. Kérdések beolvasása \n");
		printf("9. Kilépés\n");
		
		printf("Válasszon az alábbi lehetőségek közül:");
		read_integer(&choice);
		
		switch(choice) {
		  case 1:
		  	questions[arrayCount] = new_question();
		  	arrayCount++;
			break;
		  case 2:
		  	edit_question(questions, arrayCount);
			break;
		  case 3:
			delete_question(questions, arrayCount);
			arrayCount--;
			break;
		  case 4:
			list_questions(questions, arrayCount);
			break;
		  case 5:
			export_questions(questions, arrayCount);
			break;
		  case 6:
			import_questions(questions, &arrayCount);
			break;
		  case 9:
			exit(0);
			break;
		  default:
			printf("Nem létező menüpont!\n");
			break;
		}
    } while ( choice != 9);
}



int main(int argc,char** argv) {
	menu();

	return 0;
}