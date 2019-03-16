// készítette: Nyitrai Gergő
// neptun: MBFRU7
// email: nyitrai@protonmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#define FILENAME "savefile.bin"
#define BUFFSIZE 256

struct question {
	char text[BUFFSIZE];
	int numOfAnsw;
	char answers[4][BUFFSIZE];
	unsigned int isFinal;
	time_t created;
};

void menu ();
void newQuestion ();
void editQuestion ();
void deleteQuestion ();
void finalizeQuestion ();
void unFinelizeQuestion ();
void finalizeSurvey ();
int isNumeric (const char*);
int toInt (const char*);
int getNumBetween (const int, const int, const char*);
struct question* createQuestion ();
void listQuestions (const unsigned int, const unsigned int);
void readQuestions (struct question*, int*);
void writeQuestions (struct question*, const int, const int);
char* displayTime (const time_t);
int* genDistRandNumsBetween (const int, const int, const int);
int contains (const int*, const int, const int);
void handler (int signum) {}

static const char* BallGameCentre;
static const char* BallGameCity;

int main(int argc, const char* argv[]) {
	if (argc == 3) {
		BallGameCentre = argv[1];
		BallGameCity = argv[2];
		menu();
	} else {
		printf("   A programot az alábbi két paraméterrel futtassa: labdajáték centrum, város\n");
	}
	return 0;
}

void menu () {
	int selection = 0;
	do {
		printf("\n");
		printf("+-------------------------------------------------------------+\n");
		printf("|                  MLSZ Kérdőív Készítő v2.0                  |\n");
		printf("+-------------------------------------------------------------+\n");
		printf("|  [1] Új kérdés felvétele                                    |\n");
		printf("|  [2] Kérdés módosítása                                      |\n");
		printf("|  [3] Kérdés törlése                                         |\n");
		printf("|  [4] Kérdés véglegesítése                                   |\n");
		printf("|  [5] Véglegesítés törlése                                   |\n");
		printf("|  [6] Végleges kérdések listázása                            |\n");
		printf("|  [7] Összes kérdés listázása                                |\n");
		printf("|  [8] Kérdőív véglegesítése                                  |\n");
		printf("|  [9] Kilepes                                                |\n");
		printf("+-------------------------------------------------------------+\n");
		printf("\n");
		selection = getNumBetween(1, 9, "   Kérem válasszon menüpontot: ");
		switch (selection) {
			case 1 : printf("\n"); newQuestion(); break;
			case 2 : printf("\n"); editQuestion(); break;
			case 3 : printf("\n"); deleteQuestion(); break;
			case 4 : printf("\n"); finalizeQuestion(); break;
			case 5 : printf("\n"); unFinelizeQuestion(); break;
			case 6 : printf("\n"); listQuestions(1, 1); break;
			case 7 : printf("\n"); listQuestions(0, 1); break;
			case 8 : printf("\n"); finalizeSurvey(); break;
			case 9 : printf("\n"); printf("   Viszont látásra!\n"); break;
			default : break;
		}
	} while (selection != 9);
}

void newQuestion () {
	struct question* q = createQuestion();
	FILE* file = fopen (FILENAME, "ab");
	if (file == NULL) {
		printf("   Hiba történt a fájl megnyitása közben!\n");
		exit(1);
	}
	fwrite(q, sizeof(struct question), 1, file);
	fclose (file);
}

void editQuestion () {
	struct question qs[BUFFSIZE];
	int n, selected, isFinal;
	readQuestions(qs, &n);
	listQuestions(0, 1);
	printf("\n");
	selected = getNumBetween(1, n, "   Módosítandó kérdés sorszáma: ");
	isFinal = qs[selected - 1].isFinal;
	qs[selected - 1] = *createQuestion();
	qs[selected - 1].isFinal = isFinal;
	writeQuestions(qs, n, 0);
}

void deleteQuestion () {
	struct question qs[BUFFSIZE];
	int n, selected;
	readQuestions(qs, &n);
	listQuestions(0, 1);
	printf("\n");
	selected = getNumBetween(1, n, "   Törlendő kérdés sorszáma: ");
	writeQuestions(qs, n, selected);
}

void finalizeQuestion () {
	struct question qs[BUFFSIZE];
	int n, selected;
	readQuestions(qs, &n);
	listQuestions(2, 1);
	printf("\n");
	selected = getNumBetween(1, n, "   Véglegesítendő kérdés sorszáma: ");
	if (qs[selected - 1].isFinal)
		printf("   A kiválasztott kérdés már véglegesítve volt!\n");
	else {
		qs[selected - 1].isFinal = 1;
		writeQuestions(qs, n, 0);
	}
}

