#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512
#define CHAR_MERET sizeof(char)

void menu();
void jelentkezo_listazas();
void jelentkezo_beszuras();
void jelentkezo_modositas();
void jelentkezo_torles();
void rendezveny_beszuras();
void rendezveny_torles();
void copyfile();

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
		printf("l - Jelentkezettek listazasa\n");
		printf("b - Jelentkezes letrehozasa\n");
		printf("m - Jelentkezes modositasa\n");
		printf("t - Jelentkezes torlese\n");
		printf("r - Rendezveny felvetele\n");
		printf("f - Rendezveny torlese\n");
		printf("Kilepes:\tPress CTRL + C to exit\n");
		printf("\nNyomj egy gombot: (help: h)\t");
		scanf("%s",menu);
		if( strcmp(menu,"l") == 0 )
		{
			jelentkezo_listazas();
		}
		else if( strcmp(menu,"b") == 0 )
		{
			jelentkezo_beszuras();
		}
		else if( strcmp(menu,"m") == 0 )
		{
			jelentkezo_modositas();
		}
		else if( strcmp(menu,"t") == 0 )
		{
			jelentkezo_torles();
		}
		else if( strcmp(menu,"r") == 0 )
		{
			rendezveny_beszuras();
		}
		else if( strcmp(menu,"f") == 0 )
		{
			rendezveny_torles();
		}
	}	
}

void jelentkezo_listazas()
{
	char* rend = malloc(CHAR_MERET*10);
	printf("Adja meg a rendezveny azonositojat: ");
	scanf("%s", rend);
	FILE *jelentkezok;
	
	jelentkezok = fopen("jelentkezok.txt","r+");
		
	char kiir[MAX_SIZE];
	printf("\nJelentkezok:\n");
	while(fgets(kiir,MAX_SIZE,jelentkezok) != NULL)
	{
		char* r = malloc(CHAR_MERET*10);
		char* nev = malloc(CHAR_MERET*50);
		char* email = malloc(CHAR_MERET*50);
		char* data = malloc(CHAR_MERET*MAX_SIZE);
		sscanf(kiir,"%s %s %s %s",nev, email, r, data);
		if (strcmp(r, rend) == 0) {
			printf(kiir);
		}
		free(r);
		free(data);
		free(nev);
		free(email);
	};
	
	fclose(jelentkezok);
	free(rend);
}

void jelentkezo_beszuras()
{
	FILE *jelentkezok;
	FILE *rendezveny;
	jelentkezok = fopen("jelentkezok.txt","a+");
	rendezveny = fopen("rendezveny.txt","a+");
	
	char* rid = malloc(CHAR_MERET*10);
	char* nev = malloc(CHAR_MERET*50);
	char* mail = malloc(CHAR_MERET*50);
	time_t now = time(0);
	char kiir[MAX_SIZE];
	
	printf("Add meg az adataid:\n");
	printf("Nev:\t");
	scanf("%s",nev);
	printf("E-mail:\t");
	scanf("%s",mail);
	printf("Rendezveny azonositoja:\t");
	scanf("%s",rid);
	int b = 0;
	while(fgets(kiir,MAX_SIZE,rendezveny) != NULL && b == 0)
	{
		char *r = malloc(10*CHAR_MERET);
		r = strtok(kiir, " ");
		if (strcmp(r, rid) == 0) {
			b = 1;
		}
	};
	if (b == 1) {
		int i = 0;
		while(fgets(kiir,MAX_SIZE,jelentkezok) != NULL)
		{
			char *ri = malloc(CHAR_MERET*10);
			char *nev2 = malloc(CHAR_MERET*50);
			char *mail2 = malloc(CHAR_MERET*50);
			nev2 = strtok(kiir, " ");
			mail2 = strtok(kiir, " ");
			ri = strtok(kiir, " ");
			if (strcmp(ri, rid) == 0) { i = i+1; }
		};
		i = i+1;
		printf("Jelentkezesi sorszam: %d \n", i);
		char *nowtext = malloc(CHAR_MERET*MAX_SIZE); 
		nowtext = ctime(&now);
		fprintf(jelentkezok,"%s %s %s %s",nev, mail, rid, nowtext);
		free(nowtext);
		
	}
	else {
		printf("Nem letezo rendezveny\n");
	}

	fclose(jelentkezok);
	fclose(rendezveny);
	free(rid);
	free(nev);
	free(mail);
}

void copy_file(const char *from,const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen(from,"r");
	t = fopen(to,"w");
	char line[MAX_SIZE];
	while(fgets(line,sizeof(line),fr) != NULL)
	{
		fputs(line, t);
	}
	
	fclose(fr);
	fclose(t);
	
	remove(from);
}

void jelentkezo_torles()
{
	char *r = malloc(10*CHAR_MERET);
	int nr;
	printf("Add meg a jelentkezes azonositojat es sorszamat, amelyiket torolni szeretned:\t");
	scanf("%s", r);
	scanf("%d", nr);
	int i = 0;
	
	FILE *jelentkezok;
	FILE *helpfile;
	jelentkezok = fopen("jelentkezok.txt","r+");
	helpfile = fopen("helpfile.txt","w+");
	char line[MAX_SIZE];
	printf("OK\n");
	while(fgets(line,MAX_SIZE,jelentkezok) != NULL)
	{
		char *rid = malloc(10*CHAR_MERET);
		char *nev = malloc(50*CHAR_MERET);
		char *mail = malloc(50*CHAR_MERET);
		char* time = malloc(MAX_SIZE*CHAR_MERET);
		printf("OK\n");
		sscanf(line,"%s %s %s %s",nev, mail, rid, time);
		printf("OK\n");
		if (strcmp(rid, r) == 0) {i = i+1;}
		if( strcmp(rid, r) != 0 || i != nr)
		{
			fputs(line,helpfile);
		}
		free(rid);
		free(nev);
		free(mail);
		free(time);
	}
	
	fclose(jelentkezok);
	fclose(helpfile);
	// free(r);
	copy_file("helpfile.txt","jelentkezok.txt");
}

