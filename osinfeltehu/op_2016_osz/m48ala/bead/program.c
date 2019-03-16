#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512

int memberId;

void menu();
void listazas();
void beszuras();
void modositas();
void torles();
void copyfile();
void rendezvenyTorles();
void ujRendezveny()

int main(int argc,char *argv[])
{
    menu();
	
	return 0;
}

void menu()
{
	char menu[MAX_SIZE];

	while(1 < 2)
	{
		//beszuras();

		printf("\nValasszon egy lehetoseget: (h - help)\t");
		scanf("%s",menu);

		if( strcmp(menu,"h") == 0 )
		{
			printf("LISTAZAS:\tl\n");
			printf("BESZURAS:\tb\n");
			printf("MODOSITAS:\tm\n");
			printf("TORLES:\t\tt\n");
			printf("UJ RENDEZVENY:\tu\n");
			printf("RENDEZVENY TORLES:\trt\n");
			printf("KILEPES:\tNyomjon CTRL + C -t a kilepeshez\n");
		}
		if( strcmp(menu,"l") == 0 )
		{
			listazas();
		}
		else if( strcmp(menu,"b") == 0 )
		{
			beszuras();
		}
		else if( strcmp(menu,"m") == 0 )
		{
			modositas();
		}
		else if( strcmp(menu,"t") == 0 )
		{
			torles();
		}
		else if( strcmp(menu,"u") == 0 )
		{
			ujRendezveny();
		}
		else if( strcmp(menu,"rt") == 0 )
		{
			rendezvenyTorles();
		}
	}	
}

void listazas()		//LISTAZAS
{
	FILE *members;
	
	members = fopen("members.txt","r");
		
	char kiir[MAX_SIZE];
	printf("\nVENDEGEK ADATAI:");
	while(fgets(kiir,100,members) != NULL)
	{
		printf("%s",kiir);
	};
	
	fclose(members);
}


void beszuras()		//BESZURAS
{
	time_t rawtime;
	struct tm * timeinfo;
	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	
	FILE *members;
	members = fopen("members.txt","a+");
	
	char nev[MAX_SIZE];	//tag neve	
	char email[MAX_SIZE];	//tag email cime
	int rendId;	//rendezveny sorszama

	printf("\nA rendezvenyen valo reszvetelhez regisztracio szukseges!\n");
	printf("Kerem adja meg a nevet: \t");
	scanf("%s",&nev);

	printf("\nKerem adja meg az e-mail cimet: \t");
	scanf("%s",&email);
	
	printf("\nKerem adja meg a rendezveny sorszamat: \t");
	scanf("%d",&rendId);

	
	fprintf(members,"\n%s %s %d %s", nev, email, rendId, asctime (timeinfo));
	
	fclose(members);

	memberId = memberId + 1;
	printf("A jelentkező sorszáma:\t", "%d", &memberId);
}

void torles()		//TORLES
{
	char neve[MAX_SIZE];
	printf("Adja meg annak a vendegnek a nevet, akit torolni szeretne:\t");
	scanf("%s", neve);
	
	FILE *members;
	FILE *helpfile;
	members = fopen("members.txt", "r");
	helpfile = fopen("helpfile.txt", "w");
	char line[MAX_SIZE];
	
	while(fgets(line, sizeof(line), members) != NULL)
	{
		char nev[MAX_SIZE];
		char email[MAX_SIZE];
		int rendId;
		char ido[MAX_SIZE];		

		sscanf(line,"%s %s %d %s", nev, email, &rendId, ido);
		if( strcmp(nev, neve) != 0 )
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(members);
	fclose(helpfile);
	
	copy_file("helpfile.txt","members.txt");
}

void copy_file(const char *from, const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen(from, "r");
	t = fopen(to, "w");
	
	char line[MAX_SIZE];
	
	char nev[MAX_SIZE];
	char email[MAX_SIZE];
	int rendId;
	char ido[MAX_SIZE];
	
	while(fgets(line, sizeof(line), fr) != NULL)
	{
		sscanf(line, "%s %s %d %s", nev, email, &rendId, ido);
		fprintf(t, "%s %s %d %s\n", nev, email, rendId, ido);
	}
	
	fclose(fr);
	fclose(t);
	
	remove("helpfile.txt");
}

void modositas()		//MODOSITAS
{
	char neve[MAX_SIZE];
	printf("Adja meg annak a vendegnek a nevet, akinek az adatait modositani szeretne:\t");
	scanf("%s",neve);
	
	FILE *members;
	FILE *helpfile;
	members = fopen("members.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),members) != NULL)
	{
		char nev[MAX_SIZE];
		char email[MAX_SIZE];
		int rendId;
		char ido[MAX_SIZE];

		sscanf(line,"%s %s %d %s", nev, email, &rendId, ido);
		
		//Bekert uj adatok
		char newNev[MAX_SIZE];
		char newEmail[MAX_SIZE];
		int newRendId;
				
		if( strcmp(nev, neve) == 0 )
		{
			char menu[MAX_SIZE];
			
			printf("Mit szeretnel modositani?\n");
			printf("Vendeg neve: (n)\n");
			printf("Vendeg email cime: (e)\n");
			printf("Rendezveny szama: (r)\n");
						
			scanf("%s",menu);
			
			if( strcmp(menu,"n") == 0 )
			{
				printf("Adja meg a vendeg uj nevet:\t");
				scanf("%s",newNev);
				fprintf(helpfile,"%s %s %d %s\n", newNev, email, rendId, ido);
			}
			else if( strcmp(menu,"e") == 0 )
			{
				printf("Adja meg a vendeg uj email cimet:\t");
				scanf("%s",newEmail);
				fprintf(helpfile,"%s %s %d %s\n", nev, newEmail, rendId, ido);
			}
			else if( strcmp(menu,"r") == 0 )
			{
				printf("Adja meg a rendezvény uj szamat:\t");
				scanf("%d",&newRendId);
				fprintf(helpfile,"%s %s %d %s\n", nev, email, newRendId, ido);
			}
			else
			{
				modositas();
			}
		}
		else
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(members);
	fclose(helpfile);
	
	copy_file("helpfile.txt","members.txt");	
}

void ujRendezveny()
{
	FILE *rendezveny;
	rendezveny = fopen("rendezveny.txt","a+");
	
	int rendId;
	char rendNev[MAX_SIZE];
	
	printf("Add meg az uj rendezveny adatait!");
	printf("\nRendezveny azonositoja:\t");
	scanf("%d",&rendId);
	printf("\nRendezveny neve:\t");
	scanf("%s",rendNev);
	
	fprintf(rendezveny,"\n%d %s", rendId, rendNev);
	
	fclose(rendezveny);
}

void rendezvenyTorles()
{
	char neve[MAX_SIZE];
	printf("Adja meg annak a rendezvenynek a nevet, amelyet torolni szeretne:\t");
	scanf("%s", neve);
	
	FILE *rendezveny;
	FILE *helpfile;
	rendezveny = fopen("rendezveny.txt", "r");
	helpfile = fopen("helpfile.txt", "w");
	char line[MAX_SIZE];
	
	while(fgets(line, sizeof(line), rendezveny) != NULL)
	{
		int rendId;
		char rendNev[MAX_SIZE];		

		sscanf(line,"%d %s", &rendId, rendNev);
		if( strcmp(rendNev, neve) != 0 )
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(rendezveny);
	fclose(helpfile);
	
	copy_file("helpfile.txt","rendezveny.txt");
}
