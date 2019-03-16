#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int nextFreeSpaceForQuestion = 0;
int finalized = 0;

char questionArray[10][100];
char answerArray[40][100];
char dateArray[10][25];

char *currentTime() {

	char *cd = (char*)malloc(sizeof(char)*25);
    time_t ct;
    ct = time(NULL);
    sprintf(cd, "%s", asctime(localtime(&ct)));
    cd[25] = '\0';
    return cd;

}

void arrayFill() {
	for (int i = 0; i < 10; ++i) {
		questionArray[i] = "---";
	}

	for (int i = 0; i < 40; ++i) {
		answerArray[i] = "---";
	}
}

void createQuestion() {

	char temp[100];
	int tmp;

	if (nextFreeSpaceForQuestion == -1) {

		printf("You have no place on your sheet for any more questions!\n");
		printf("Limit for questions is 10\n");
		printf("Delete a question to have place!\n");

	} else {

		printf("Give me the question! (Maximum 100 characters)\n");
		scanf("%s", temp);
		strcpy(questionArray[nextFreeSpaceForQuestion], temp);

		printf("Now we just need answers!\nPlease add atleast 2, if you don't want any more answer options, just fill it with ---\n\n");

		for (int i = 0; i < 4; ++i) {
			printf("Answer option %i: \n");
			scanf("%s", temp);
			tmp = 4 * nextFreeSpaceForQuestion + i;
			strcpy(answerArray[tmp], temp);
		}

		dateArray[nextFreeSpaceForQuestion] = currentTime();

		tmp = 0;

		for (int i = nextFreeSpaceForQuestion + 1; i < 10; ++i) {
			if ( strcmp("---", questionArray[i]) == 0 && tmp == 0 ) {
				tmp = 1;
				nextFreeSpaceForQuestion = i;
			}
		}

		if ( tmp == 0 ) {
			nextFreeSpaceForQuestion = -1;
		}

		printf("Nice, great!\n\n");

	}
}

void modifyQuestion(int questionNumber) {
	if (questionNumber < 0 || questionNumber > 9) {
		printf("Please give me a valid number!\n\n");
	} else {


		int loopBool = 0;
		char switchChar;
		char tmp[100];

		do {


			printf("Modifying this question: %s\n", questionArray[questionNumber]);
			printf("What to modify?\nTo modify the question, hit: Q\nTo modify the answers, smash: A\nTo modify both, demolish: B\n");

			scanf("%c", switchChar);

			switch (switchChar) {
				case 'Q': case 'q':
					
					printf("Give me the new question!\n");
					scanf("%s", tmp);
					strcpy(questionArray[questionNumber], tmp);

					dateArray[nextFreeSpaceForQuestion] = currentTime();

					break;

				case 'A': case 'a':

					for (int i = 0; i < 4; ++i) {
						printf("Give me the %i. answer option!\n", i+1);
						scanf("%s", tmp);
						strcpy(answerArray[4*questionNumber+i], tmp);
					}

					dateArray[nextFreeSpaceForQuestion] = currentTime();

					break;

				case 'B': case 'b':

					printf("Give me the question!\n");
					scanf("%s", tmp);
					strcpy(questionArray[questionNumber], tmp);

					for (int i = 0; i < 4; ++i) {
						printf("Give me the %i. answer option!\n", i+1);
						scanf("%s", tmp);
						strcpy(answerArray[4*questionNumber+i], tmp);
					}

					dateArray[nextFreeSpaceForQuestion] = currentTime();

					break;
				default:

					printf("Please type in a valid character!\n");
					loopBool = 1;
			}

		} while (loopBool == 1);

		printf("thenk :)\n");

	}
}

