#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

struct Question
{
	char text[80];
	unsigned int answNum;
	char answers[4][80];
	time_t createDate;
};

const char fname[] = "cons";
const char tempfile[] = "cons.tmp";
const char ffname[] = "fcons";
bool waiting = true;

int GetNumber(char* str, int mini, int maxi)
{
	int n;
	char *p;
	char num[10];
	do
	{
		printf("Legalabb %d maximum %d. %s", mini, maxi, str);
		fgets(num, sizeof(num),stdin);
		n = strtol(num, &p, 10);
	}while(num[0] == '\n' || (*p != '\n' && *p != '\0') || n > maxi || n < mini);
	
	return n;
}

void AddQuestion()
{
	struct Question qest;
	printf("Kerdes: ");
	  
	fgets(qest.text, sizeof(qest.text), stdin);
	qest.answNum = GetNumber("Valaszok szama: ", 2, 4);
	
	unsigned int i=0;
	for(; i<qest.answNum; i++)
	{
		printf("\n%d. Valasz: ", i+1);
		fgets(qest.answers[i], sizeof(qest.answers[i]), stdin);
	}
	
	qest.createDate = time(NULL);
	printf("\nKeredes letrehozasanak idopontja: %s", ctime(&qest.createDate));
	
	int f = open(fname, O_CREAT | O_WRONLY | O_APPEND, S_IWUSR | S_IRUSR);
	write(f, &qest, sizeof(struct Question));
	close(f);

	return;
}

void DisplayQuest(struct Question qest)
{
	printf("Szovege: %s", qest.text);
	int i;
	for (i = 0; i < qest.answNum; i++)
		printf("%d. valasz: %s", i+1, qest.answers[i]);
	printf("Letrehozva: %s\n", ctime(&qest.createDate));

	return;
}

int CountQuestions()
{
	struct Question qest;
	int f = open(ffname, O_RDONLY);
	
	if (f < 0)
		return 0;

	int i=0;
	while (read(f, &qest, sizeof(struct Question)))
		i++;
	close(f);

	return i;
}

int GetList()
{
	printf("--------\n");
	struct Question qest;
	int f = open(fname, O_RDONLY);
	
	if (f < 0)
	{
		printf("Nem letezik a fajl meg. Adjon hozza kerdest.\n");
		return -1;
	}
	
	int i=0;
	while (read(f, &qest, sizeof(struct Question)))
	{
		printf("%d. Kerdes:\n", i+1);
		DisplayQuest(qest);
		printf("\n");
		i++;
	}
	close(f);

	return i;
}

void EditQuestion()
{
	int num = GetList();
	if (num == 0)
	{
		printf("Nincsenek kerdesek meg!\n");
		return;
	}
	
	int f = open(fname, O_RDWR); 
	if (num == 0 || f < 0)
		return;

	printf("Adja meg a modositando kerdest?\n");
	int qind = GetNumber("Sorszam ", 1, num);
	lseek(f, (qind-1)*sizeof(struct Question), SEEK_SET);
	
	struct Question qest;
	read(f, &qest, sizeof(struct Question));
	printf("Kerdes:\n");
	DisplayQuest(qest);
	printf("WMit szeretne modositani?\n"
		"1 - Kerdes\n"
		"2 - Valaszok\n");
	int option = GetNumber("Valasztas: ", 1, 2);
	
	switch(option)
	{
		case 1:
			printf("Az kerdes: \n");
			fgets(qest.text, sizeof(qest.text), stdin);
			break;

		case 2:
			printf("1 - Uj valasz megadasa\n");
			printf("2 - Valasz torlese\n");
			int j;
			for (j=0; j<qest.answNum; j++)
				printf("%d - Modositas %d. valaszt\n", 3+j, j+1);
			option = GetNumber("Valasztas: ", 1, 2+qest.answNum);
			
			switch (option)
			{
				case 1:
					if (qest.answNum > 3)
						printf("Mar maximalis 4 valaszlehetoseg van!\n");
					else
					{
						printf("Az uj valasz: \n");
						fgets(qest.answers[qest.answNum], 200, stdin);
						qest.answNum++;
					}
					break;

				case 2:
					if (qest.answNum < 3)
						printf("Csak 2 valaszlehetoseg adott!\n");
					else
					{
						int delans = GetNumber("Torolni kivant valasz: ", 1, qest.answNum);
						memcpy(qest.answers[delans-1], qest.answers[qest.answNum-1], 200);
						qest.answNum--;
					}
					break;

				default:
					printf("Uj valasz: \n");
					fgets(qest.answers[option-3], sizeof(qest.answers[option-2]), stdin);
					break;
			}
			break;
	}
	lseek(f, (qind-1)*sizeof(struct Question), SEEK_SET);
	write(f, &qest, sizeof(struct Question));
	close(f);
	
	return;
}

