/* Feladat:
Ha október, akkor "Oktoberfest"! A "Sör-Kocsi Budapest"(SKB) társaság mintegy társrendezvényként, akciós sörtúrákat szervez. Egy sörbicikli túrához szervezzük a jelentkezőket! A készítendő program adjon lehetőséget:
-  új jelentkező adatainak (név,email,telefonszám, útvonal, hány fő, felvételi dátum) 
	~ rögzítésére
	~ módosítására
	~ törlésére. 
- 3 útvonal: Parlament, Hősök tere, Vár
- listázás: teljes névsor vagy útvonal alapján

Ezen kívül egy jelentkező megmondhatja, hogy hány fő nevében jelentkezik. (Minden hozott vendég 15% kedvezményt kap!) 
A program tudja még az adatok kezelése mellett a teljes névsort vagy az egyes útvonalakra jelentkezetteket listázni! A jelentkezéskor tárolja automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc).

Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk.*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define max 1024

void rogzites();
void modositas();
void torles();
void listazas();
void utvonal();
void copy_file(const char *from,const char *to);

int main(int argc, char *argv[])
{
	int i=0;
	//while (i>0)
	//{
		char* menu;	
		printf("Mit szeretnel tenni?\n");
		printf("Rogzites: r, modositas: m, torles: t, listazas: l, kilepes: k\n");
		printf("Melyik legyen? ");
		scanf("%s",menu);
		if(strcmp(menu, "r")==0)
		{
			rogzites();
		}
		else if(strcmp(menu, "m")==0)
		{
			modositas();
		}
		else if(strcmp(menu, "t")==0)
		{
			torles();
		}
		else if(strcmp(menu, "l")==0)
		{
			listazas();
		}
		else if(strcmp(menu, "k")==0)
		{
			return 0;
		}
	//}
	return 0;
}

void rogzites()
{
	FILE *jelentkezok;
	jelentkezok=fopen("jelentkezok.txt","a+");
	
	char nev[max]="";
	int fo=0;
	char email[max]="";
	char tel[max]="";
	char ut[max]="";
	
	printf("Add meg a jelentkezo adatait: \n");
	printf("Jelentkezo neve: ");
	scanf("%s",nev);
	printf("Fo: ");
	scanf("%d",&fo);
	printf("E-mail: ");
	scanf("%s",email);
	printf("Telefonszam: ");
	scanf("%s",tel);
	printf("Utvonal: ");
	scanf("%s",ut);
	
	time_t t = time(NULL);
    struct tm tm = *localtime(&t);
	fprintf(jelentkezok, "%s %d %s %s %s %d-%d-%d %d:%d:%d\n", nev, fo, email, tel, ut, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	fclose(jelentkezok);
}

void torles()
{
	char name[max];
	printf("Torlendo jelentkezo neve: ");
	scanf("%s", name);
	
	FILE *jelentkezok;
	FILE *jelentkezok2;
	jelentkezok=fopen("jelentkezok.txt","r");
	jelentkezok2=fopen("jelentkezok2.txt","w");
	char line[max];
	
	while (fgets(line, sizeof(line), jelentkezok)!=NULL)
	{
		char nev[max];
		int fo;
		char email[max];
		char tel[max];
		char ut[max];
		int ev;
        int ho;
        int nap;
        int ora;
        int perc;
        int sec;
		
		sscanf(line, "%s %d %s %s %s %d-%d-%d %d:%d:%d", nev, &fo, email, tel, ut, &ev, &ho, &nap, &ora, &perc, &sec);
		if (strcmp(name, nev)!=0)
		{
			fputs(line,jelentkezok2);
		}
	}
	
	fclose(jelentkezok);
	fclose(jelentkezok2);
	remove("jelentkezok.txt");
	rename("jelentkezok2.txt", "jelentkezok.txt");
}

void modositas()
{
	char nev[max];
	printf("Modositando jelentkezo neve: ");
	scanf("%s", nev);
	
	FILE * jelentkezok;
	FILE * jelentkezok2;
	jelentkezok=fopen("jelentkezok.txt", "r");
	jelentkezok2=fopen("jelentkezok2.txt", "w");
	char line[max];
	
	while (fgets(line, sizeof(line), jelentkezok)!=NULL)
	{
		char nev2[max];
		int fo;
		char email[max];
		char tel[max];
		char ut[max];
		int ev;
        int ho;
        int nap;
        int ora;
        int perc;
		int sec;
		
		sscanf(line, "%s %d %s %s %s %d-%d-%d %d:%d:%d\n", nev2, &fo, email, tel, ut, &ev, &ho, &nap, &ora, &perc, &sec);
		
		char ujnev[max];
		int ujfo;
		char ujemail[max];
		char ujtel[max];
		char ujut[max];
		printf("%s", nev2);
		
		if (strcmp(nev, nev2)==0)
		{
			char menu[max];
			
			printf("Mit szeretnel modositani?\n");
			printf("Nev: n, Fo: f, email: e, tel: t, utvonal: u\n");
			
			scanf("%s", menu);
			
			if(strcmp(menu,"n")==0)
			{
				printf("Uj nev: ");
				scanf("%s", ujnev);
				fprintf(jelentkezok2, "%s %d %s %s %s %d-%d-%d %d:%d:%d\n", ujnev, fo, email, tel, ut, ev, ho, nap, ora, perc, sec);
			}
			else if(strcmp(menu, "f")==0)
			{
				printf("Hany fo: ");
				scanf("%d", &ujfo);
				fprintf(jelentkezok2, "%s %d %s %s %s %d-%d-%d %d:%d:%d\n", nev, ujfo, email, tel, ut, ev, ho, nap, ora, perc, sec);
			}
			else if(strcmp(menu, "e")==0)
			{
				printf("Uj email: ");
				scanf("%s", ujemail);
				fprintf(jelentkezok2, "%s %d %s %s %s %d-%d-%d %d:%d:%d\n", nev, fo, ujemail, tel, ut, ev, ho, nap, ora, perc, sec);
			}
			else if(strcmp(menu, "t")==0)
			{
				printf("Uj telefonszam: ");
				scanf("%s", ujtel);
				fprintf(jelentkezok2, "%s %d %s %s %s %d-%d-%d %d:%d:%d\n", nev, fo, email, ujtel, ut, ev, ho, nap, ora, perc, sec);
			}
			else if(strcmp(menu, "u")==0)
			{
				printf("Uj utvonal: ");
				scanf("%s", ujut);
				fprintf(jelentkezok2, "%s %d %s %s %s %d-%d-%d %d:%d:%d\n", nev, fo, email, tel, ujut, ev, ho, nap, ora, perc, sec);
			}
			else
			{
				printf("Helytelen menupontot adtal meg.");
				modositas();
			}
		}
		else
		{
			fputs(line, jelentkezok2);
		}
	}
	
	fclose(jelentkezok);
	fclose(jelentkezok2);
	
	copy_file("jelentkezok2.txt","jelentkezok.txt");
	
	//remove("jelentkezok.txt");
	//rename("jelentkezok2.txt", "jelentkezok.txt");
}

void copy_file(const char *from,const char *to)
{
    remove(to);
    FILE *fr;
    FILE *t;
    fr = fopen(from,"r");
    t = fopen(to,"w");

    char line[max];

    char name[max];
    char email[max];
    char phoneno[max];
    char route[max];
	int howmany;
	int ev;
	int ho;
	int nap;
	int ora;
	int perc;
	int sec;

    while(fgets(line,sizeof(line),fr) != NULL)
    {
        sscanf(line,"%s %d %s %s %s %d-%d-%d %d:%d:%d", name, &howmany, email, phoneno, route, &ev, &ho, &nap, &ora, &perc, &sec);
        fprintf(t,"%s %d %s %s %s %d-%d-%d %d:%d:%d\n", name, howmany, email, phoneno, route, ev, ho, nap, ora, perc, sec);
    }

    fclose(fr);
    fclose(t);

   remove(from);
}

void listazas()
{
	FILE *jelentkezok;
	jelentkezok=fopen("jelentkezok.txt", "r");
	char opcio[max];
	printf("Teljes nevsor: n, utvonal szerinti listazas: u\n");
	scanf("%s", opcio);
	if (strcmp(opcio, "n")==0)
	{
		char jelent[max];
		printf("Jelentkezok adatai:\n");
		while (fgets(jelent, max, jelentkezok)!=NULL)
		{
			printf("%s", jelent);
		}
	}
	else if(strcmp(opcio, "u")==0)
	{
		utvonal();
	}
	
	fclose(jelentkezok);
}

void utvonal()
{
	char utv[max];
	
	printf("Melyik utvonal nevsorat szeretned kilistazni (Parlament, Hosok, Var)? ");
	scanf("%s", utv);
	
	FILE *jelentkezok;
	jelentkezok=fopen("jelentkezok.txt","r");
	char line[max];
	
	while (fgets(line, sizeof(line), jelentkezok)!=NULL)
	{
		char nev[max];
		int fo;
		char email[max];
		char tel[max];
		char ut[max];
		char ido[max];
        int ev;
        int ho;
        int nap;
        int ora;
        int perc;
        int sec;
		
		sscanf(line, "%s %d %s %s %s %d-%d-%d %d:%d:%d", nev, &fo, email, tel, ut, &ev, &ho, &nap, &ora, &perc, &sec);
		if(strcmp(utv, ut)==0)
		{
			printf("%s %d %s %s %s %d-%d-%d %d:%d:%d\n", nev, fo, email, tel, ut, ev, ho, nap, ora, perc, sec);
		} 
	}
	
	fclose(jelentkezok);
	
	/*char* jelent[max];
	while(feof(jelentkezok)>0 || feof(jelentkezok)<0)
	{
		fgets(jelent,max,jelentkezok);
		/*char* str[max]=strtok(jelent, " ");
		if (strcmp(utv, jelent[4])==0) // 0-tól 4. //1-től 5.
		{
			printf("%s",jelent);
		}
	}*/
}