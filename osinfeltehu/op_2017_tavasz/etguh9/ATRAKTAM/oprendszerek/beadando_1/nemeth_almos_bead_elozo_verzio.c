#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open   //****
#include <sys/stat.h>   //****
#include <errno.h> //perror, errno   //****
#include <string.h>
#include <time.h> //ctime


#define MAX_QUESTION_SIZE 200
#define MAX_ANSWER_SIZE 80




//Question struct:

struct Question
{
	char question[MAX_QUESTION_SIZE];
	int answers_Count;
	char answers[4][MAX_ANSWER_SIZE];
	size_t date;
};




//global variables:

struct Question* questions;
int questions_Count;




//function declarations:

void read_String(char* s, int size);
void load_Questions();
void list_Questions();
void new_Question();
void change_Question();
void delete_Question();
void finalize_Questions();
void save_Questions();
int menu();




//main function:

int main(int argc,char** argv)
{
	load_Questions();

	printf("\nMLSZ konzultacio\n");

	while (menu()) {}

	free(questions);

	return 0;
}




//function definitions:

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
		while ((getchar()) != '\n') {}
	}
}


void load_Questions()
{
	questions_Count = 0;

	if (access("data", F_OK) == 0)
	{
		printf("Szeretne betolteni a korabbi munkamenet kerdeseit? (i/n)");
		char c;
		do			
		{
			c = getchar();
			while(getchar() != '\n') {}
		} while (toupper(c) != 'I' && toupper(c) != 'N');
		
		if (toupper(c) == 'I')
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
				//HIBA
			}
		}
	}
}


void list_Questions()
{
	if (questions_Count > 0)
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
	else
	{
		printf("Nincsenek kerdesek\n");
	}
}


void new_Question()
{
	printf("Valaszok szama (minimum 2, maximum 4) (a kilepeshez irjon be 0-t): ");
	int c = -1;
	do
	{
		scanf("%d", &c);
		while ((getchar()) != '\n') {}
	} while (c < 2 && c != 0 || c > 4);
	
	if (c > 0)
	{
		struct Question q;
		q.answers_Count = c;

		printf("Kerdes szovege (max %i karakter): ", MAX_QUESTION_SIZE);
	   	read_String(q.question, MAX_QUESTION_SIZE);
	   	printf("Kerdes szovege:  %s\n\n", q.question);
	
		int i;
		for (i = 0; i < q.answers_Count; ++i)
		{
			printf("%i. valasz szovege (max %i karakter): ", i+1, MAX_ANSWER_SIZE);
			read_String(q.answers[i], MAX_ANSWER_SIZE);
		    printf("Valasz szovege:  %s\n\n", q.answers[i]);
		}

		q.date = time(NULL);

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
			questions[questions_Count - 1] = q;
		}
		else
		{
			//HIBA
		}
	}
}