void unFinelizeQuestion () {
	struct question qs[BUFFSIZE];
	int n, selected;
	readQuestions(qs, &n);
	listQuestions(1, 1);
	printf("\n");
	selected = getNumBetween(1, n, "   Visszavonandó kérdés sorszáma: ");
	if (qs[selected - 1].isFinal) {
		qs[selected - 1].isFinal = 0;
		writeQuestions(qs, n, 0);
	} else
		printf("   A kiválasztott kérdés nem volt végleges!\n");
}

void finalizeSurvey () {
	int status, qSelected = 1, pipefd[2];
	if (pipe(pipefd) == -1) {
		printf("   Hiba a csővezeték nyitásakor!\n");
		exit(1);
	}
	signal(SIGTERM, handler);
	pid_t child = fork();
	if (child < 0) {
		printf("   Hiba a Kérdezőbiztos kiküldése közben! (pipe)\n");
		exit(1);
	} else if (child > 0) { //parent
		struct question qs[BUFFSIZE];
		int i, j, l, n, m = 0, finalQIndex[BUFFSIZE], *selectedQIndex, k, result[3][20];
		pause();
		printf("   MLSZ: Kérdések kiválasztása ...\n");
		sleep(1);
		readQuestions(qs, &n);
		for (i = 0; i < n; i++)
			if (qs[i].isFinal)
				finalQIndex[m++] = i;
		if (m < 3) {
			printf("   MLSZ: Hiba! Nincs minimum 3 végleges kérdés!\n");
			qSelected = 0;
			kill(child, SIGTERM);
		} else {
			selectedQIndex = genDistRandNumsBetween(0, m - 1, 3);
			printf("   MLSZ: kiválasztott kérdések sorszáma: %d, %d, %d.\n", finalQIndex[selectedQIndex[0]] + 1, finalQIndex[selectedQIndex[1]] + 1, finalQIndex[selectedQIndex[2]] + 1);
			sleep(1);			
			printf("   MLSZ: Kérdések küldése ...\n");
			sleep(1);
			for (i = 0; i < 3; i++)
				write(pipefd[1], &qs[finalQIndex[selectedQIndex[i]]], sizeof(struct question));
			close(pipefd[1]);
			fflush(NULL);
			kill(child, SIGTERM);
			pause();
			printf("   MLSZ: Konzultáció eredményének fogadása ...\n");
			sleep(1);
			read(pipefd[0], &k, sizeof(int));
			read(pipefd[0], &result, 3 * 20 * sizeof(int));
			close(pipefd[0]);
			printf("   MLSZ: A konzultáció eredménye:\n");
			printf("\n");
			for (i = 0; i < 3; i++) {
				int temp[4] = {0, 0, 0, 0};
				for (j = 0; j < k; j++)
					temp[result[i][j]]++;
				printf("   %d. %s (felvéve: %s)\n", finalQIndex[selectedQIndex[i]] + 1, qs[finalQIndex[selectedQIndex[i]]].text, displayTime(qs[finalQIndex[selectedQIndex[i]]].created));
				for (j = 0; j < qs[finalQIndex[selectedQIndex[i]]].numOfAnsw; j++)
					printf("      %c) %s      - %d db\n", 'a' + i, qs[finalQIndex[selectedQIndex[i]]].answers[j], temp[j]);
			}
		}
		waitpid(child, &status, 0);
	} else { //child
		struct question* qs = malloc(3 * sizeof(struct question));
		int i, j, k, result[3][20];
		printf("   Kérdezőbiztos: Bejelentkeztem innen: %s centrum, %s város!\n", BallGameCentre, BallGameCity);
		sleep(1);
		kill(getppid(), SIGTERM);
		pause();
		if (!qSelected)
			printf("   Kérdezőbiztos: Nem kaptam meg a kérdéseket!\n");
		else {
			printf("   Kérdezőbiztos: Kérdések olvasása ...\n");
			sleep(1);
			for (i = 0; i < 3; i++)
				read(pipefd[0], &qs[i], sizeof(struct question));
			close(pipefd[0]);
			srand(time(NULL));
			k = 10 + rand() % 11;
			printf("   Kérdezőbiztos: konzultáció kezdése %d emberrel ...\n", k);
			sleep(1);
			for (i = 0; i < 3; i++)
				for (j = 0; j < k; j++)
					result[i][j] = rand() % qs[i].numOfAnsw;
			printf("   Kérdezőbiztos: konzultáció eredményének küldése ...\n");
			sleep(1);
			write(pipefd[1], &k, sizeof(int));
			write(pipefd[1], &result, 3 * 20 * sizeof(int));
			close(pipefd[1]);
			fflush(NULL);
			printf("   Kérdezőbiztos: Konzultáció befejezése ...\n");
			sleep(1);
			kill(getppid(), SIGTERM);
		}
		exit(0);
	}
}