void DeleteQuestion()
{
	int qnum = GetList();
	if (qnum == 0)
	{
		printf("Nincsenek kerdesek!\n");
		return;
	}
	
	int f = open(fname, O_RDWR); 
	if (f < 0)
		return;
	
	int f2 = open(tempfile, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	printf("Melyik kerdest szeretne torolni?\n");
	int delq = GetNumber("Index: ", 1, qnum);
	struct Question qest;
	int i = 1;
	while (read(f, &qest, sizeof(struct Question)))
	{
		if (i != delq)
		  write(f2, &qest, sizeof(struct Question));
		i++;
	}
  
	close(f2);
	close(f);
	rename(tempfile, fname);
}

void Ending()
{
	int qnum = GetList();
	int f = open(fname, O_RDWR); 
	if (f < 0)
		return;

	int f2 = open(ffname, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	struct Question qest;
  
	while (read(f, &qest, sizeof(struct Question)))
	{
		printf("Vegeleges legyen a kerdes?\n");
		DisplayQuest(qest);
		printf("0 - nem\n"
			   "1 - igen\n");
		
		int ans = GetNumber("Valasztas: ", 0, 1);
		if (ans)
			write(f2, &qest, sizeof(struct Question));
	}
	close(f2);
	close(f);
}

void Handler(int signum)
{
	waiting = false;
	printf("A biztos bejelentkezett az MLSZ-be.\n");
}

void FillConsultation()
{
	int status;
	printf("-----------\n");
	int numOfQs = CountQuestions();
	if (numOfQs < 3)
	{
		printf("Legalabb 3 keredesrol lehet konzultalni!\n");
		return;
	}

	char city[100];
	printf("Melyik varosba menjen a kerdezobiztos? ");
	fgets(city, sizeof(city), stdin);
	signal(SIGUSR1, Handler);
	int toParent[2];
	int toChild[2];
	pipe(toParent);
	pipe(toChild);
	struct Question chosenQs[3];
	pid_t child=fork();
	int answers[3];
  
	if (child > 0) //MLSZ
	{
		while (waiting)
			usleep(1);
		
		close(toParent[1]);
		close(toChild[0]);
    
		struct Question temp;
		int chosen[3] = {0,0,0};
		while (chosen[0] == chosen[1] || chosen[0] == chosen[2] || chosen[2] == chosen[1])
		{
			int i;
			for (i=0; i<3; i++)
				chosen[i] = rand() % numOfQs + 1;
		}
    
		int f = open(ffname, O_RDONLY);
		int i=0;
		int j=0;
		while (read(f, &temp, sizeof(struct Question)))
		{
			i++;
			if (i==chosen[0] || chosen[1] == i || chosen[2] == i)
			{
				chosenQs[j] = temp;
				j++;
			}
		}
		close(f);
    
		printf("A kisorsolt kerdesek:  \n");
		for(i=0; i<3; i++)
			DisplayQuest(chosenQs[i]);

		write(toChild[1], chosenQs, sizeof(chosenQs));
		int tester = 0;
		while (read(toParent[0], answers, sizeof(answers)) != 0)
		{
			printf("%d. kitolto valaszai: \n", tester+1);
			int i;
			for (i=0; i<3; i++)
				printf("valasz %d. kerdesre: %d\n", i+1, answers[i]);
		
			tester++;
		}
		
		printf("MLSZ kuldte a kerdeseket.\n");
		waitpid(child, &status, 0);
		close(toParent[0]);
		close(toChild[1]);
		waiting = true;
	} else
	{
		close(toParent[0]);
		close(toChild[1]);
		printf("A kerdezobiztos elindult %s varosaba\n", city);
		kill(getppid(), SIGUSR1);
		read(toChild[0], chosenQs, sizeof(chosenQs));
		int numOfPeople = rand() % 11 + 10;
		int i;
    
		for (i=0; i<numOfPeople; i++)
		{
			int j;
			for (j=0; j<3; j++)
				answers[j] = rand() % chosenQs[j].answNum + 1;
			write(toParent[1], answers, sizeof(answers));
		}
		
		close(toParent[1]);
		close(toChild[0]);
		exit(0);
	}
}

void StartMenu()
{
	srand(time(NULL));
	bool quit = false;
	printf("\n\n   MLSZ - Nemzeti Konzultacio\n\n");
	while(!quit)
	{
		printf("\nKerem valasszon az alabbi menupontok kozul\n\n 1. Uj Kerdes hozzaadasa\n 2. Kerdes modositasa\n 3. Kerdes(ek) listazasa\n 4. Kerdes torlese\n 5. Kerdoiv befejezese, konzultacio inditasa\n 6. Kilepes\n");
		int option = GetNumber("Valasztas: ", 1, 6);

		switch (option)
		{
			case 1:
				AddQuestion();
				break;
			case 2:
				EditQuestion();
				break;
			case 3:
				GetList();
				break;
			case 4:
				DeleteQuestion();
				break;
			case 5:
				Ending();
				FillConsultation();
				break;
			case 6:
				quit = true;
				printf("\nKoszonjuk, hogy kitoltotte a konzultaciot!");
				break;
			default:
				printf("Error! Entered value: %d\n", option);
				break;
    }
  }
}

int main(int argc, char** argv)
{
	StartMenu();	
	return 0;
}