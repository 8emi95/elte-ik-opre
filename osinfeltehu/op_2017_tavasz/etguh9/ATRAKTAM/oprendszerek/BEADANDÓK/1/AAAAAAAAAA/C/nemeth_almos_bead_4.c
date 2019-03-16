#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>


#define MAX_QUESTION_SIZE 11
#define MAX_ANSWER_SIZE 81




struct Question
{
	char question[MAX_QUESTION_SIZE];
	int numer_Of_Answers;
	char answers[4][MAX_ANSWER_SIZE];
	//date felvetel_date   //felvetel datuma    //***********BEFEJEZNI*************
	//int sorszam angolul  lehet h nem kell//***********BEFEJEZNI*************
};




struct Question* questions;
int questions_Count;




void read_String(char* s, int size)
{
	fgets(s, size, stdin);
	while(s[0] == '\n')
	{
	    fgets(s, size, stdin);
	}
    if (s[strlen(s) - 1] == '\n')
	{
       	s[strlen(s) - 1] = '\0';
	}
	else
	{
		int a;
		while ((a = getchar()) != '\n' && a != EOF) {}
	}
}


char read_Menu()
{
	//
}


void load_Questions()
{
	int f = open("data", O_RDONLY);
	if (f != -1)
	{
		read(f, &questions_Count, sizeof(int));
		questions = (struct Question*)malloc(questions_Count*sizeof(struct Question));
		
		int i;
		for (i = 0; i < questions_Count; ++i)
		{
			read(f, &(questions[i]), sizeof(struct Question));
		}
	
		close(f);
	}
	else
	{
		questions_Count = 0;
	}
}


void new_Question()
{
	++questions_Count;
	struct Question *temp_Questions;
	if (questions_Count == 1)
	{
		temp_Questions = (struct Question*)malloc(sizeof(struct Question));
	}
	else
	{	
		temp_Questions = (struct Question*)realloc(questions, questions_Count);
	}

	if (temp_Questions != NULL)
	{
		questions = temp_Questions;

		struct Question q;
		printf("Kerdes szovege (max %i karakter): ", MAX_QUESTION_SIZE);
	    read_String(q.question, MAX_QUESTION_SIZE);
    	printf("Kerdes szovege:  %s\n\n", q.question);

		int i;
		for (i = 0; i < 4; ++i)
		{
			printf("%i. valasz szovege (max %i karakter): ", i+1, MAX_ANSWER_SIZE);
			read_String(q.answers[i], MAX_ANSWER_SIZE);
		    printf("Valasz szovege:  %s\n\n", q.answers[i]);
		}

		questions[questions_Count - 1] = q;

		printf("\nkerdes: %s\n\n", questions[questions_Count - 1].question);
		printf("\nmeret: %i\n\n", questions_Count);
	}
	else
	{
		//HIBA
	}
}

void change_Question()
{
	//
}

void list_Questions()
{
	int i;
	for (i = 0; i < questions_Count; ++i)
	{
		printf("%i. kerdes:\n", i+1);
		printf("\tKerdes szovege: %s\n", questions[i].question);
		int j;
		for (j = 0; j < 4; ++j)
		{
			printf("\t\t%i. valasz: %s\n", j+1, questions[i].answers[j]);
		}
	}
}

void delete_Question()
{
	if (questions_Count > 0)
	{
		list_Questions();
		printf("\nAdja meg a torlendo kerdes sorszamat (ah megsem kivan torolni, irjon be 0-t): ");

		int del;
		scanf("%d", &del);

		while (del < 0 || del > questions_Count)
		{
			int a;
			while ( (a = getchar()) != '\n' && a != EOF ) {}
			scanf("%d", &del);
		}

		if(del > 0)
		{
			--questions_Count;

			int i;
			for (i = del-1; i < questions_Count; ++i)
			{
				questions[i] = questions[i+1];
			}

			questions = (struct Question*)realloc(questions, questions_Count);
		}
	}
	else
	{
		printf("Nincsenek kerdesek\n");
	}
}