int isNumeric (const char* string) {
	int result = 1;
	while (*string && result) {
		char c = *string++;
		if (c < '0' || c > '9')
			result = 0;
	}
	return result;
}

int toInt (const char* string) {
	int result = 0;
	while (*string) {
		result *= 10;
		result += *string++ - '0';
	}
	return result;
}

int getNumBetween (const int min, const int max, const char* msg) {
	char input[BUFFSIZE];
	int num = min - 1;
	do {
		printf("%s", msg);
		scanf(" %s", input);
		if (isNumeric(input))
			num = toInt(input);
		else
			num = min - 1;
		if (num < min || num > max)
			printf("     Hiba! %d és %d közötti értéket adjon meg!\n", min, max);
	} while (num < min || num > max);
	return num;
}

struct question* createQuestion () {
	struct question* q = malloc(sizeof(struct question));
	char input[BUFFSIZE];
	int i;
	printf("   A kérdés szövege: ");
	scanf (" %[^\n]s", input);
	strcpy(q -> text, input);
	q -> numOfAnsw = getNumBetween(2, 4, "   Válaszok száma : ");
	for (i = 0; i < q -> numOfAnsw; i++) {
		printf("   %d. válasz: ", i + 1);
		scanf (" %[^\n]s", input);
		strcpy(q -> answers[i], input);
	}
	q -> isFinal = 0;
	q -> created = time(NULL);
	return q;
}

void listQuestions (const unsigned int filter, const unsigned int showAnswers) {
	struct question* q = malloc(sizeof(struct question));
	FILE* file = fopen (FILENAME, "rb");
	int n = 1;
	if (file == NULL) {
		printf("   Hiba történt a fájl megnyitása közben!\n");
		exit(1);
	}
	while (fread(q, sizeof(struct question), 1, file)) {
		if (!filter || (filter == 1 && q -> isFinal) || (filter == 2 && !(q -> isFinal))) {
			printf("   %d. %s (felvéve: %s)\n", n, q -> text, displayTime(q -> created));
			if (showAnswers) {
				int i;
				for (i = 0; i < q -> numOfAnsw; i++)
					printf("      %c) %s\n", 'a' + i, q -> answers[i]);
			}
		}
		n++;
	}
	fclose(file);
}

void readQuestions (struct question* qs, int* size) {
	int n = 0;
	FILE* file = fopen (FILENAME, "rb");
	if (file == NULL) {
		printf("   Hiba történt a fájl megnyitása közben!\n");
		exit(1);
	}
	while (fread(&qs[n++], sizeof(struct question), 1, file)) {}
	fclose(file);
	(*size) = n - 1;
}

void writeQuestions (struct question* qs, const int size, const int deleteThis) {
	int i;
	FILE* file = fopen (FILENAME, "wb");
	for (i = 0; i < size; i++)
		if (i != deleteThis - 1)
			fwrite(&qs[i], sizeof(struct question), 1, file);
	fclose(file);
}

char* displayTime (const time_t raw) {
	char* result = malloc(81 * sizeof(char));
	struct tm* info = localtime(&raw);
	strftime(result, 80, "%Y-%m-%d %H:%M:%S", info);
	return result;
}

int* genDistRandNumsBetween (const int min, const int max, const int amount) {
	int i, j, r, *result = malloc(amount * sizeof(int));
	srand(time(NULL));
	for (i = 0; i < amount; i++)
		result[i] = min - 1;
	for (i = 0; i < amount; i++) {
		r = min + rand() % ((max - min) + 1);
		while (contains(result, i + 1, r))
			r = min + rand() % ((max - min) + 1);
		result[i] = r;
	}
	return result;
}

int contains (const int* arr, const int size, const int value) {
	int i = 0, result = 0;
	while (i < size && !result)
		if (arr[i++] == value)
			result = 1;
	return result;
}
