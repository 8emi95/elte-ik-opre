#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

struct Question
{
	char text[80];
	int numberOfAnswers;
	char** answers;
	char* timeOfCreation;
};

void printMenu()
{
	printf("Mit szeretne tenni?\n\n");
	printf("1: Uj kerdes felvetele\n");
	printf("2: Kerdes modositasa\n");
	printf("3: Kerdes torlese\n");
	printf("4: Kerdesek listazasa\n");
	printf("5: Kerdoiv veglegesitese es kilepes\n\n");
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
	
	//A törlendõ kérdés dinamikusan lefoglalt részeinek felszabadítása
	int i;
	for (i=0;i<questions[n-1].numberOfAnswers;++i)
	{
		free(questions[n-1].answers[i]);
	}
	free(questions[n-1].answers);
	free(questions[n-1].timeOfCreation);
	
	//A torlendo kerdes utani minden kerdes eggyel elorebb mozgatasa
	for (i=n-1;i<numberOfQuestions-1;++i)
	{
		questions[i] = questions[i+1];
	}
}

void saveToFile(struct Question *questions, int numberOfQuestions)
{
	FILE* file;
	file = fopen("kerdoiv.txt","w");
	if (file == NULL)
	{
		perror("Fajl letrehozasi hiba");
		exit(1);
	}
	int i;
	for (i=0;i<numberOfQuestions;++i)
	{
		fprintf(file,"%s\n",questions[i].text);
		int j;
		for (j=0;j<questions[i].numberOfAnswers;++j)
		{
			fprintf(file,"%s ",questions[i].answers[j]);
		}
		fprintf(file,"\n");
		fprintf(file,"%s\n",questions[i].timeOfCreation);
	}
	fclose(file);
}

int main()
{
	printf("\n*********************** Kerdoiv osszeallito program ***********************\n\n");
	
	struct Question questions[100];
	int numberOfQuestions = 0;
	
	int stayInCycle = 1;
	while (stayInCycle)
	{
		printMenu();
		char c;
		c = getchar(); getchar();
		printf("\n\n");
		switch (c)
		{
			case '1': addQuestion(questions,numberOfQuestions); numberOfQuestions++; break;
			case '2': modifyQuestion(questions, numberOfQuestions); break;
			case '3': deleteQuestion(questions,numberOfQuestions); numberOfQuestions--; break;
			case '4': listQuestions(questions,numberOfQuestions); break;
			case '5': stayInCycle = 0; saveToFile(questions,numberOfQuestions); break;
			default: printf("A fenti opciok valamelyiket kell valasztania!\n\n"); break;
		}
		//while ((c = getchar()) != '\n') { }			//Több mint egy karakter megadása esetén a fölöslegesek kiürítése
		
	}
	return 0;
}