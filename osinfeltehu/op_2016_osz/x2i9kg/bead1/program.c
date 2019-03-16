/*
Egy kiállításokat, rendezvényeket szervezo társaság a "King of Stands",
elektronikus látogatói nyilvántartást készített.
A látogatók a nyilvántartásba jelentkezéskor megadják nevüket,
mail címüket, a rendezvény azonosító számát!
Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja,
hogy hányadik vendégként történt meg a regisztráció!
Az alkalmazás adjon lehetoséget vendég jelentkezéshez,
a jelentkezettek listázásához, egy vendég adatainak módosításához,
törléséhez.
Legyen lehetoségünk egy új rendezvény indításához,
egy rendezvény összes adatának törléséhez!
A jelentkezoknél a korábbi adatok mellett rögzítsük automatikusan
a jelentkezési idot(rendszerido) is(ezt nem kell bekérni)!
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512

void menu();
void listazas();
void latogato_beszuras();
void latogato_modositas();
void latogato_torles();
void rendezveny_beszuras();
void rendezveny_torles();
void copy_file(const char *from,const char *to,bool l);

int main(int argc,char *argv[])
{
    printf("King of Stands\n");
    menu();
	
    return 0;
}

void menu()
{
	char menu[MAX_SIZE];	
	while(1 < 2)
	{
		printf("\nNyomj egy gombot: (help: h)\t");
		scanf("%s",menu);
		
		if( strcmp(menu,"h") == 0 )
		{
			printf("LISTAZAS:\t\tl\n");
			printf("LATOGATO BESZURAS:\tb\n");
			printf("LATOGATO MODOSITAS:\tm\n");
			printf("LATOGATO TORLES:\tt\n");
			printf("RENDEZVENY BESZURAS:\ti\n");
			printf("RENDEZVENY TORLES:\td\n");
			printf("KILEPES:\tPress CTRL + C to exit\n");
		}
		if( strcmp(menu,"l") == 0 )
		{
			listazas();
		}
		else if( strcmp(menu,"b") == 0 )
		{
			latogato_beszuras();
		}
		else if( strcmp(menu,"m") == 0 )
		{
			latogato_modositas();
		}
		else if( strcmp(menu,"t") == 0 )
		{
			latogato_torles();
		}
		else if( strcmp(menu,"i") == 0 )
		{
			rendezveny_beszuras();
		}
		else if( strcmp(menu,"d") == 0 )
		{
			rendezveny_torles();
		}
	}	
}

void listazas()		//LISTÁZÁS
{
	FILE *latogatok;
	FILE *rid;
	
	latogatok = fopen("latogatok.txt","r");
	rid = fopen("rid.txt","r");
		
	char kiir[MAX_SIZE];
	printf("\nLATOGATOK ADATAI:\n");
	printf("RID\tNEV\tEMAIL\tIDO\n");
	while(fgets(kiir,100,latogatok) != NULL)
	{
		printf("%s",kiir);
	};
	
	printf("\n\nRENDEZVENYEK:\n");
	printf("RID\tNEV\n");
	while(fgets(kiir,100,rid) != NULL)
	{
		printf("%s",kiir);
	};
	
	fclose(latogatok);
	fclose(rid);
}

void latogato_beszuras()		//LÁTOGATÓ BESZÚRÁS
{
	FILE *latogatok;
	latogatok = fopen("latogatok.txt","r");
	
	int count = 0;
	char line[MAX_SIZE];
	int rid;	//rendezvény  ID-je
	char nev[MAX_SIZE];	//látogató neve
	char cim[MAX_SIZE];	//látogató email címe
	char ido[MAX_SIZE];	//látogató regisztrálási ideje

	while(fgets(line,sizeof(line),latogatok) != NULL)
	{
		sscanf(line,"%d %s %s %s",&rid,nev,cim,ido);
		if (rid != 0 && strcmp(nev,"")!=0 && strcmp(cim,"")!=0 && strcmp(ido,"")!=0) {
			++count;
		}		
	}
	
	fclose(latogatok);

	latogatok = fopen("latogatok.txt","a+");
	
	printf("Add meg a latogato  adatait:");
	printf("\nNeve:\t");
	scanf("%s",nev);
	printf("Email cime:\t");
	scanf("%s",cim);
	printf("Rendezveny id-je:\t");
	scanf("%d",&rid);

	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(ido, 26, "%Y/%m/%d-%H:%M:%S", timeinfo);
	printf("\nIdo:\t%s", ido);
	
	if (count == 0)
	{
		fprintf(latogatok,"%d %s %s %s",rid,nev,cim,ido);
	}
	else
	{
		fprintf(latogatok,"\n%d %s %s %s",rid,nev,cim,ido);
	}
	
	fclose(latogatok);
	
	printf("\n\nAz uj latogato sorszama: %d", ++count);
}

void latogato_modositas()		//LÁTOGATÓ MÓDOSÍTÁS
{
	char nev[MAX_SIZE];
	printf("Add meg annak a latogatonak a nevet, amelyiket modositani szeretned:\t");
	scanf("%s",nev);
	
	FILE *latogato;
	FILE *helpfile;
	latogato = fopen("latogatok.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),latogato) != NULL)
	{
		int id;
		char old_nev[MAX_SIZE];
		char cim[MAX_SIZE];
		char ido[MAX_SIZE];
		sscanf(line,"%d %s %s %s",&id,old_nev,cim,ido);
		
		//Bekért uj adatok
		int newID;
		char newNEV[MAX_SIZE];
		char newCIM[MAX_SIZE];
		
		if( strcmp(old_nev,nev) == 0 )
		{
			char menu[MAX_SIZE];
			
			printf("Mit szeretnel modositani?\n");
			printf("Rendezveny: (r)\n");
			printf("Nev: (n)\n");
			printf("Email cim: (e)\n");
			
			scanf("%s",menu);
			
			if( strcmp(menu,"r") == 0 )
			{
				printf("Add meg az uj rendezveny  ID-jet:\t");
				scanf("%d",&newID);
				fprintf(helpfile,"%d %s %s %s\n",newID,old_nev,cim,ido);
			}
			else if( strcmp(menu,"n") == 0 )
			{
				printf("Add meg az uj nevet:\t");
				scanf("%s",newNEV);
				fprintf(helpfile,"%d %s %s %s\n",id,newNEV,cim,ido);
			}
			else if( strcmp(menu,"e") == 0 )
			{
				printf("Add meg az uj cimet:\t");
				scanf("%s",newCIM);
				fprintf(helpfile,"%d %s %s %s\n",id,old_nev,newCIM,ido);
			}
			else
			{
				latogato_modositas();
			}
		}
		else
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(latogato);
	fclose(helpfile);
	
	copy_file("helpfile.txt","latogatok.txt",false);	
}

void latogato_torles()		//LÁTOGATÓ TÖRLÉS
{
	char keresett_nev[MAX_SIZE];
	printf("Add meg annak a latogatonak a nevet, amelyiket torolni szeretned:\t");
	scanf("%s",keresett_nev);
	
	FILE *latogato;
	FILE *helpfile;
	latogato = fopen("latogatok.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),latogato) != NULL)
	{
		int id;
		char nev[MAX_SIZE];
		sscanf(line,"%d %s",&id,nev);
		if( strcmp(nev,keresett_nev) != 0 )
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(latogato);
	fclose(helpfile);
	
	copy_file("helpfile.txt","latogatok.txt",false);
}

void rendezveny_beszuras()		//RENDEZVÉNY BESZÚRÁS
{
	FILE *rendezveny;
	rendezveny = fopen("rid.txt","a+");
	
	int rid;	//rendezvény  ID-je
	char nev[MAX_SIZE];	//rendezvény neve
	
	int count = 0;
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),rendezveny) != NULL)
	{
		sscanf(line,"%d %s",&rid,nev);
		if (rid != 0 && strcmp(nev,"")!=0) {
			++count;
		}
	}
	
	printf("Add meg a rendezveny adatait:");
	printf("\nID:\t");
	scanf("%d",&rid);
	printf("\nNev:\t");
	scanf("%s",nev);
	
	if (count > 0) {
		fprintf(rendezveny,"\n%d %s",rid,nev);
	} else {
		fprintf(rendezveny,"%d %s",rid,nev);
	}
	
	
	fclose(rendezveny);
}

void rendezveny_torles()		//RENDEZVÉNY TÖRLÉS
{
	// Rendezvény törlése a rendezvények listájából
	int id;
	printf("Add meg annak a rendezvenynek az ID-jet, amelyiket torolni szeretned:\t");
	scanf("%d",&id);
	
	FILE *rendezveny;
	FILE *helpfile;
	rendezveny = fopen("rid.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	char nev[MAX_SIZE];
	int rid;
	
	while(fgets(line,sizeof(line),rendezveny) != NULL)
	{
		sscanf(line,"%d %s",&rid,nev);
		if( rid != id )
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(rendezveny);
	fclose(helpfile);
	
	copy_file("helpfile.txt","rid.txt",true);

	// Azon látogatók törlése, melyek a törlendö rendezvényre jelentkeztek
	FILE *latogato;
	latogato = fopen("latogatok.txt","r");
	helpfile = fopen("helpfile.txt","w");
	
	char line2[MAX_SIZE];
	char nev2[MAX_SIZE];
	char cim[MAX_SIZE];
	char ido[MAX_SIZE];
	int rid2;
	while(fgets(line2,sizeof(line2),latogato) != NULL)
	{
		sscanf(line2,"%d %s %s %s",&rid2,nev2,cim,ido);
		if( rid2 != id )
		{
			fputs(line2,helpfile);
		}
	}
	
	fclose(latogato);
	fclose(helpfile);
	
	copy_file("helpfile.txt","latogatok.txt",false);
}

void copy_file(const char *from,const char *to,bool l) //Segéd függvény - l = rendezvény fájl-e?
{
	FILE *fr;
	FILE *t;
	fr = fopen(from,"r");
	t = fopen(to,"w");
	
	int count = 0;
	char line[MAX_SIZE];
	int rid;
	char nev[MAX_SIZE];
	char cim[MAX_SIZE];
	char ido[MAX_SIZE];
	while(fgets(line,sizeof(line),fr) != NULL)
	{
		if (l)
		{
			sscanf(line,"%d %s",&rid,nev);
			if (count == 0)
			{
				fprintf(t,"%d %s",rid,nev);
			}
			else
			{
				fprintf(t,"\n%d %s",rid,nev);
			}
		}
		else
		{
			sscanf(line,"%d %s %s %s",&rid,nev,cim,ido);
			if (count == 0)
			{
				fprintf(t,"%d %s %s %s",rid,nev,cim,ido);
			}
			else
			{
				fprintf(t,"\n%d %s %s %s",rid,nev,cim,ido);
			}
		}
		++count;
	}
	
	fclose(fr);
	fclose(t);
	
	remove("helpfile.txt");
}
