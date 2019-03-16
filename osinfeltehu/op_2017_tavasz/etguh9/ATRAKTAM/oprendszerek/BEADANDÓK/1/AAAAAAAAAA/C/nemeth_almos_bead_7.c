#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime


#define MAX_QUESTION_SIZE 21
#define MAX_ANSWER_SIZE 81




struct Question
{
	char question[MAX_QUESTION_SIZE];
	int answers_Count;
	char answers[4][MAX_ANSWER_SIZE];
	size_t date;
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


int read_Question_Number()
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


void list_Questions()
{
	int i;
	for (i = 0; i < questions_Count; ++i)
	{
		printf("%i. kerdes:\n", i+1);
		printf("\tKerdes szovege: %s\n", questions[i].question);
		int j;
		for (j = 0; j < questions[i].answers_Count; ++j)
		{
			printf("\t\t%i. valasz: %s\n", j+1, questions[i].answers[j]);
		}
		struct tm * valm;
		valm = localtime(&questions[i].date);
		printf ( "\t\tBeviteli datum: %s", asctime(valm) );
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
		temp_Questions = (struct Question*)malloc(questions_Count * sizeof(struct Question));
		int i;
		for (i = 0; i < questions_Count-1; ++i)
		{
			temp_Questions[i] = questions[i];
		}
		free(questions);
	}

	if (temp_Questions != NULL)
	{
		questions = temp_Questions;

		struct Question q;
		printf("Kerdes szovege (max %i karakter): ", MAX_QUESTION_SIZE);
	    read_String(q.question, MAX_QUESTION_SIZE);
    	printf("Kerdes szovege:  %s\n\n", q.question);

		q.answers_Count = 4;   ///***********************

		int i;
		for (i = 0; i < q.answers_Count; ++i)
		{
			printf("%i. valasz szovege (max %i karakter): ", i+1, MAX_ANSWER_SIZE);
			read_String(q.answers[i], MAX_ANSWER_SIZE);
		    printf("Valasz szovege:  %s\n\n", q.answers[i]);
		}

		q.date = time(NULL);
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
	if (questions_Count > 0)
	{
		list_Questions();
		printf("\nAdja meg a modositando kerdes sorszamat (ha megsem kivan kerdest modositani, irjon be 0-t): ");

		int change;
		scanf("%d", &change);

		while (change < 0 || change > questions_Count)
		{
			int a;
			while ( (a = getchar()) != '\n' && a != EOF ) {}
			scanf("%d", &change);
		}

		if (change > 0)
		{
			--change;
			printf("1 - Kerdes szovegenek modositasa\n");
			printf("2 - Valaszok modositasa\n");
			printf("0 - Megsem\n");

			char c  = getchar();
			while (c != '1' && c != '2' && c != '0')
			{
				c = getchar();
			}
			int a;
			while ( (a = getchar()) != '\n' && a != EOF ) {}

			if (c == '1')
			{
				printf("Korabbi verzio: %s\n", questions[change].question);
				printf("Kerdes szovege (max %i karakter): ", MAX_QUESTION_SIZE);
			    read_String(questions[change].question, MAX_QUESTION_SIZE);
    			printf("Modositott:  %s\n\n", questions[change].question);
			}
			else if (c == '2')
			{
				printf("1 - Valasz szovegenek modositasa\n");
				printf("2 - Valasz hozzaadasa\n");
				printf("3 - Valasz torlese\n");
				printf("0 - Megsem\n");

				char c  = getchar();
				while (c != '1' && c != '2' && c != '3' && c != '0')
				{
					c = getchar();
				}
				int a;
				while ( (a = getchar()) != '\n' && a != EOF ) {}

				if (c == '1')
				{
					printf("Valaszok:\n");
					int i;
					for (i = 0; i < questions[change].answers_Count; ++i)
					{
						printf("%i. valasz: %s\n", i+1, questions[change].answers[i]);
					}
					printf("\nAdja meg a modositando valasz sorszamat (ha megsem kivan valaszt modositani, irjon be 0-t): ");

					int change_ans;
					scanf("%d", &change_ans);

					while (change_ans < 0 || change_ans > questions[change].answers_Count)
					{
						int a;
						while ( (a = getchar()) != '\n' && a != EOF ) {}
						scanf("%d", &change_ans);
					}
					if (change_ans > 0)
					{
						--change_ans;
						printf("Korabbi verzio: %s\n", questions[change].answers[change_ans]);
						printf("Kerdes szovege (max %i karakter): ", MAX_ANSWER_SIZE);
					    read_String(questions[change].answers[change_ans], MAX_ANSWER_SIZE);
		    			printf("Modositott:  %s\n\n", questions[change].answers[change_ans]);
					}
				}
				else if (c == '2')
				{
					if (questions[change].answers_Count < 4)
					{
						printf("%i. valasz szovege (max %i karakter): ", questions[change].answers_Count, MAX_ANSWER_SIZE);
						read_String(questions[change].answers[questions[change].answers_Count], MAX_ANSWER_SIZE);
					    printf("Valasz szovege:  %s\n\n", questions[change].answers[questions[change].answers_Count]);
						++questions[change].answers_Count;
					}
					else
					{
						printf("Nem lehet tobb valaszt felvenni\n");
					}
				}
				else if (c == '3')
				{
					if (questions[change].answers_Count > 2)
					{
						int i;
						for (i = 0; i < questions[change].answers_Count; ++i)
						{
							printf("%i. valasz: %s\n", i+1, questions[change].answers[i]);
						}
						printf("\nAdja meg a torlendo  valasz sorszamat (ha megsem kivan valaszt torolni, irjon be 0-t): ");
						int del_ans;
						scanf("%d", &del_ans);
						while (del_ans < 0 || del_ans > questions[change].answers_Count)
						{
							int a;
							while ( (a = getchar()) != '\n' && a != EOF ) {}
							scanf("%d", &del_ans);
						}

						if (del_ans > 0)
						{
							--del_ans;
							--questions[change].answers_Count;
							int i;
							for (i = del_ans; i < questions[change].answers_Count; ++i)
							{
								strcpy(questions[change].answers[i], questions[change].answers[i+1]);
							}
						}
					}
					else
					{
						printf("Nem lehet valaszt torolni\n");
					}
				}
			}
		}
	}
	else
	{
		printf("Nincsenek kerdesek\n");
	}
}


void delete_Question()
{
	if (questions_Count > 0)
	{
		list_Questions();
		printf("\nAdja meg a torlendo kerdes sorszamat (ha megsem kivan torolni, irjon be 0-t): ");

		int del;
		scanf("%d", &del);

		while (del < 0 || del > questions_Count)
		{
			int a;
			while ( (a = getchar()) != '\n' && a != EOF ) {}
			scanf("%d", &del);
		}

		if (del > 0)
		{
			--questions_Count;

			struct Question* temp_Questions = (struct Question*)malloc(questions_Count * sizeof(struct Question));
			int i;
			for (i = 0; i < del-2; ++i)
			{
				temp_Questions[i] = questions[i];
			}
			for (i = del-1; i < questions_Count; ++i)
			{
				temp_Questions[i] = questions[i+1];
			}
			free(questions);
			questions = temp_Questions;
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




int main(int argc,char** argv)
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
		printf("1 - Kerdesek listazasa\n");
		printf("2 - Uj kerdes felvetele\n");
		printf("3 - Kerdes modositasa\n");
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
		while ( (a = getchar()) != '\n' && a != EOF ) {}
	/*
		int i;
		scanf("%1d", &i);
		printf("%i-t irtal be\n\n", i);

	*/

		switch (menu)
		{
			case '1':
{
//				size_t *tt = (size_t*)malloc((size_t)20 *sizeof(size_t));
//				struct tm *valm = localtime(tt);
			list_Questions(); break;
}
			case '2': new_Question(); break;
			case '3': change_Question(); break;
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



/*
 struct stat st;
 stat(argv[0],&st);  //fstat(file descriptor, stat structure)
 size_t t = st.st_mtime;
 printf("The last modification was: %s\n", ctime(&t)); //convert time_t to string 
*/




//scanf("%1d", &i);



/*
    time_t t = time(0);
    struct tm * now = localtime( & t );
printf("\n\n%i\n", now->tm_year + 1900);
printf("%i\n", now->tm_mon + 1);
printf("%i\n\n\n", now->tm_mday);
*/




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