void deleteQuestion(int questionNumber) {
	if (questionNumber < 0 || questionNumber > 9) {
		printf("Not a valid numberino\n");
	} else {

		printf("Deleting this question and all their answers: %s", questionArray[questionNumber]);
		printf("Are you sure? Y : N");

		char switchChar;
		scanf("%c", switchChar);

		switch (switchChar) {
			case 'Y': case 'y': 
				printf("okeydokey its deleted\n");
				strcpy(questionArray[questionNumber], "---");
				for (int i = 0; i < 4; ++i) {
					strcpy(answerArray[4*questionNumber+i], "---");
				}
		}

	}
}

void listQuestions(int answers) {
	if (answers == 1) {

		for (int i = 0; i < 10; ++i) {

			if (strcmp(questionArray[i], "---") != 0) {
				printf("%i. %s\n", i+1, questionArray[i]);
			}

		}

	} else {
		for (int i = 0; i < 10; ++i) {

			if (strcmp(questionArray[i], "---") != 0) {
				printf("%i. %s\n", i+1, questionArray[i]);
				printf("%s\n", answerArray[4*i]);
				printf("%s\n", answerArray[4*i+1]);
				printf("%s\n", answerArray[4*i+2]);
				printf("%s\n", answerArray[4*i+3]);
				printf("%s\n", dateArray[i]);
				printf("\n");
			}
			
		}
	}
}

struct kuldendo {
	char question[3][100];
	char answer[12][100];
};