void change_Question()
{
	if (questions_Count > 0)
	{
		list_Questions();
		printf("\nAdja meg a modositando kerdes sorszamat (ha megsem kivan kerdest modositani, irjon be 0-t): ");

		int change = -1;
		do
		{
			scanf("%d", &change);
			while ((getchar()) != '\n') {}
		} while (change < 0 || change > questions_Count);

		if (change > 0)
		{
			--change;
			printf("1 - Kerdes szovegenek modositasa\n");
			printf("2 - Valaszok modositasa\n");
			printf("0 - Megsem\n");

			int c = -1;
			do
			{
				scanf("%d", &c);
				while ((getchar()) != '\n') {}
			} while (c < 0 || c > 2);

			if (c == 1)
			{
				printf("Korabbi verzio: %s\n", questions[change].question);
				printf("Kerdes szovege (max %i karakter): ", MAX_QUESTION_SIZE);
			    read_String(questions[change].question, MAX_QUESTION_SIZE);
    			printf("Modositott:  %s\n\n", questions[change].question);
			}
			else if (c == 2)
			{
				printf("1 - Valasz szovegenek modositasa\n");
				printf("2 - Valasz hozzaadasa\n");
				printf("3 - Valasz torlese\n");
				printf("0 - Megsem\n");

				int c = -1;
				do
				{
					scanf("%d", &c);
					while ((getchar()) != '\n') {}
				} while (c < 0 || c > 3);

				if (c == 1)
				{
					printf("Valaszok:\n");
					int i;
					for (i = 0; i < questions[change].answers_Count; ++i)
					{
						printf("%i. valasz: %s\n", i+1, questions[change].answers[i]);
					}
					printf("\nAdja meg a modositando valasz sorszamat (ha megsem kivan valaszt modositani, irjon be 0-t): ");

					int change_ans = -1;
					do
					{
						scanf("%d", &change_ans);
						while ((getchar()) != '\n') {}
					} while (change_ans < 0 || change_ans > questions[change].answers_Count);

					if (change_ans > 0)
					{
						--change_ans;
						printf("Korabbi verzio: %s\n", questions[change].answers[change_ans]);
						printf("Kerdes szovege (max %i karakter): ", MAX_ANSWER_SIZE);
					    read_String(questions[change].answers[change_ans], MAX_ANSWER_SIZE);
		    			printf("Modositott:  %s\n\n", questions[change].answers[change_ans]);
					}
				}
				else if (c == 2)
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
				else if (c == 3)
				{
					if (questions[change].answers_Count > 2)
					{
						int i;
						for (i = 0; i < questions[change].answers_Count; ++i)
						{
							printf("%i. valasz: %s\n", i+1, questions[change].answers[i]);
						}
						printf("\nAdja meg a torlendo  valasz sorszamat (ha megsem kivan valaszt torolni, irjon be 0-t): ");

						int del_ans = -1;
						do
						{
							scanf("%d", &del_ans);
							while ((getchar()) != '\n') {}
						} while (del_ans < 0 || del_ans > questions[change].answers_Count);

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

		int del = -1;
		do
		{
			scanf("%d", &del);	
			while ((getchar()) != '\n') {}
		} while (del < 0 || del > questions_Count);

		if (del > 0)
		{
			--del;
			--questions_Count;

			struct Question* temp_Questions = (struct Question*)malloc(questions_Count * sizeof(struct Question));
			int i;
			for (i = 0; i < del; ++i)
			{
				temp_Questions[i] = questions[i];
			}
			for (i = del; i < questions_Count; ++i)
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


void finalize_Questions()
{
	if (questions_Count > 0)
	{
		int *questions_Mask = (int*)malloc(questions_Count*sizeof(int));
		int *chosen_Questions = (int*)malloc(questions_Count*sizeof(int));
		int chosen_Questions_Count = 0;

		int i;
		for (i = 0; i < questions_Count; ++i)
		{
			questions_Mask[i] = 0;
		}

		list_Questions();
		int c;
		do
		{
			printf("\nAdja meg a kovetkezo kerdes sorszamat (0-val veglegesiti a kerdessort): ");

			c = -1;
			do
			{
				scanf("%d", &c);
				while ((getchar()) != '\n') {}
			} while (c < 0 || c > questions_Count || c != 0 && questions_Mask[c-1] == 1);

			if (c > 0)
			{
				chosen_Questions[chosen_Questions_Count] = c-1;
				++chosen_Questions_Count;
				questions_Mask[c-1] = 1;
			}
		} while(c > 0 && chosen_Questions_Count < questions_Count);

		free(questions_Mask);

		printf("Szeretne elmenteni a megadott kerdessort (ezzel torli a ki nem valasztott kerdeseket)? (i/n)");
		char ch;
		do			
		{
			ch = getchar();
			while(getchar() != '\n') {}
		} while (toupper(ch) != 'I' && toupper(ch) != 'N');
	
		if (toupper(ch) == 'I')
		{
			struct Question *temp_Questions = (struct Question*)malloc(chosen_Questions_Count*sizeof(struct Question));
			
			int i;
			for (i = 0; i < chosen_Questions_Count; ++i)
			{
				temp_Questions[i] = questions[chosen_Questions[i]];
			}

			questions_Count = chosen_Questions_Count;
			free(questions);
			questions = temp_Questions;

			printf("A veglegesites megtortent\n");
		}

		free(chosen_Questions);
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

		printf("A mentes megtortent\n");
	}
	else
	{
		//HIBA
	}
}


int menu()
{
	printf("\nMENU\n\n");
	printf("1 - Kerdesek listazasa\n");
	printf("2 - Uj kerdes felvetele\n");
	printf("3 - Kerdes modositasa\n");
	printf("4 - Kerdes torlese\n");
	printf("5 - Kerdessor veglegesitese\n");
	printf("6 - Kerdessor mentese (fajlba)\n");
	printf("9 - Kilepes\n\n");  

	int menu = -1;
	do
	{
		scanf("%d", &menu);	
		while ((getchar()) != '\n') {}
	} while (menu < 1 || menu > 6 && menu != 9);

	switch (menu)
	{
		case 1: list_Questions(); break;
		case 2: new_Question(); break;
		case 3: change_Question(); break;
		case 4: delete_Question(); break;
		case 5: finalize_Questions(); break;
		case 6: save_Questions(); break;
		case 9: return 0; break;
	}

	return 1;
}