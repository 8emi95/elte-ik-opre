#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUESTION_SIZE 11
#define MAX_ANSWER_SIZE 81



int main()
{
	printf("PROGI\n\n");

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

	return 0;
}






#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno


struct QA
{
	char question[MAX_QUESTION_SIZE];
	char answer[MAX_ANSWER_SIZE];
};


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


/*
int j;
for (j = 0; j < 4; ++j)
{
//	system("clear");
	printf("\nMENU\n\n");
	printf("Olvasas - 1\n");
	printf("Iras - mindenmas\n\n");

	int i;
	scanf("%1d", &i);
	printf("%i-t irtal be\n\n", i);
}*/


	int i;  //törölni
    if (i == 1)
    {
		printf("Olvasas\n");
        int f = open("adatok", O_RDONLY);
		char *s;
		printf("beolvasott bitek szama: %i\n", read(f, s, 200));
		printf("s == %s\n", s);
		close(f);
    }
    else
    {
		printf("Iras\n");
		printf("Beleirok egy valamit\n");
int f = open("adatok", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
//        int f = open("adatok", O_CREAT, 600);
		printf("kiirt bitek szama: %i\n", write(f, "valami", 20));

		close(f);
    }
    return 0;
}