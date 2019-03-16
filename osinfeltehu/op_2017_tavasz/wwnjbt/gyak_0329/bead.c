#include <stdio.h> 
#include <stdlib.h>	
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "bead.h"

/*Miután véglegesítettük a kérdéseket az MLSZ (szülõ) kérdezõbiztost(gyerek) küld a paraméterként megadott
labdajáték centrumba, városba.

Az MLSZ véletlenszerûen kiválaszt 3 kérdést, amit csõvezetéken kiküld a kérdezõbiztosnak azután, 
hogy az bejelentkezett az MLSZ-hez, hogy készen áll a feladat végrehajtására.
A kérdezõbiztos véletlenszerûen választ 10 és 20 közti konzultálni kívánó polgárt,
akik kitöltik a kérdõívet (véletlenszerûen). Az eredményt visszaküldi csõvezetéken az MLSZ-be, 
majd a kérdezõbiztos befejezi a konzultációt. Az MLSZ fogadja az eredményt és a képernyõre írja a konzultáció eredményét.

Készítsünk C programot, amellyel a leírt konzultáció elvégezhetõ.*/


void readString(char* string, unsigned int maxLength) {
	do {
		fgets(string, maxLength, stdin);
	} while (string[0] == '\n');

	char* lastChar = &string[strlen(string) - 1];

	if (*lastChar == '\n')
		*lastChar = '\0';
}
int	 readPositiveInt(unsigned int min, unsigned int max) {
	char buffer[64];
	unsigned int input = 0;
	int tries = 0;

	while (input == 0 || input > max || input < min) {
		if (tries > 0) {
			printf("Egy %i-%i kozotti egesz szamot varok!\n", min, max);
		}
		readString(buffer, sizeof(buffer));
		input = atoi(buffer);
		tries++;
	}

	return input;

}

void printMenu() {
	printf("\t\t[1] Kerdoiv megjelenitese\n");
	printf("\t\t[2] Uj kerdes hozzaadasa\n");
	printf("\t\t[3] Meglevo kerdesek es valaszok szerkesztese\n");
	printf("\t\t[4] Kerdes torlese\n");
	printf("\t\t[5] A kerdessor veglegesitese es mentese\n");
	printf("\t\t[6] Kilepes\n");
}

int  initializeSurvey(Survey* survey) {
	if (survey == NULL)
		return -1;

	survey->head = NULL;
	survey->size = 0;

	return 0;
};
void destroySurvey(Survey* survey) {
	SurveyNode* current = survey->head;
	while (current != NULL) {
		SurveyNode* next = current->next;
		free(current);
		current = next;
	}
	survey->head = NULL;
	survey->size = 0;
}

void printSurvey(Survey* survey) {
	SurveyNode* p = survey->head;
	int i;
	if (p == NULL)
		printf("A kerdoiv meg ures!\n");
	else {
		printf("[MLSZ nemzeti konzultacio]\n\n");
		int questionCount = 0;
		while (p != NULL) {
			questionCount++;
			printf("(%i) %s \t (%s)\n", questionCount, p->question, p->questionDate);
			for (i = 0; i < ANSWER_MAX_COUNT; i++) {
				if (p->answers[i][0] != '\0')
					printf("  %s\n", p->answers[i]);
			}
			printf("\n");
			p = p->next;
		}
	}


}
int  printNodeToScreen(SurveyNode* node) {
	if (node == NULL)
		return -1;

	printf("%s \t (%s)\n", node->question, node->questionDate);
	int i;
	for (i = 0; i < node->answerCount; i++) {
		printf("  %s\n", node->answers[i]);
	}
	printf("\n");

	return 0;
}

int  printNodeToFile(SurveyNode* node, FILE* fp) {
	if (fp == NULL)
		return -1;

	if (node == NULL)
		return -1;

	fprintf(fp, "%s \t (%s)\n", node->question, node->questionDate);
	int i;
	for (i = 0; i < node->answerCount; i++) {
		fprintf(fp, "  %s\n", node->answers[i]);
	}
	fprintf(fp, "\n");

	return 0;

}
int  finalizeAndSaveSurvey(Survey* survey) {
	printf("A kerdessor veglegesitese elkezdodott.\n");
	/*
	char path[20];
	printf("Adja meg a fajlnevet!\t");
	readString(path, sizeof(path));

	FILE* fp = fopen(path, "w");

	if (fp == NULL) {
		printf("Nem sikerult megnyitni a fajlt!\n");
		return -1;
	}

	fprintf(fp, "[MLSZ nemzeti konzultacio]\n\n");
	*/
	printf("Kerem valassza ki a vegleges kerdeseket!\n");
	SurveyNode* currentNode = survey->head;

	while (currentNode != NULL) {
		char keepQuestion[64];
		int questionNumber = 1;
		printf("Jelenlegi kerdes es annak valaszai:\n\n");
		printNodeToScreen(currentNode);
		printf("(i: kerdes megtartasa, minden mas: kerdes elvetese)\t");
		readString(keepQuestion, sizeof(keepQuestion));
		
		if (strcmp(keepQuestion, "i") != 0) {
			deleteQuestion(survey, questionNumber);
		}

		currentNode = currentNode->next;
		questionNumber++;
	}

	return 0;
}

