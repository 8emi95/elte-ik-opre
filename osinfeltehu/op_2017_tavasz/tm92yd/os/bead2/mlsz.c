//Operációs rendszerek 2. Beadandó - Varga Máté István - TM92YD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

struct questions {
	char question[256];
	char answer1[256];
	char answer2[256];
	char answer3[256];
	char answer4[256];
	char date[80];
};

struct questions qs[256];
int questionNumber = 5;

int menu() {
	int selection = 0;
	
	printf("Magyar Labdajatekok Szervezete (MLSZ) nemzeti konzultacio\n\n");
	printf("Kerem valasszon az alabbi lehetosegek kozul:\n\n");
	printf("\t1. Kerdessor megalkotasa\n");
	printf("\t2. Uj kerdes felvetele\n");
	printf("\t3. Kerdes modositasa\n");
	printf("\t4. Kerdes torlese\n");
	printf("\t5. Kerdesek listazasa\n");
	printf("\t6. Kerdessor kikuldese, majd kilepes a programbol\n");	
	printf("Kerem adja meg a valasztott funkcio szamat: ");
	scanf("%d", &selection);

	return selection;
}

int main(int argc,char ** argv) {
	addQuestions();

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

	//Random kérdések
	struct questions sendQS[3];
	int qsN = 3;
	int r = 0;
	int lastRandom = 0;

	int i;
	for(i=0; i<qsN; ++i) {
		while(r == lastRandom) {
			srand(time(NULL));
			r = rand() % questionNumber;
		}

		strcpy(sendQS[i].question, qs[r].question);
		strcpy(sendQS[i].answer1, qs[r].answer1);
		strcpy(sendQS[i].answer2, qs[r].answer2);
		strcpy(sendQS[i].answer3, qs[r].answer3);
		strcpy(sendQS[i].answer4, qs[r].answer4);
		strcpy(sendQS[i].date, qs[r].date);
		lastRandom = r;
	}

	/*
	//küldendő kérdések
	for(i=0; i<qsN; ++i) {
		printf("Q: %s, A1: %s, A2: %s, A3: %s, A4: %s, D: %s\n", sendQS[i].question, sendQS[i].answer1, sendQS[i].answer2, sendQS[i].answer3, sendQS[i].answer4, sendQS[i].date);
	}
	*/ 

	int pipefd[2]; 
    pid_t pid;
	char buffer[512];

	srand(time(NULL));
	int answers = rand() % 10 + 10;

	printf("Valaszolok szama: %i\n", answers);

	struct questions getQS[3];

	if (pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

	int status;	

	pid = fork();
    if (pid == -1)  {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    } 

	 if (pid == 0) {
		//Gyerek
		//Bejelentkezés
		printf("K -- Kerdezobiztos bejelentkezik\n");
		write(pipefd[1], "Felkeszultem a kerdesek fogadasara!",36);

		sleep(2);

		//Olvasas
		printf("K -- Kerdezobiztos olvassa a kerdeseket!\n");
		int i;
		for(i=0; i<qsN; ++i) {
			read(pipefd[0], getQS[i].question, sizeof(getQS[i].question));
			read(pipefd[0], getQS[i].answer1, sizeof(getQS[i].answer1));
			read(pipefd[0], getQS[i].answer2, sizeof(getQS[i].answer2));
			read(pipefd[0], getQS[i].answer3, sizeof(getQS[i].answer3));
			read(pipefd[0], getQS[i].answer4, sizeof(getQS[i].answer4));
			read(pipefd[0], getQS[i].date, sizeof(getQS[i].question));
		}
		printf("K -- Kerdezobiztos kiolvasta a kerdeseket!\n");
		close(pipefd[0]);
				
		//Kapott kérdesek
		for(i=0; i<qsN; ++i) {
			printf("Q: %s, A1: %s, A2: %s, A3: %s, A4: %s, D: %s\n", getQS[i].question, getQS[i].answer1, getQS[i].answer2, getQS[i].answer3, getQS[i].answer4, getQS[i].date);
		}

		//Küldés
		printf("K -- Valaszok kuldese az MLSZ-nek!\n");
		int j;
		for(j=0; i<answers; ++i) {
			for(i=0; i<qsN; ++i) {
				srand(time(NULL));
				int randomAnswer = rand() % 4 + 1;
				int ok = 0;
				while(ok != 1) {
					if(randomAnswer == 1) {
						strcpy(buffer, getQS[i].question);
						strcat(buffer, " - ");
						strcat(buffer, getQS[i].answer1);
						ok = 1;
					} else if(randomAnswer == 2) {
						strcpy(buffer, getQS[i].question);
						strcat(buffer, " - ");
						strcat(buffer, getQS[i].answer2);
						ok = 1;
					} else if(randomAnswer == 3) {
						if(strcmp("", getQS[i].answer3) != 0) {
							strcpy(buffer, getQS[i].question);
							strcat(buffer, " - ");
							strcat(buffer, getQS[i].answer3);
							ok = 1;
						} else {
							randomAnswer = rand() % 4 + 1;
						}
					} else if(randomAnswer == 4) {
						if(strcmp("", getQS[i].answer4) != 0) {
							strcpy(buffer, getQS[i].question);
							strcat(buffer, " - ");
							strcat(buffer, getQS[i].answer4);
							ok = 1;
						} else {
							randomAnswer = rand() % 4 + 1;
						}
					} 
				}
				write(pipefd[1], buffer ,sizeof(buffer));
			}	
			sleep(1);
		}
		close(pipefd[1]);
	 } else {
		//Szülő	
		//Bejelentkezese
		printf("MLSZ varakozik a valaszolora\n");
		read(pipefd[0], buffer, sizeof(buffer));
		printf("%s\n",buffer);

		//Küldés
		printf("MLSZ kiuldi a kerdeseket!\n");
		int i;
		for(i=0; i<qsN; ++i) {
			write(pipefd[1], sendQS[i].question, sizeof(sendQS[i].question));
			write(pipefd[1], sendQS[i].answer1, sizeof(sendQS[i].answer1));
			write(pipefd[1], sendQS[i].answer2, sizeof(sendQS[i].answer2));
			write(pipefd[1], sendQS[i].answer3, sizeof(sendQS[i].answer3));
			write(pipefd[1], sendQS[i].answer4, sizeof(sendQS[i].answer4));
			write(pipefd[1], sendQS[i].date, sizeof(sendQS[i].question));
		}
		printf("MLSZ kikuldte a kerdeseket!\n");
		close(pipefd[1]);

		sleep(10);

		//Olvasas
		printf("MLSZ olvassa a valaszokat!\n");
		int j;
		for(j=0; j<answers; ++j) {
			printf("%i . valaszolo\n", j+1);
			for(i=0; i<qsN; ++i) {
				read(pipefd[0], buffer, sizeof(buffer));
				printf("Valaszok: %s \n",buffer);
			}
			sleep(2);
		}
		printf("\n\n");
		close(pipefd[0]);
	    printf("MLSZ befejezte!\n");	
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

	strcpy(qs[questionNumber].question, question);
	strcpy(qs[questionNumber].answer1, answer1);
	strcpy(qs[questionNumber].answer2, answer2);
	strcpy(qs[questionNumber].answer3, answer3);
	strcpy(qs[questionNumber].answer4, answer4);
	strcpy(qs[questionNumber].date, currTime);

	++questionNumber;

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

}

addQuestions() {
	if( access( "questions.txt", F_OK ) != -1 ) {
		int ret = remove("questions.txt");
	/*
		if(ret == 0) {
			printf("File deleted successfully\n");
		} else {
			printf("Error: unable to delete the file\n");
		}
	*/
	}

	FILE *questions = fopen("questions.txt", "a+");

	char question[256] = "Ön jár/járt magyar futballmérkőzésre?";
	char answer1[256] = "Igen, jelenleg is rendszeresen";
	char answer2[256] = "Nem";
	char answer3[256] = "Igen, régebben";
	char answer4[256] = "";
	char date[80] = "2017-04-07 13:45:08";

	strcpy(qs[0].question, question);
	strcpy(qs[0].answer1, answer1);
	strcpy(qs[0].answer2, answer2);
	strcpy(qs[0].answer3, answer3);
	strcpy(qs[0].answer4, answer4);
	strcpy(qs[0].date, date);

	fprintf(questions, "%s %s %s %s %s %s\n", question, answer1, answer2, answer3, answer4, date);

	strcpy(question, "Önt ebben mennyire befolyásolta az a kép, amit a média fest a magyar futballról?");
	strcpy(answer1, "Egyáltalán nem");
	strcpy(answer2, "Teljes mértékben");
	strcpy(answer3, "");
	strcpy(answer4, "");
	strcpy(date, "2017-04-07 13:51:36");

	strcpy(qs[1].question, question);
	strcpy(qs[1].answer1, answer1);
	strcpy(qs[1].answer2, answer2);
	strcpy(qs[1].answer3, answer3);
	strcpy(qs[1].answer4, answer4);
	strcpy(qs[1].date, date);

	fprintf(questions, "%s %s %s %s %s %s\n", question, answer1, answer2, answer3, answer4, date);

	strcpy(question, "Ön szerint mennyire jellemző a magyar futballra a huliganizmus?");
	strcpy(answer1, "Egyáltalán nem");
	strcpy(answer2, "Ritkán fordul elő");
	strcpy(answer3, "A szurkolók egy rétegére");
	strcpy(answer4, "Teljes mértékben");

	strcpy(date, "2017-04-07 13:50:31");

	strcpy(qs[2].question, question);
	strcpy(qs[2].answer1, answer1);
	strcpy(qs[2].answer2, answer2);
	strcpy(qs[2].answer3, answer3);
	strcpy(qs[2].answer4, answer4);
	strcpy(qs[2].date, date);

	fprintf(questions, "%s %s %s %s %s %s\n", question, answer1, answer2, answer3, answer4, date);

	strcpy(question, "Önt zavarja-e, ha a szurkolók a stadionbeli hangulat fokozása érdekében görögtüzeket égetnek, petárdákat használnak?");
	strcpy(answer1, "Igen");
	strcpy(answer2, "Nem");
	strcpy(answer3, "");
	strcpy(answer4, "");
	strcpy(date, "2017-04-07 13:50:59");

	strcpy(qs[3].question, question);
	strcpy(qs[3].answer1, answer1);
	strcpy(qs[3].answer2, answer2);
	strcpy(qs[3].answer3, answer3);
	strcpy(qs[3].answer4, answer4);
	strcpy(qs[3].date, date);

	fprintf(questions, "%s %s %s %s %s %s\n", question, answer1, answer2, answer3, answer4, date);

	strcpy(question, "Ön lát esélyt arra, hogy a magyar futball problémái a közeljövőben megoldódjanak?");
	strcpy(answer1, "Igen");
	strcpy(answer2, "Nem");
	strcpy(answer3, "");
	strcpy(answer4, "");
	strcpy(date, "2017-04-07 13:52:08");

	strcpy(qs[4].question, question);
	strcpy(qs[4].answer1, answer1);
	strcpy(qs[4].answer2, answer2);
	strcpy(qs[4].answer3, answer3);
	strcpy(qs[4].answer4, answer4);
	strcpy(qs[4].date, date);

	fprintf(questions, "%s %s %s %s %s %s\n", question, answer1, answer2, answer3, answer4, date);
	fclose(questions);
}
