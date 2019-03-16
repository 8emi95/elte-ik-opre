/*
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdet a labdajátékok felvirágoztatására!
Elso lépésként meg kell alkotni azt a kérdéssort és hozzá kérdésenként azt a minimum ketto, maximum 4 választási lehetoséget
amelyet majd a labdajátékok jobbítása iránt felelosséget érzok kitölthetnek. A készítendo program adjon lehetoséget a kérdéssor
megalkotására. Tudjunk új kérdést felvenni, kérdést módosítani és kérdést törölni. Módosításhoz, törléshez kínálja fel választásra
a kérdést és válaszait a program! Tudjunk emellett természetesen kérdéseket és válaszaikat listázni! A kérdésekhez tároljuk
automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc).
Végezetül a program adjon lehetoséget a kérdések véglegesítésére, azaz annak meghatározására, hogy mely kérdések szerepeljenek
a végso konzultációs kérdoíven!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_SIZE 512

#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

void menu();
void Listazas();
void ujKerdes();
void modKerdes();
void torKerdes();
void veglegesKerdessor();
void futtatas();

void masolas(char forrasNeve[50]);
void torol(char forrasNeve[50], int sorSzam);
int kereses(char forrasNeve[50], char szoveg[50]);

struct felmeres
{
	char k[50];
	char v1[50];
	char v2[50];
	char v3[50];
	char v4[50];
}kerdesek[100], konzi[3];

void gyerekFolyamat(int tP,int fP, struct felmeres konzi[3]);
void szuloFolyamat(int fC,int tC, struct felmeres konzi[3]);



int main()
{
    int choice = 1;
	int hiba = 0;

	while(choice != 7)
	{
		if(!hiba) { menu(); }

		while(!scanf("%d", &choice))
		{
			scanf("%s");
			printf("\nA megadott parancs nem ismerheto fel! Ujra: ");
		}
		getchar();

		hiba = 0;

		switch(choice)
		{
			case 1:
				Listazas();
				break;
			case 2:
				ujKerdes();
				break;
			case 3:
				modKerdes();
				break;
			case 4:
				torKerdes();
				break;
            case 5:
				veglegesKerdessor();
				break;
			case 6:
				futtatas();
				break;
			case 7:
				printf("\nViszont latasra! A megadott adatok mentesre kerultek!\n");
				break;
			default:
				printf("\nA megadott parancs nem ismerheto fel! Ujra: ");
				hiba = 1;
				break;
		}
	}
}

void menu()
{
    printf("\n\n\t\t M \t E \t N\t U\t \n\n");
    printf("\n \t\t1. Kerdessor listazasa\n");
    printf("\t\t2. Uj kerdes felvetele\n");
    printf("\t\t3. Kerdes modositasa\n");
    printf("\t\t4. Kerdes torlese\n");
    printf("\t\t5. Kerdessor veglegesitese\n");
	printf("\t\t6. Konzultacio\n");
    printf("\t\t7. Kilepes a programbol\n");
	printf("\nParancs: ");

}

void Listazas(){
    FILE* kerdessor;
	int db = 0;

	if(kerdessor = fopen("kerdessor.txt", "r"))
	{
		char line[250];

		while(fgets(line, 250, kerdessor) != NULL)
		{
			db++;

						db++;

			if(db == 1)
			{
				printf("\nA lehetseges kerdesek: \n");
			}

			char akt[50];
			int i = 0;

			while(line[i] != ',')
			{
				akt[i] = line[i];
				i++;
			}

			akt[i] = '\0';
			printf("%s\n", akt);
		}


		if(db == 0)
		{
			printf("\nNincs megjelenitheto kerdes!\n");
		}

		fclose(kerdessor);
	}
	else
	{
		printf("\nNincs megjelenitheto kerdes!\n");
	}


}
void ujKerdes(){

    FILE* kerdessor;
	kerdessor = fopen("kerdessor.txt", "a+");

	char kerdes[50];
	char valasz1[50];
	char valasz2[50];
	char valasz3[50];
	char valasz4[50];
    int year;
	int month;
	int day;
	int hour;
	int minute;
	int sec;

	printf("\nAdjon meg uj kerdest: ");
	fgets(kerdes, 50, stdin);
	kerdes[strlen(kerdes)-1] = '\0';

    printf("1. Valasz: ");
    fgets(valasz1, 50, stdin);
    valasz1[strlen(valasz1)-1] = 0;

    printf("2. Valasz: ");
    fgets(valasz2, 50, stdin);
    valasz2[strlen(valasz2)-1] = 0;

    printf("3. Valasz: ");
    fgets(valasz3, 50, stdin);
    valasz3[strlen(valasz3)-1] = 0;

    printf("4. Valasz: ");
    fgets(valasz4, 50, stdin);
    valasz4[strlen(valasz4)-1] = 0;

	if(!(kereses("kerdessor.txt", kerdes)))
	{
	    time_t rawtime;
        struct tm *timeInfo;
        time (&rawtime);
        timeInfo = localtime(&rawtime);
        year = timeInfo -> tm_year+1900;
        month = timeInfo -> tm_mon+1;
        day = timeInfo -> tm_mday;
        hour = timeInfo -> tm_hour;
        minute = timeInfo -> tm_min;
	    sec = timeInfo -> tm_sec;

		fprintf(kerdessor, "%s, %s, %s, %s, %s,  %d.%d.%d %d:%d:%d\n", kerdes, valasz1, valasz2, valasz3, valasz4, year, month, day, hour, minute, sec);
	}
	else
	{
		printf("\nA kerdes mar letezik!\n");
	}
	fclose(kerdessor);
}

void modKerdes(){

    char kerdes[50];
	Listazas();
	printf("Modositani kivant kerdes: ");
	fgets(kerdes, 50, stdin);

	if(!(kereses("kerdessor.txt", kerdes)))
	{
		printf("\nA megadott kerdes nem szerepel a lehetseges kozott!\n");
	}
	else
	{
		int index = kereses("kerdessor.txt", kerdes);

		if(index)
		{
		    torol("kerdessor.txt", index);
		    ujKerdes();
        }
		else
		{
			printf("\nAz adatbazisban nem szerepel a megadottaknak megfelelo adat!\n");
		}
	}

}

void torKerdes()
{
    char kerdes[50];
    Listazas();
	printf("\nA torolni kivant kerdest: ");
	fgets(kerdes, 50, stdin);
	int sorSzam = kereses("kerdessor.txt", kerdes);
	if(!sorSzam)
	{
		printf("A megadott kerdes nem szerepel az adatbazisban!\n");
	}
	else
	{
        torol("kerdessor.txt", sorSzam);
        printf("A megadott kerdes torlesre kerult!\n");
	}

}
void torol(char forrasNeve[50], int sorSzam)
{
	int db = 0;
	char line[250];
	FILE* file;
	file = fopen(forrasNeve, "r");
	char copyFileName[50] = "help.txt";
	FILE* copyFile;
	copyFile = fopen(copyFileName, "w");

	while(fgets(line, 250, file) != NULL)
	{
		db++;
		if(db != sorSzam)
		{
			fprintf(copyFile, "%s", line);
		}
	}
	fclose(copyFile);
	fclose(file);
	remove(forrasNeve);
	rename(copyFileName, forrasNeve);
}

void masolas(char forrasNeve[50])
{
	int db = 0;
	char line[250];
	FILE* file;
	file = fopen(forrasNeve, "r");
	char copyFileName[50] = "veglegesites.txt";
	FILE* copyFile;
	copyFile = fopen(copyFileName, "w");

	while(fgets(line, 250, file) != NULL)
	{
        fprintf(copyFile, "%s", line);
	}
	fclose(copyFile);
	fclose(file);
}

void veglegesKerdessor(){

    char kerdes[50];

    Listazas();

    masolas("kerdessor.txt");
    printf("Az eddigi kerdesek veglegesitesre kerultek!\n");
}

int kereses(char forrasNeve[50], char szoveg[50])
{
	int sorSzam = 0;
	FILE* file;

	if(file = fopen(forrasNeve, "r"))
	{
		char line[270];
		int db = 0;

		while(fgets(line, 270, file) != NULL && sorSzam == 0)
		{
			db++;
			char akt[270];
			int i = 0;

			while(line[i] != ',')
			{
				akt[i] = line[i];
				i++;
			}

			akt[i] = '\0';

			if(strlen(akt) == strlen(szoveg)-1)
			{
				int j = 0;
				while(akt[j] == szoveg[j]) { j++; }
				if(j == strlen(akt))
				{
					sorSzam = db;
				}
			}
		}

		fclose(file);
	}

	return sorSzam;
}

void futtatas()
{
	int db=0;
	int i,j;
	FILE* vegleges;
	
	
	if(vegleges = fopen("veglegesites.txt", "r"))
	{
		char line[250];
		
		while(fgets(line, 250, vegleges) != NULL)
		{	j=0;
			i =0;
			while(line[i] != ',')
			{
				kerdesek[db].k[i] = line[j];
				i++;
				j++;
			}
			j++;
			kerdesek[db].k[i]='\0';
			i = 0;
			while(line[i] != ',')
			{
				kerdesek[db].v1[i] = line[j];
				i++;
				j++;
			}
			j++;
			kerdesek[db].v1[i]='\0';
			i = 0;
			while(line[i] != ',')
			{
				kerdesek[db].v2[i] = line[j];
				i++;
				j++;
			}
			j++;
			kerdesek[db].v2[i]='\0';
			i = 0;
			while(line[i] != ',')
			{
				kerdesek[db].v3[i] = line[j];
				i++;
				j++;
			}
			j++;
			kerdesek[db].v3[i]='\0';
			i = 0;
			while(line[i] != ',')
			{
				kerdesek[db].v4[i] = line[j];
				i++;
				j++;
			}
			kerdesek[db].v4[i]='\0';
			db++;
		}
	}
	fclose(vegleges);
		
	for(i=0; i<3; i++)
	{
		int j =rand()% db +1;
		konzi[i]=kerdesek[j];
	}
	
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
		gyerekFolyamat(cfd[1],pipefd[0], konzi);
	}

	else
	{
		close(pipefd[0]);
		close(cfd[1]);
		szuloFolyamat(cfd[0],pipefd[1], konzi);
	}
}

void szuloFolyamat(int fC,int tC, struct felmeres konzi[100])
{
	int i,j;
	int qindex;
	
	int people;
	read(fC, &people, sizeof(int));
	for(j=0; j < people; ++j)
	{
		printf("\n%d-edik kitolto:",j);
		for(qindex=0; qindex < 3; ++qindex )
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

void gyerekFolyamat(int tP,int fP, struct felmeres konzi[100])
{

	int people=10 + (rand() % 10 + 1);
	int aOut;
	write(tP, &people,sizeof(int));
	int i,j;
	
	for( i=0; i < people; ++i)
	{
		for(j=0; j < 3; ++j)
		{
            int aCount = 4;
            if(konzi[j].v3[0] == '\0'){--aCount;}
			if(konzi[j].v4[0] == '\0'){--aCount;}
			aOut= rand() % aCount + 1;
			write(tP,&aOut, sizeof(int));
		}
	}
	close(tP);
	close(fP);
	exit(0);
}

void handler(int signumber)
{
  printf("Az adatok megerkeztek.\n");
}


	