int main() {

	char switchAnswer;
	int timeToStop = 0;
	int questionNumber;
	char timeToDecide;
	char *filename;
	File fileObj;

	arrayFill();
	
	printf("Well met!\n\n");

	do {

		printf("Give me a new name for your question sheet!\n");
		scanf("%s", filename);

		fileObj = fopen(filename, "rb+");
		if (fileObj != NULL) {
			printf("There is a sheet with this name already!\n");
		} else {

			freopen(filename, "wb", fileObj);
			timeToStop = 1;
		
		}

	} while (timeToStop == 0);

	timeToStop = 0;

	do {
		if (finalized == 0) {
			printf("\nFor creating a new question, press: 1\n");
			printf("For modifying an existing question, hit: 2\n");
			printf("For deleting an existing question, smash: 3\n");
			printf("To finalize the question sheet, obliterate: 4\n");
		}
		printf("To list all the submitted questions and answers, demolish: 5\n");
		printf("To exit, strike: 6\n");
		scanf("%c", switchAnswer);

		switch(switchAnswer) {
			case '1':

				createQuestion();
				break;

			case '2':

				listQuestions(0);
				scanf("%i", questionNumber);
				modifyQuestion(questionNumber-1);
				break;

			case '3':

				listQuestions(0);
				scanf("%i", questionNumber);
				deleteQuestion(questionNumber-1);
				break;

			case '4':

				printf("Are you sure you want to finalize your Q&A sheet?\n");
				printf("Once you finalize it, you can no longer add, change or delete questions or answers!\n")
				printf("Confirm: Y. Cancel: Anything else");
				scanf("%c", timeToDecide);
				if (timeToDecide == 'Y' || timeToDecide == 'y') {
					finalized = 1;
					printf("You finalized your sheet!\n\n");
				}

				break;

			case '5':

				listQuestions(1);
				break;

			case '6':

				for (int i = 0; i < 10; ++i) {

					if (strcmp(questionArray[i], "---") != 0) {

						fputs("1.\n", fileObj);
						fputs(questionArray[i], fileObj);
						fputs(answerArray[4*i], fileObj);
						fputs(answerArray[4*i+1], fileObj);
						fputs(answerArray[4*i+2], fileObj);
						fputs(answerArray[4*i+3], fileObj);
						fputs(dateArray[i], fileObj);

					}

				}

				printf("It is finished.\n\n");
				timeToStop = 1;
				break;

			default:
				printf("What are you doing?\n");
		}


	} while (timeToStop == 0);

	//szülő gyerek things

	int pipefd[4];
	pid_t pid;
	

	if (pipe(pipefd) == -1) {
		perror("Hiba a pipe nyitaskor");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	} else {
		printf("Piros kerdobiztos keszen all\n\n");
	}

	if (pid == 0) {

		sleep(2);
		printf("Piros kerdobiztos elkezd olvasni!\n");
		struct kuldendo k;
		read(pipefd[0],k,sizeof(k));
		printf("piros kerdobiztos beolvasta, elvegzi a kutatast!\n");
		srand(time(NULL));
		int kitoltok = (rand() % 11) + 10;
		int tmp = 2 * kitoltok;
		char elso[tmp];
		char masodik[tmp];
		char harmadik[tmp];

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < kitoltok; ++j) {
				if (i == 0) {

					srand(time(NULL));
					tmp = rand % 4 + 1;
					strcat(elso, tmp + " ");

				} else if (i == 1) {

					srand(time(NULL));
					tmp = rand % 4 + 1;
					strcat(masodik, tmp + " ");

				} else {

					srand(time(NULL));
					tmp = rand % 4 + 1;
					strcat(harmadik, tmp + " ");

				}
			}
		}
		strcat(elso, "\n");
		strcat(masodik, "\n");
		strcat(harmadik, "\n");

		printf("Piros kerdobiztos vegzett a kutatassal, kuldi az eredmenyeket!\n");
		write(pipefd[1], elso, tmp);
		write(pipefd[2], masodik, tmp);
		write(pipefd[3], harmadik, tmp);

	} else {

		printf("Piros MLSZ keszen all es indit\n");

		struct kuldendo uzenet;
		int elhasznalt[3];
		for ( int i = 0; i < 3; ++i ) {
			timeToStop = 0;
			while (timeToStop == 0) {
				srand(time(NULL));
				int randnumber = rand() % 10;
				if (i == 0) {
					strcpy(uzenet.question[0], questionArray[randnumber]);
					strcpy(uzenet.answer[0], answerArray[randnumber]);
					strcpy(uzenet.answer[1], answerArray[randnumber+1]);
					strcpy(uzenet.answer[2], answerArray[randnumber+2]);
					strcpy(uzenet.answer[3], answerArray[randnumber+3]);
					elhasznalt[0] = randnumber;
					timeToStop = 1;
				} else if (i == 1 && randnumber != elhasznalt[0]) {
					strcpy(uzenet.question[1], questionArray[randnumber]);
					strcpy(uzenet.answer[4], answerArray[randnumber]);
					strcpy(uzenet.answer[5], answerArray[randnumber+1]);
					strcpy(uzenet.answer[6], answerArray[randnumber+2]);
					strcpy(uzenet.answer[7], answerArray[randnumber+3]);
					elhasznalt[1] = randnumber;
					timeToStop = 1;
				} else if (i == 2 && randnumber != elhasznalt[0] && randnumber != elhasznalt[1]) {
					strcpy(uzenet.question[2], questionArray[randnumber]);
					strcpy(uzenet.answer[8], answerArray[randnumber]);
					strcpy(uzenet.answer[9], answerArray[randnumber+1]);
					strcpy(uzenet.answer[10], answerArray[randnumber+2]);
					strcpy(uzenet.answer[11], answerArray[randnumber+3]);
					elhasznalt[2] = randnumber;
					timeToStop = 1;
				}
			}
		}

		write(pipefd[0], uzenet, 1500);
		printf("Piros MLSZ beirta az adatokat\n");
		fflush(NULL);
		wait()

		printf("Piros MLSZ olvas!\n");
		char s[40];
		read(pipefd[1], s, sizeof(s));
		printf("Elsore a valaszok: %s", s);
		read(pipefd[2], s, sizeof(s));
		printf("Masodikra a valaszok: %s", s);
		read(pipefd[3], s, sizeof(s));
		printf("Harmadikra a valaszok: %s", s);

		printf("Piros MLSZ befejezte!");

	}

	fclose(fileObj);
	return 0;
}