SurveyNode* getLastNode(Survey* survey) {
	SurveyNode* p = survey->head;
	unsigned int i;
	for (i = 1; i < survey->size; i++) {
		p = p->next;
	}

	return p;
}

int  addQuestion(Survey* survey, char* question) {
	SurveyNode** p = &(survey->head);
	while (*p != NULL) {
		p = &((*p)->next);
	}
	*p = malloc(sizeof(SurveyNode));
	(*p)->next = NULL;
	(*p)->answerCount = 0;

	strcpy((*p)->question, question);

	int i;
	for (i = 0; i < ANSWER_MAX_COUNT; i++)
		strcpy((*p)->answers[i], "\0");

	updateQuestionDate(*p);
	survey->size++;
	return 0;
}
int  deleteQuestion(Survey* survey, unsigned int nodeNumber) {

	unsigned int lastnodeNumber = survey->size;
	if (nodeNumber > lastnodeNumber || nodeNumber == 0)
		return -1;

	if (nodeNumber == 1) {
		SurveyNode* p = survey->head->next;
		free(survey->head);
		survey->head = p;
	}
	else if (nodeNumber == lastnodeNumber) {
		SurveyNode** secondToLast = &(survey->head);

		while (((*secondToLast)->next)->next != NULL) {
			secondToLast = &((*secondToLast)->next);
		}
		free((*secondToLast)->next);
		(*secondToLast)->next = NULL;
	}
	else {
		SurveyNode* nodeBeforeDelete = survey->head;
		unsigned int i;
		for (i = 2; i < nodeNumber; i++) {
			nodeBeforeDelete = nodeBeforeDelete->next;
		}
		SurveyNode* nodeAfterDelete = nodeBeforeDelete->next->next;
		free(nodeBeforeDelete->next);
		nodeBeforeDelete->next = nodeAfterDelete;
	}
	survey->size--;
	return 0;
}
int  modifyQuestion(Survey* survey, unsigned int nodeNumber, char* newQuestion) {
	unsigned int lastnodeNumber = survey->size;
	if (nodeNumber > lastnodeNumber || nodeNumber == 0 || strlen(newQuestion) > QUESTION_MAX_LENGTH)
		return -1;

	SurveyNode* p = survey->head;
	unsigned int i;
	for (i = 1; i < nodeNumber; i++)
		p = p->next;

	if (strcpy(p->question, newQuestion) != NULL) {
		updateQuestionDate(p);
		return 0;
	}
	else
		return -1;
}
void updateQuestionDate(SurveyNode* p) {
	time_t timer;
	struct tm* currentTime;

	time(&timer);
	currentTime = localtime(&timer);

	strftime(p->questionDate, sizeof(p->questionDate), "%Y-%m-%d %H:%M:%S", currentTime);
}


int addAnswer(SurveyNode* node, char* answer) {
	if (node == NULL)
		return -1;
	if (node->answerCount >= ANSWER_MAX_COUNT) {
		fprintf(stderr, "Egy kerdeshez maximum csak %u valaszlehetoseg engedelyezett!\n", ANSWER_MAX_COUNT);
		return -1;
	}
	else {
		int i = 0;
		while (node->answers[i][0] != '\0') {
			i++;
		}
		strcpy(node->answers[i], answer);
		node->answerCount++;
		return 0;
	}
}
int deleteAnswer(SurveyNode* node, unsigned int ansNumber) {
	if (node == NULL)
		return -1;

	unsigned int ansCount = node->answerCount;

	if (ansCount <= ANSWER_MIN_COUNT) {
		fprintf(stderr, "Egy kerdeshez minimum %u valaszlehetoseg szukseges!\n", ANSWER_MIN_COUNT);
		return -1;
	}

	unsigned int i;
	for (i = ansNumber; i < ansCount; i++) {
		strcpy(node->answers[i - 1], node->answers[i]);
	}
	strcpy(node->answers[ansCount - 1], "\0");
	node->answerCount--;

	return 0;
}
int modifyAnswer(SurveyNode* node, unsigned int ansNumber, char* newAnswer) {

	if (strcpy(node->answers[ansNumber - 1], newAnswer) != NULL)
		return 0;
	else
		return -1;
}

