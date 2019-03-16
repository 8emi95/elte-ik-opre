#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct questions
{
	char question[160];
	char answers[4][100];
	int answerNumber;
	char creationDate[100];
} questions;

typedef struct surveyTaker
{
	char answer[3][100];
} surveyTaker;

struct questions qs[100];
int questionNumber = 0;

void newLineToNull(char *line)
{
	char *pos;
	if ((pos = strchr(line, '\n')) != NULL)
	{
		*pos = '\0';
	}
}

void readFromFile()
{
	FILE *f;
	f = fopen("kerdesek.txt", "r");
	if (f == NULL)
	{
		f = fopen("kerdesek.txt", "a");
		return;
	}
	char line[200];
	while (fgets(line, sizeof(line), f))
	{
		newLineToNull(line);
		strcpy(qs[questionNumber].question, line);
		fgets(line, sizeof(line), f);
		newLineToNull(line);
		qs[questionNumber].answerNumber = atoi(line);
		int i;
		for (i = 0; i < qs[questionNumber].answerNumber; ++i)
		{
			fgets(line, sizeof(line), f);
			newLineToNull(line);
			strcpy(qs[questionNumber].answers[i], line);
		}
		fgets(line, sizeof(line), f);
		newLineToNull(line);
		strcpy(qs[questionNumber].creationDate, line);
		questionNumber++;
	}
	fclose(f);
}

bool isvalueinarray(int val, int *arr, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (arr[i] == val)
			return true;
	}
	return false;
}

void handler(int signumber)
{
	printf("Signal with number %i has arrived\n", signumber);
}

int main(int argc, char *argv[])
{
	readFromFile();
	srand(time(NULL));
	int status;
	int parentToChild[2]; // unnamed pipe file descriptor array
	int childToParent[2];
	pid_t pid;

	if (pipe(parentToChild) == -1)
	{
		perror("Hiba a parentToChild pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	if (pipe(childToParent) == -1)
	{
		perror("Hiba a childToParent pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	pid = fork(); // creating parent-child processes
	if (pid == -1)
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	if (pid != 0)
	{ // parent process
		close(parentToChild[0]);
		close(childToParent[1]);
		int questionIds[3] = {-1, -1, -1};
		questions *sqs = malloc(3 * sizeof(questions));
		int i;
		int tmp;
		do
		{
			for (i = 0; i < 3; ++i)
			{

				questionIds[i] = rand() % (questionNumber);
				printf("%d", questionIds[i]);
				sqs[i] = qs[questionIds[i]];
			}
		}while (questionIds[0] == questionIds[1] || questionIds[1] == questionIds[2] || questionIds[0] == questionIds[2]);
		printf("Kérdőív elkészítve. A kitöltendő kérdőív a következő:\n");
		for (i = 0; i < 3; ++i)
		{
			printf("%d. Kérdés: %s\n", i+1, sqs[i].question);
			int j;
			for (j = 0; j < sqs[i].answerNumber; ++j)
			{
				printf("%d. Válasz: %s\n", j+1, sqs[i].answers[j]);
			}
		}
		printf("MLSZ elkezdi kuldeni a csobe az adatokat!\n");
		write(parentToChild[1], sqs, 3 * sizeof(questions));

		int peopleNum = 0;
		printf("MLSZ olvassa az adatokat a csőböl!\n");
		read(childToParent[0], &peopleNum, sizeof(peopleNum));
		surveyTaker *sTakers = malloc(peopleNum * sizeof(surveyTaker));
		read(childToParent[0], sTakers, peopleNum * sizeof(surveyTaker));

		printf("Az kérdőív kitöltésének az eredményei megérkeztek! A kérdőívet %d ember töltötte ki! Az eredmények a következők:\n", peopleNum);
		for (i = 0; i < peopleNum; ++i)
		{
			printf("%d. kitöltő válaszai:\n", i+1);
			int j;
			for (j = 0; j < 3; ++j)
			{
				printf("%d. Kérdés: %s\n", j+1, sqs[j].question);
				printf("Válasz: %s\n", sTakers[i].answer[j]);
			}
		}

		close(parentToChild[1]);
		close(childToParent[0]);
		waitpid(pid, &status, 0);
	}
	else
	{ // child process
		printf("K�ld�tt indul!\n");
		questions *sqs = malloc(3 * sizeof(questions));
		close(parentToChild[1]);
		close(childToParent[0]);
		printf("K�ld�tt kiolvassa az adatokat a csobol!\n");
		read(parentToChild[0], sqs, 3 * sizeof(questions));
		printf("Küldött megkapta a kérdéseket!\n");
		int peopleNum = rand() % (11) + 10;
		int i;
		surveyTaker *sTakers = malloc(peopleNum * sizeof(surveyTaker));
		for (i = 0; i < peopleNum; ++i)
		{
			int j;
			for (j = 0; j < 3; ++j)
			{
				int randAnswer = rand() % (sqs[j].answerNumber);
				strcpy(sTakers[i].answer[j], sqs[j].answers[randAnswer]);
			}
		}
		printf("Küldött kitöltette %d emberrel a kérdőívet!\n", peopleNum);
		printf("Küldött visszaküldi az MLSZ-nek az adatokat\n");
		write(childToParent[1], &peopleNum, sizeof(peopleNum));
		write(childToParent[1], sTakers, peopleNum * sizeof(surveyTaker));
		close(childToParent[1]);
		close(parentToChild[0]);
		printf("Küldött befejezte a munkáját.\n");
		exit(0);
	}
}
