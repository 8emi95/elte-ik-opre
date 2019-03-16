#include "question_creation.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

void printMenu()
{
	printf("Mit szeretne tenni?\n\n");
	printf("1: Uj kerdes felvetele\n");
	printf("2: Kerdes modositasa\n");
	printf("3: Kerdes torlese\n");
	printf("4: Kerdesek listazasa\n");
	printf("5: Kerdoiv veglegesitese\n\n");
}

void addQuestion(struct Question *questions, int numberOfQuestions)
{	
	printf("Az uj kerdes (max. 80 karakter):  ");
	gets(questions[numberOfQuestions].text);
	
	printf("Valaszok szama (2-4):  ");
	char c = getchar(); getchar();
	int numberOfAnswers = atoi(&c);
	questions[numberOfQuestions].answers = (char**)malloc(sizeof(char) * numberOfAnswers);
	int i;
	for (i=0;i<numberOfAnswers;++i)
	{
		questions[numberOfQuestions].answers[i] = (char*)malloc(sizeof(char) * 40);
		int k = i + 1;
		printf("%d. valasz: ",k);
		gets(questions[numberOfQuestions].answers[i]);
	}
	questions[numberOfQuestions].numberOfAnswers = numberOfAnswers;
	time_t currentTime;
	currentTime = time(NULL);
	questions[numberOfQuestions].timeOfCreation = ctime(&currentTime);
	
	printf("\n");
}

void listQuestions(struct Question *questions, int numberOfQuestions)
{
	if (numberOfQuestions == 0)
	{
		printf("Nincsenek megadva kerdesek\n\n");
		return;
	}
	
	int i;
	for (i=0;i<numberOfQuestions;++i)
	{
		int k = i + 1;
		printf("%d. kerdes: %s \n",k,questions[i].text);
		printf("Valaszok: ");
		int j;
		for (j=0;j<questions[i].numberOfAnswers;++j)
		{
			printf("%s     ",questions[i].answers[j]);
		}
		printf("\nA kerdes letrehozva/utoljara modositva: %s",questions[i].timeOfCreation);
		printf("\n\n");
	}
}

void modifyQuestion(struct Question *questions, int numberOfQuestions)
{
	if (numberOfQuestions == 0)
	{
		printf("Nincsenek megadva kerdesek\n\n");
		return;
	}
	
	listQuestions(questions, numberOfQuestions);
	printf("A modositando kerdes sorszama: ");
	char c = getchar(); getchar();
	int n = atoi(&c);
	
	//Az n-edik (a tömbben n-1 -edik) kérdés és válaszainak kiírása
	printf("Az %d. kerdes: %s\n",n,questions[n-1].text);
	printf("Valaszai: ");
	int i;
	for (i=0;i<questions[n-1].numberOfAnswers;++i)
	{
		printf("%s     ",questions[n-1].answers[i]);
	}
	printf("\n\n");
	
	//Módosítások bekérése
	printf("Szeretne a kerdes szoveget modositani? (y/n) ");
	c = getchar(); getchar();
	if (c == 'y')
	{
		printf("A kerdes uj szovege: ");
		gets(questions[n-1].text);
		time_t currentTime;
		currentTime = time(NULL);
		questions[n-1].timeOfCreation = ctime(&currentTime);
	}
	printf("\n");
	
	printf("Szeretne (teljesen) uj valaszokat megadni? (y/n) ");
	c = getchar(); getchar();
	if (c == 'y')
	{
		//Memóriafelszabadítás
		for (i=0;i<questions[n-1].numberOfAnswers;++i)
		{
			free(questions[n-1].answers[i]);
		}
		free(questions[n-1].answers);
		
		//Új válaszok bekérése, tárolása
		printf("Valaszok szama (2-4):  ");
		c = getchar(); getchar();
		int numberOfAnswers = atoi(&c);
		
		questions[n-1].answers = (char**)malloc(sizeof(char) * numberOfAnswers);
		for (i=0;i<numberOfAnswers;++i)
		{
			questions[n-1].answers[i] = (char*)malloc(sizeof(char) * 40);
			int k = i + 1;
			printf("%d. valasz: ",k);
			gets(questions[n-1].answers[i]);
		}
		questions[n-1].numberOfAnswers = numberOfAnswers;
		time_t currentTime;
		currentTime = time(NULL);
		questions[n-1].timeOfCreation = ctime(&currentTime);
	}
	printf("\n\n");
}

void deleteQuestion(struct Question *questions, int numberOfQuestions)
{
	if (numberOfQuestions == 0)
	{
		printf("Nincsenek megadva kerdesek\n\n");
		return;
	}
	
	listQuestions(questions, numberOfQuestions);
	printf("A torlendo kerdes sorszama: ");
	char c = getchar(); getchar();
	int n = atoi(&c);
	printf("\n");
	
	//A törlendő kérdés dinamikusan lefoglalt részeinek felszabadítása
	int i;
	for (i=0;i<questions[n-1].numberOfAnswers;++i)
	{
		free(questions[n-1].answers[i]);
	}
	free(questions[n-1].answers);
	//free(questions[n-1].timeOfCreation);		//Törléskor hibát oko(hat)
	
	//A torlendo kerdes utani minden kerdes eggyel elorebb mozgatasa
	for (i=n-1;i<numberOfQuestions-1;++i)
	{
		questions[i] = questions[i+1];
	}
}