void jelentkezo_modositas()
{
	char *r = malloc(10*CHAR_MERET);
	int nr;
	printf("Add meg a jelentkezes azonositojat es sorszamat, amelyiket modositani szeretned:\t");
	scanf("%s", r);
	scanf("%d", &nr);
	int i = 0;
	
	FILE *jelentkezes;
	FILE *helpfile;
	jelentkezes = fopen("jelentkezok.txt","r+");
	helpfile = fopen("helpfile.txt","w+");
	char line[MAX_SIZE];
	
	while(fgets(line,MAX_SIZE,jelentkezes) != NULL)
	{
		char *rid = malloc(10*CHAR_MERET);
		char *nev = malloc(50*CHAR_MERET);
		char *mail = malloc(50*CHAR_MERET);
		char* date = malloc(MAX_SIZE*CHAR_MERET);

		sscanf(line,"%s %s %s %s",nev, mail, rid, date);
		if (strcmp(rid, r) == 0) {i = i+1;}
		if( strcmp(rid, r) != 0 || i != nr) {
			fputs(line,helpfile);
		}
		else {
			char *rid = malloc(10*CHAR_MERET);
			char *nev = malloc(50*CHAR_MERET);
			char *mail = malloc(50*CHAR_MERET);
			time_t now = time(0);
			char* timetext = ctime(&now);
			printf("Add meg az adataid:");
			printf("Nev:\t");
			scanf("%s",nev);
			printf("E-mail:\t");
			scanf("%s",mail);
			printf("Rendezveny azonositoja:\t");
			scanf("%s",rid);
			char* data = malloc(MAX_SIZE*CHAR_MERET);
			sprintf(data,"%s %s %s %s\n",nev, mail, rid, timetext);
			fputs(data, helpfile);
			free(rid);
			free(nev);
			free(mail);
			free(timetext);
			free(data);
		}
	}
	
	fclose(jelentkezes);
	fclose(helpfile);
	free(r);
	
	copy_file("helpfile.txt","jelentkezok.txt");	
}
void rendezveny_torles()
{
	char *r = malloc(10*CHAR_MERET);
	printf("Add meg a rendezveny azonositojat, amelyiket torolni szeretned:\t");
	scanf("%s", r);
	printf("%s\n",r);
	FILE *jelentkezok;
	FILE *helpfile;
	jelentkezok = fopen("jelentkezok.txt","r");
	helpfile = fopen("helpfile.txt","w+");
	char line[MAX_SIZE];
	
	while(fgets(line,MAX_SIZE,jelentkezok) != NULL)
	{
		char *rid = malloc(10*CHAR_MERET);
		char *nev = malloc(50*CHAR_MERET);
		char *mail = malloc(50*CHAR_MERET);
		char* time = malloc(MAX_SIZE*CHAR_MERET);
		sscanf(line,"%s %s %s %s",nev, mail, rid, time);
		printf("%s\n", rid);
		if( strcmp(rid, r) != 0)
		{
			fputs(line,helpfile);
		}
		free(rid);
		free(nev);
		free(mail);
		free(time);
	}
	
	fclose(jelentkezok);
	fclose(helpfile);
	
	copy_file("helpfile.txt","jelentkezok.txt");

	FILE *rendezveny;
	FILE *helpf;
	rendezveny = fopen("rendezveny.txt","r");
	helpf = fopen("helpfile.txt","w+");
	while(fgets(line,sizeof(line),rendezveny) != NULL)
	{
		char *rid = malloc(10*CHAR_MERET);
		char *nev = malloc(50*CHAR_MERET);
		sscanf(line,"%s %s", nev, rid);
		if( strcmp(rid, r) != 0)
		{
			fputs(line,helpf);
		}
		free(rid);
		free(nev);
	}

	fclose(rendezveny);
	fclose(helpf);
	copy_file("helpfile.txt","rendezveny.txt");
	free(r);	
}

void rendezveny_beszuras()
{
	FILE *rendezveny;
	rendezveny = fopen("rendezveny.txt","r+");
	
	char* rid = malloc(CHAR_MERET*10);
	char* nev = malloc(CHAR_MERET*50);
	char kiir[MAX_SIZE];
	
	printf("Add meg a rendezveny nevet:\n");
	scanf("%s",nev);
	printf("Add meg a rendezveny azonositojat:\n");
	scanf("%s",rid);

	int b = 0;
	while(fgets(kiir,MAX_SIZE,rendezveny) != NULL)
	{
		char *r = malloc(CHAR_MERET*10);
		char *nev2 = malloc(CHAR_MERET*50);
		nev2 = strtok(kiir, " ");
		r = strtok(kiir, " ");
		if (strcmp(r, rid) == 0) { b = 1; }
		free(r);
		free(nev2);
	};
	fprintf(rendezveny,"%s %s",nev,rid);
	fclose(rendezveny);
	free(rid);
	free(nev);
}