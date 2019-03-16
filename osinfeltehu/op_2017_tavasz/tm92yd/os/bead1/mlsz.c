//Operációs rendszerek 1. Beadandó - Varga Máté István - TM92YD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int menu() {
	int selection = 0;
	
	printf("Magyar Labdajatekok Szervezete (MLSZ) nemzeti konzultacio\n\n");
	printf("Kerem valasszon az alabbi lehetosegek kozul:\n\n");
	printf("\t1. Kerdessor megalkotasa\n");
	printf("\t2. Uj kerdes felvetele\n");
	printf("\t3. Kerdes modositasa\n");
	printf("\t4. Kerdes torlese\n");
	printf("\t5. Kerdesek listazasa\n");
	printf("\t6. Kilepes a programbol\n\n");	
	printf("Kerem adja meg a valasztott funkcio szamat: ");
	scanf("%d", &selection);

	return selection;
}

int main(int argc,char ** argv) {
    int selection = menu();
	
	while (selection != 6) {
		switch(selection) {
			case 1:
				newQuestionnaire();
				break;
			case 2:
				newQuestion();
				break;
			case 3:
				modifyQuestion();
				break;
			case 4:
				removeQuestion();
				break;
			case 5:
				listQuestions();
				break;
			case 6:
				return 0;
			default:
				printf("\n\nA megadott szam ervenytelen, kerem probalja ujra!\n\n");
				break;
		}

		selection = menu();
	}
}

newQuestionnaire() {
	FILE *questions = fopen("questions.txt", "r");
	FILE *questionnaire = fopen("questionnaire.txt", "a+");

	printf("Adja meg hany kerdest szeretne a kerdessorhoz!: \n");
	getchar();

	char answers = getchar();
	int n = answers - '0';

	int questions_number[n];

	listQuestions();
	printf("Adja meg a kerdessek sorszamat majd nyomjon egy entert!\n");

	int i;
	for(i=0; i<n; ++i) {
		scanf("%i", &questions_number[i]);
	}

	int j = 0;
	char currentLine[256];
	while (fgets(currentLine, sizeof(currentLine), questions)) {
		++j;
		for(i=0; i<n; ++i) {
			//printf("%i j , %i, qn\n", j, questions_number[i]);
			if(questions_number[i] == j) {
				fprintf(questionnaire, currentLine);
			}
		}
	}
	fclose(questions);
	fclose(questionnaire);

	printf("A kerdessor letrejott a questionnare.txt fajlban!\n");
}

newQuestion() {
	FILE *questions = fopen("questions.txt", "a+");

	char question[256] = "";
	char answer1[256] = "";
	char answer2[256] = "";
	char answer3[256] = "";
	char answer4[256] = "";

	printf("Adja meg hany valaszlehetoseget szeretne a kerdeshez (2-4)!: \n");
	getchar();

	char answers = getchar();

	int i;
	int n = answers - '0';

	getchar();

	for(i=1; i<=n+1; ++i) {
		if(i==1) {
			printf("Kerdes: ");
			scanf("%s", question);
		} else if(i==2) {
			printf("Valasz 1: ");
			scanf("%s", answer1);
		} else if(i==3) {
			printf("Valasz 2: ");
			scanf("%s", answer2);
		} else if(i==4) {
			printf("Valasz 3: ");
			scanf("%s", answer3);
		} else if(i==5) {
			printf("Valasz 4: ");
			scanf("%s", answer4);
		}
	}

	printf("A kerdes letrejott.\n\n");
	printf("A folytatashoz kerem nyomjon meg egy gombot...\n");

	time_t currentTime;
	struct tm * timeinfo;
	char currTime[80];

	time (&currentTime);
	timeinfo = localtime (&currentTime);
	
	strftime(currTime, 80, "%F %T", timeinfo);

	fprintf(questions, "%s %s %s %s %s %s\n", question, answer1, answer2, answer3, answer4, currTime);
	fclose(questions);
	getchar();
}

modifyQuestion() {
	listQuestions();
	printf("Kerem adja meg a modositando kerdes sorszamat: ");
	int n = 0;
	scanf("%d", &n);

	FILE *questions = fopen("questions.txt", "r");
	FILE *new_questions = fopen("new_questions.txt", "a+");

	char currentLine[256];
	int j = 0;
	while (fgets(currentLine, sizeof(currentLine), questions)) {
		++j;
		if(j == n) {
			char question[256] = "";
			char answer1[256] = "";
			char answer2[256] = "";
			char answer3[256] = "";
			char answer4[256] = "";
			printf("Adja meg hany valaszlehetoseget szeretne a kerdeshez (2-4)!: \n");
			getchar();

			char answers = getchar();;

			int i;
			int n = answers - '0';

			getchar();

			for(i=1; i<=n+1; ++i) {
				if(i==1) {
					printf("Kerdes: ");
					scanf("%s", question);
				} else if(i==2) {
					printf("Valasz 1: ");
					scanf("%s", answer1);
				} else if(i==3) {
					printf("Valasz 2: ");
					scanf("%s", answer2);
				} else if(i==4) {
					printf("Valasz 3: ");
					scanf("%s", answer3);
				} else if(i==5) {
					printf("Valasz 4: ");
					scanf("%s", answer4);
				}
			}

			time_t currentTime;
			struct tm * timeinfo;
			char currTime[80];

			time (&currentTime);
			timeinfo = localtime (&currentTime);
			
			strftime(currTime, 80, "%F %T", timeinfo);
			
			fprintf(new_questions, "%s %s %s %s %s Modositva: %s\n", question, answer1, answer2, answer3, answer4, currTime);
			getchar();
		} else {
			fprintf(new_questions, currentLine);
		}
	}

	fclose(questions);
	fclose(new_questions);

	remove("questions.txt");
	rename("new_questions.txt", "questions.txt");
}

removeQuestion() {
	listQuestions();
	printf("Kerem adja meg az eltavolitando kerdes sorszamat: ");
	int n = 0;
	scanf("%d", &n);

	FILE *questions = fopen("questions.txt", "r");
	FILE *new_questions = fopen("new_questions.txt", "a+");

	char currentLine[256];
	int i = 1;
	while (fgets(currentLine, sizeof(currentLine), questions)) {
		++i;
		if(i != n+1) {
			fprintf(new_questions, currentLine);
		}
	}

	fclose(questions);
	fclose(new_questions);

	remove("questions.txt");
	rename("new_questions.txt", "questions.txt");
}

listQuestions() {
	getchar();

	FILE *questions = fopen("questions.txt", "r");

	char currentLine[256];
	int i = 1;
	while (fgets(currentLine, sizeof(currentLine), questions)) {
		printf("%d. %s", i, currentLine);
		++i;
	}

	fclose(questions);
/*
	printf("\n\nA folytatashoz kerem nyomjon meg egy gombot...");
	getchar();
*/
}
