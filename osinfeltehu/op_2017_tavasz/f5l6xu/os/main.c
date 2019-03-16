#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>


struct survey{
    char question[100];
    char answer1[100];
    char answer2[100];
    char answer3[100];
    char answer4[100];
    char time[100];
};

int main()
{
	char c[100];
    struct survey questions[100];
    printf("Uj kerdoivet szeretne letrehozni, vagy a meglevot hasznalni? (u/r)\n");
    print:scanf("%[^\n]",&c);
	getchar();

    if(strcmp(c, "r") == 0)
        goto step2;
    else if(strcmp(c,"u") == 0)
        goto step1;

    step1: printf("Adjon nevet az uj kerdoivnek. (.txt) \n");
    char name[100];
    scanf("%[^\n]",&name);
	getchar();

    int num;
    printf("Hany kerdest szeretne hozzaadni?\n");
    scanf("%d",&num);
    getchar();

    int i;

    for(i=1;i<num+1;i++)
    {
        printf("[%d]kerdes:",i);
        scanf("%[^\n]",&questions[i].question);
		getchar();
        printf("[%d]valasz 1:",i);
        scanf("%[^\n]",&questions[i].answer1);
		getchar();
        printf("[%d]valasz 2:",i);
        scanf("%[^\n]",&questions[i].answer2);
		getchar();
        printf("[%d]valasz 3:",i);
        scanf("%[^\n]",&questions[i].answer3);
		getchar();
        printf("[%d]valasz 4:",i);
        scanf("%[^\n]",&questions[i].answer4);
		getchar();
        time_t timep;
        time (&timep);
        strcpy(questions[i].time,ctime(&timep));
        printf("%s\n",questions[i].time);
    }

    goto start;

    step2: printf("Melyik meglevo kerdoivet szeretne hasznalni?(txt)\n");
    system("ls -a | grep *.txt");
    char usename[100];
    scanf("%[^\n]",&usename);
	getchar();
    int number=0;

    FILE *fp;
    char *name2 = usename;
    if ((fp=fopen(usename,"r"))==NULL)
        {
            printf("Hiba! Nem lehet megnyitni a fajlt! \n",name2);
            goto step2;
        }
    while(fscanf(fp,"%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];",&questions[number+1].question,
               &questions[number+1].answer1,&questions[number+1].answer2,
               &questions[number+1].answer3,&questions[number+1].answer4,
               &questions[number+1].time)==6)
    {
        number++;
    }

    fclose(fp);

    int j;

    do{

        start: printf("Kerem adja meg a vegrehajtani kivant muvelet szamat: \n");
        printf("1. Kerdes hozzaadasa \n");
        printf("2. Kerdes torlese \n");
        printf("3. Kerdes modositasa \n");
        printf("4. Kerdes valasztasa veglegesitesre \n");
        printf("5. Kerdesek listazasa \n");
        printf("6. Kerdoiv letrehozasa\n");
        printf("7. Futtatas\n");
        printf("8. Kilepes. \n");

        scanf("%d", &j);

        int d;
        char name1[100];
        if (strcmp(c, "u") == 0)
        {
            d = num;
            strcpy(name1,name);
        }
        else if(strcmp(c, "r") == 0)
        {
            d = number;
            strcpy(name1,usename);
        }

        int s=0;
        while(0<strlen(questions[s+1].question))
        {
            s++;
        }


        switch(j)
        {

            case 1:add_question(c,d,questions);break;
            case 2:delete_question(s,questions);break;
            case 3:modify_question(s,questions);break;
            case 4:end(s,name1,questions);break;
            case 5:list_question(s,questions);break;
            case 6:create(s,questions);break;
            case 7:run(s,questions);break;
            case 8:exit(0);break;

        }


         goto start;

    } while(j != 8);


	return 0;
}

void add_question(char c[100], int number, struct survey questions[100]){
    int num;
    int num1;
    int i;
    printf("Hany kerdest szeretne hozzaadni?\n");
    scanf("%d",&num);
    getchar();
    num1 = number + num;

    for(i=number+1;i<num1+1;i++)
    {
        printf("[%d]kerdes:",i);
        scanf("%[^\n]",&questions[i].question);
		getchar();
        printf("[%d]valasz 1:",i);
        scanf("%[^\n]",&questions[i].answer1);
		getchar();
        printf("[%d]valasz 2:",i);
        scanf("%[^\n]",&questions[i].answer2);
		getchar();
        printf("[%d]valasz 3:",i);
        scanf("%[^\n]",&questions[i].answer3);
		getchar();
        printf("[%d]valasz 4:",i);
        scanf("%[^\n]",&questions[i].answer4);
		getchar();
        time_t timep;
        time (&timep);
        strcpy(questions[i].time,ctime(&timep));
        printf("%s\n",questions[i].time);
    }
}

void delete_question(int s,struct survey questions[100])
{
    list_question(s,questions);

    printf("Kerem adja meg a torolni kivant kerdes sorszamat : \n");
    int d;
    int i;
    scanf("%d",&d);
    getchar();

    for (i=d;i<s+2;i++)
    {
        strcpy(questions[i].question,questions[i+1].question);
        strcpy(questions[i].answer1,questions[i+1].answer1);
        strcpy(questions[i].answer2,questions[i+1].answer2);
        strcpy(questions[i].answer3,questions[i+1].answer3);
        strcpy(questions[i].answer4,questions[i+1].answer4);
        strcpy(questions[i].time,questions[i+1].time);
    }
    s = s - 1;
    list_question(s,questions);
}