int menuAddQuestionWithAnswers(Survey* survey) {
	printf("Mi legyen az uj kerdes szovege?\t");
	char qBuffer[QUESTION_MAX_LENGTH];

	readString(qBuffer, QUESTION_MAX_LENGTH);
	addQuestion(survey, qBuffer);
	printf("Kerdes hozzaadva!\n\n");
	printf("Hany valaszlehetoseget akar megadni(min %i, max %i)?\n", ANSWER_MIN_COUNT, ANSWER_MAX_COUNT);
	unsigned int answerCount;
	answerCount = readPositiveInt(ANSWER_MIN_COUNT, ANSWER_MAX_COUNT);

	SurveyNode* p = getLastNode(survey);
	unsigned int i;
	for (i = 0; i < answerCount; i++) {
		menuAddAnswer(p, i + 1);
	}

	return 0;
}
int menuEditQuestionAndAnswers(Survey* survey) {
	if (survey->head == NULL) {
		printf("A kerdoiv meg ures!\n");
		return -1;
	}
	printf("Hanyadik kerdest szeretne szerkeszteni?\t");
	int nodeNumber = readPositiveInt(1, survey->size);

	printf("Szeretne a kerdes szovegen valtoztatni? (i:igen, barmi mas:nem)\t");
	char changeQuestion[64];

	readString(changeQuestion, sizeof(changeQuestion));
	int returnVal = 0;

	if (strcmp(changeQuestion, "i") == 0)
		returnVal = menuModifyQuestion(survey, nodeNumber);
	else
		printf("\t%s\n", changeQuestion);
	if (returnVal == 0) {
		char editAnswers[64] = "i\0";

		while (strcmp(editAnswers, "i") == 0) {
			printf("Szeretne a valaszokon szerkeszteni? (i:igen, barmi mas: nem)\t");
			readString(editAnswers, sizeof(editAnswers));

			if (strcmp(editAnswers, "i") == 0)
				returnVal = menuEditAnswer(survey, nodeNumber);
		}
	}

	return returnVal;
}

int menuModifyQuestion(Survey* survey, unsigned int nodeNumber) {
	printf("Mire szeretne modositani a kerdest?\t");
	char buffer[QUESTION_MAX_LENGTH];

	readString(buffer, sizeof(buffer));
	return modifyQuestion(survey, nodeNumber, buffer);
}
int menuDeleteQuestion(Survey* survey) {
	if (survey->size == 0) {
		printf("A kerdoiv meg ures!\n");
		return 0;
	}
	printf("Hanyadik kerdest szeretne torolni?\t");
	char buffer[16];
	readString(buffer, sizeof(buffer));

	if (deleteQuestion(survey, atoi(buffer)) == 0)
		return 0;
	else {
		printf("A torles nem sikerult.\n");
		return -1;
	}
}

int menuAddAnswer(SurveyNode* node, unsigned int ansNumber) {
	printf("%i. valasz:\t", ansNumber);
	char buffer[ANSWER_MAX_LENGTH];
	readString(buffer, sizeof(buffer));
	return addAnswer(node, buffer);
}
int menuModifyAnswer(SurveyNode* node, unsigned int ansNumber) {
	printf("A valasz uj szovege: \t");
	char buffer[ANSWER_MAX_LENGTH];
	readString(buffer, sizeof(buffer));
	return modifyAnswer(node, ansNumber, buffer);
}
int menuEditAnswer(Survey* survey, unsigned int nodeNumber) {
	if (survey == NULL)
		return -1;

	SurveyNode* currentNode = survey->head;
	unsigned int i;
	for (i = 1; i < nodeNumber; i++)
		currentNode = currentNode->next;

	char newAnswer[64] = "i";

	while (currentNode->answerCount < ANSWER_MAX_COUNT && strcmp(newAnswer, "i") == 0) {
		printf("Szeretne uj valaszt hozzaadni? (i: igen, barmi mas: nem)\t\t");
		readString(newAnswer, sizeof(newAnswer));

		if (strcmp(newAnswer, "i") == 0)
			menuAddAnswer(currentNode, currentNode->answerCount + 1);
		printNodeToScreen(currentNode);
	}

	printf("(t: torles, m: modositas, barmi mas: szerkesztes befejezese )\t");
	char delOrModify[64];
	readString(delOrModify, sizeof(delOrModify));

	if ( strcmp(delOrModify, "t") != 0 && strcmp(delOrModify, "m") != 0)
		return 0;
	else {
		printf("Hanyadik valaszt szeretne ");
		(strcmp(delOrModify, "t") == 0) ? (printf("torolni? ")) : (printf("modositani? "));
		printf("\n(jelenleg %i  valasz van a kerdesre)\t", currentNode->answerCount);

		int ansNumber = readPositiveInt(1, currentNode->answerCount);

		if (strcmp(delOrModify, "t") == 0)
			return deleteAnswer(currentNode, ansNumber);
		else
			return menuModifyAnswer(currentNode, ansNumber);
	}
}

