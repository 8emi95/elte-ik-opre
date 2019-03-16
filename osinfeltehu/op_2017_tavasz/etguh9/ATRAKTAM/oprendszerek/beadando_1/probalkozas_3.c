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


struct Question* question_Array;


void load_Questions()
{
//	int f = open("data", O_RDONLY);
//	if (f == -1)
	{
		int f = open("data", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
		int i = 45;
		write(f, &i, sizeof(int));
		i = 50;
		write(f, &i, sizeof(int), sizeof(int));
		i = 700;
		write(f, &i, sizeof(int), 0);
	}
//else
{
int f = open("data", O_RDONLY);

	int count;
	read(f, &count, sizeof(int), 0);   		//hibakezeles
	printf("hossz: %i\n", count);
	read(f, &count, sizeof(int), sizeof(int));   		//hibakezeles
	printf("hossz: %i\n", count);
	read(f, &count, sizeof(int));   		//hibakezeles
	printf("hossz: %i\n", count);
//	printf("s == %s\n", s);

	close(f);
}
}

void read_New_Question()
{
	//
}

void change_Question()
{
	//
}

void delete_Question()
{
	//
}

void list_Question()
{
	//
}

void save_Questions()
{
	int f = open("data", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
//        int f = open("adatok", O_CREAT, 600);
	printf("kiirt bitek szama: %i\n", write(f, "valami", 20));

	close(f);
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

	printf("PROGI\n\n");   //***********BEFEJEZNI*************

	//MENÜ   //***********BEFEJEZNI*************
	printf("\nMENU\n\n");
	printf("1 - Uj kerdes felvetele\n");
	printf("2 - Kerdes modositasa\n");
	printf("3 - Kerdes torlese\n");
	printf("4 - Kerdesek listazasa\n");
	printf("5 - Kerdesek veglegesitese\n");
	printf("9 - Kilepes\n\n");  

	char menu = getchar();
	while (menu != '1' && menu != '2' && menu != '3'
			&& menu != '4' && menu != '5' && menu != '9')
	{
		menu = getchar();
	}
//	while ( (menu = getchar()) == '\n' || menu == EOF ) {}
	int a;
	while ( (a = getchar()) != '\n' && a != EOF ) {}   //buffer törlése
/*
	int i;
	scanf("%1d", &i);
	printf("%i-t irtal be\n\n", i);
*/

	switch (menu)
	{
		case '1': read_New_Question();
		case '2': change_Question();
		case '3': delete_Question();
		case '4': list_Question();
		case '5': save_Questions();
		case '9': exit(0);
	}

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

printf("%i\n\n", sizeof(char));
printf("***********valami***********\n");
char *s = (char*)malloc(200*sizeof(char));
char *ss = (char*)malloc(8*sizeof(char));

strcpy(s, "123456789 _23456789 __3456789 ___456789 ____56789 _____6789 ______789 _______89 ________9 _________");

printf("\n\n%s\n\n\n", s);

free(s);
free(ss);

int k;
for (k = 0; k < 3; ++k)
{
	char karakter = getchar();
	char ai;
	while ( (ai = getchar()) != '\n' && ai != EOF ) { }
	printf("karakter: \"%c\"\n", karakter);
}



	
    return 0;
}