void modify_question(int s,struct survey questions[100])
{
    list_question(s,questions);
    printf("Kerem adja meg a modositani kivant kerdes sorszamat! \n");
    int d;
    scanf("%d",&d);
    getchar();

    printf("Melyiket szeretne modositani? (kerdes(1), valasz1(2),valasz2(3),valasz3(4),valasz4(5))\n");
    int curr;
    scanf("%s",&curr);

    char e[100];

    getchar();

    switch(curr)
    {
        case 1:scanf("%[^\n]",&e);getchar();strcpy(questions[d].question,e);
        time_t timep;
        time (&timep);
        strcpy(questions[d].time,ctime(&timep));
        printf("%s\n",questions[d].time);break;
        case 2:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer1,e);break;
        case 3:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer2,e);break;
        case 4:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer3,e);break;
        case 5:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer4,e);break;
    }
    s = s;
    time_t timep;
        time (&timep);
        strcpy(questions[d].time,ctime(&timep));
        printf("%s\n",questions[d].time);
    list_question(s,questions);
}

void end(int s,char name[100],struct survey questions[100]){
    FILE *fp;
    fp = fopen(name,"w+");

    int i;
    for (i=1;i<s+1;i++)
    {
        fprintf(fp,"%s;%s;%s;%s;%s;%s;",
               questions[i].question,questions[i].answer1,
               questions[i].answer2,questions[i].answer3,
               questions[i].answer4,questions[i].time);
    }
    fclose(fp);
    exit(0);
}

void list_question(int s,struct survey questions[100]){
    int i;
    for (i=1;i<s+1;i++)
    {
        printf("[%d]kerdes:%s; valasz1:%s; valasz2:%s; valasz3:%s; valasz4:%s; ido:%s;\n",
               i,questions[i].question,questions[i].answer1,
               questions[i].answer2,questions[i].answer3,
               questions[i].answer4,questions[i].time);
    }
}


void create(int s,struct survey questions[100]){
    list_question(s,questions);
    printf("Hany kerdest szeretne valasztani\n");
    int m;
    scanf("%d",&m);
    getchar();
    struct mem
    {
        int t;
    }
    p[100];
    printf("Kerem a kerdesek szamat.\n");
    int j;
    for (j=1;j<m+1;j++)
    {
        scanf("%d",&p[j].t);
        getchar();
    }
    printf("Adja meg a mentett fajl nevet.\n");
    char name[100];
    scanf("%[^\n]",&name);
	getchar();

    FILE *fp;
    fp = fopen(name,"w+");

    int i;
    int z=0;
    for (i=1;i<s+1;i++){
        for(j=1;j<m+1;j++){
            if (i=p[j].t)
            {
                z++;
                fprintf(fp,"[%d]kerdes:%s; valasz1:%s; valasz2:%s; valasz3:%s; valasz4:%s; ido:%s;\n",
                    z,questions[i].question,questions[i].answer1,
                    questions[i].answer2,questions[i].answer3,
                    questions[i].answer4,questions[i].time);
            }
        }
    }
    fclose(fp);
}

struct str
{
	char question[100];
    char answer1[100];
    char answer2[100];
    char answer3[100];
    char answer4[100];
};

void handler(int signumber)
{
  printf("Az adatok megerkeztek.\n");
}

void run(int s,struct survey questions[100])
{
    system("clear");
	signal(SIGTERM,handler);
	int status;

	int pipefd[2];
	pid_t pid;

	if (pipe(pipefd) == -1)
    {
            perror("Pipe hiba keletkezett!\n");
            exit(EXIT_FAILURE);
    }

    int cfd[2];

	if (pipe(cfd))
	{

		perror("Fork hiba keletkezett!\n");
		exit(EXIT_FAILURE);
	}

    pid=fork();

    if(pid ==0)
    {
		close(pipefd[1]);
		close(cfd[0]);
		childProcess(cfd[1],pipefd[0], questions, s);
	}

	else
	{
		close(pipefd[0]);
		close(cfd[1]);
		parentProcess(cfd[0],pipefd[1], questions, s);
	}
}

void parentProcess(int fC,int tC,struct survey questions[100],int s)
{
    struct str qu[100];
    struct str qm[100];

    int i;
    for (i=0;i<3;i++)
    {
			write(tC, &qu[i].question,sizeof(qu[i].question));
    }


	int people;
	read(fC, &people, sizeof(int));
	for(int j=0; j < people; ++j)
	{
		printf("\n%d-edik kitolto:",j);
		for(int qindex=0; qindex < 3; ++qindex )
		{
				int aindex;
				read(fC, &aindex, sizeof(int));
				printf("%d,",aindex);
		}
		printf("\n");
	}
	close(fC);
	close(tC);
}

void childProcess(int tP,int fP,struct survey questions[100],int s)
{

	int people=10 + (rand() % 10 + 1);
	int aOut;
	write(tP, &people,sizeof(int));
	for(int i=0; i < people; ++i)
	{
		for(int j=0; j < 3; ++j)
		{
            int aCount = 4;
            if(questions[j].answer3[0] == '\0'){--aCount;}
			if(questions[j].answer4[0] == '\0'){--aCount;}
			aOut= rand() % aCount + 1;
			write(tP,&aOut, sizeof(int));
		}
	}
	close(tP);
	close(fP);
	exit(0);
}