int shrinkTo3Questions(Survey* survey) {
	srand(time(NULL));
	while (survey->size > 3) {
		int randomIndex = rand() % survey->size + 1;
		deleteQuestion(survey, randomIndex);
	}
	return 0;
}

void handler(int signumber){
	printf("A kerdezobizottsag keszen all.\n");
}

SurveyNode* makeSurveyIntoArray(Survey* survey) {
	static SurveyNode questionArray[3];
	SurveyNode* p = survey->head;
	int i;
	
	for (i = 0; i < 3; i++) {
		questionArray[i] = *p;
		p = p->next;
	}
	
	return questionArray;
}

int startVotingProcess(Survey* survey) {
	if (survey->size < 3)
		return -1;
	
	
	signal(SIGUSR1, handler);
	int pipefd[2];
	SurveyNode* questionsToChild;
	SurveyNode questionsFromParent[3];
	int voteResultsFromChild[3][ANSWER_MAX_COUNT];
	pid_t  child;
	
	if (pipe(pipefd) == -1) {
		   perror("Hiba a pipe nyitaskor!");
		   exit(EXIT_FAILURE);
	}
	
	child = fork();
	
	if (child < 0) {
		perror("Nem sikerult forkolni!\n");
		exit(1);
	}
	if (child > 0) {
		pause();
		shrinkTo3Questions(survey);
		questionsToChild = makeSurveyIntoArray(survey);
		
		write(pipefd[1], questionsToChild, 3 * sizeof(SurveyNode));
		wait(-1);
		
		read(pipefd[0], voteResultsFromChild, sizeof(voteResultsFromChild));
	
		int i,j;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < ANSWER_MAX_COUNT; j++) {
				printf("%i ", voteResultsFromChild[i][j]);
			}
			printf("\n");
		}
	}
	else {
		kill(getppid(), SIGUSR1);
		sleep(3);
		read(pipefd[0], questionsFromParent, sizeof(questionsFromParent));
		
		int voterCount = (rand() % 11) + 10; //10-20 között
		
		int voteResultMatrix[3][ANSWER_MAX_COUNT];
		int i, j;
		
		for (i = 0; i < 3; i++)
			for (j = 0; j < ANSWER_MAX_COUNT; j++)
				voteResultMatrix[i][j] = 0;
		
		for (i = 0; i < 3; i++) {
			for (j = 0; j < voterCount; j++) {
				int votedAnswer = rand() % questionsFromParent[i].answerCount;
				voteResultMatrix[i][votedAnswer]++;
			}
		}
		
		write(pipefd[1], voteResultMatrix, sizeof(voteResultMatrix));
	}
	return 0;
}

int main() {
	Survey footballSurvey;
	Survey* pFootballSurvey = &footballSurvey;
	initializeSurvey(pFootballSurvey);
	int running = 1;

	while (running) {
		printMenu();
		int menuOption = readPositiveInt(1, 6);

		switch (menuOption) {
		case 1:
			printSurvey(pFootballSurvey);
			break;
		case 2:
			menuAddQuestionWithAnswers(pFootballSurvey);			
			break;
		case 3:
			if (pFootballSurvey->head != NULL)
				printSurvey(pFootballSurvey);
			menuEditQuestionAndAnswers(pFootballSurvey);
			break;
		case 4:
			if (pFootballSurvey->head != NULL)
				printSurvey(pFootballSurvey);
			menuDeleteQuestion(pFootballSurvey);
			break;
		case 5:
			finalizeAndSaveSurvey(pFootballSurvey);
			break;
		case 6:
			running = 0;
			break;
		default:
			printf("Ilyen menuopcio nincs!\n");
			break;
		}
	}
	startVotingProcess(pFootballSurvey);
	
	destroySurvey(pFootballSurvey);

	printf("Vege a programnak!\n");
	return 0;
}