void save_Questions()
{
	int f = open("data", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (f != -1)
	{
		write(f, &questions_Count, sizeof(int));

		int i;
		for (i = 0; i < questions_Count; ++i)
		{
			write(f, &(questions[i]), sizeof(struct Question));
		}
	
		close(f);
	}
	else
	{
		//HIBA
	}
}




int main()   //int argc,char** argv   //***********BEFEJEZNI*************
{
/*
char sgsg[2][10];
getchar();
int i;
for (i = 0; i < 3; ++i)
{
	strcpy(sgsg[i], "valami");
	printf("sgsg[%i] = %s\n", i, sgsg[i]);
}
*/


	load_Questions();
//hibakezelés   //***********BEFEJEZNI*************

	do
	{

		printf("PROGI\n\n");   //***********BEFEJEZNI*************

		//MENÜ   //***********BEFEJEZNI*************
		printf("MENU\n\n");
		printf("1 - Uj kerdes felvetele\n");
		printf("2 - Kerdes modositasa\n");
		printf("3 - Kerdesek listazasa\n");
		printf("4 - Kerdes torlese\n");
		printf("5 - Kerdesek veglegesitese\n");
		printf("9 - Kilepes\n\n");  

		char menu = getchar();
		while (menu != '1' && menu != '2' && menu != '3'
				&& menu != '4' && menu != '5' && menu != '9')
		{
			menu = getchar();
		}

		int a;
		while ( (a = getchar()) != '\n' && a != EOF ) {}   //buffer törlése
	/*
		int i;
		scanf("%1d", &i);
		printf("%i-t irtal be\n\n", i);

	*/

		switch (menu)
		{
			case '1': new_Question(); break;
			case '2': change_Question(); break;
			case '3': list_Questions(); break;
			case '4': delete_Question(); break;
			case '5': save_Questions(); break;
			case '9': exit(0); break;
		}
	} while (1);

/*
**********************************************************************************************************************************
    char *name = malloc (MAX_QUESTION_SIZE);
    if (name == NULL) {
        printf ("No memory\n");
        return 1;
    }

    printf("Irj be valamit (%i): ", MAX_QUESTION_SIZE);
    fgets (name, MAX_QUESTION_SIZE, stdin);
    if ((strlen(name)>0) && (name[strlen (name) - 1] == '\n'))
        name[strlen (name) - 1] = '\0';
    printf("Ezt irtad be:  %s\n\n", name);

	int a;
	while ( (a = getchar()) != '\n' && a != EOF ) {}

    printf("Irj be valamit (%i): ", MAX_QUESTION_SIZE);
    fgets (name, MAX_QUESTION_SIZE, stdin);
    if ((strlen(name)>0) && (name[strlen (name) - 1] == '\n'))
        name[strlen (name) - 1] = '\0';
    printf("Ezt irtad be:  %s\n\n", name);

    free (name);
*/
	return 0;
}





int main2(int argc,char** argv)
{
/*

new question-ben volt

int m;
for (m = 0; m < questions_Count; ++m)
{
	strcpy(questions[m].question, "valami");
	char *s = (char*)malloc(sizeof(char));
	snprintf(s, sizeof(sizeof(char)), "%i", m);
	strcat(questions[m].question, s);
	printf("%i: %s\n", m, questions[m].question);
}
*/


//scanf("%1d", &i);




printf("%i\n\n", sizeof(char));
printf("***********valami***********\n");
char *s = (char*)malloc(200*sizeof(char));
char *ss = (char*)malloc(8*sizeof(char));

strcpy(s, "123456789 _23456789 __3456789 ___456789 ____56789 _____6789 ______789 _______89 ________9 _________");

printf("\n\n%s\n\n\n", s);

free(s);
free(ss);	
    return 0;
}