#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512
#define REND_FORMAT "%d %s\n"
#define VEND_FORMAT "%s %s %d %d \n"
#define REND_FILE "rendezveny.txt"
#define VEND_FILE "vendeg.txt"
#define TMP_FILE "tmp.txt"

struct Vendeg {
   char  	nev[50];
   char  	email[50];
   int  	rend_id;
   time_t  	j_ido;
};
typedef struct Vendeg VendegS ;

struct Rendezveny {
   char  	nev[50];
   int  	id;
};

void menu();

void listazas(); //listázás
void jelentkezes(); //jelentkezés
void modositas(); //módosítás
void torles(); //törlés

void rendezveny_letrehozas();
void rendezveny_torles();

void kiir_rend();
void beolvas_rend();
void kiir_vendeg();
void beolvas_vendeg();
void beker_vendeg(VendegS*);

struct Vendeg vendegek[MAX_SIZE];
struct Rendezveny rendezvenyek[MAX_SIZE];

int vendegek_szama = 0;
int rendezvenyek_szama = 0;

int main(int argc,char *argv[])
{
	beolvas_rend();
	beolvas_vendeg();

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
			printf("Vendegek listazasa:\t1\n");
			printf("Vendeg jelentkezese:\t2\n");
			printf("Vendeg adatainak modositasa:\t3\n");
			printf("Vendeg torlese:\t\t4\n");
			printf("Rendezveny letrehozasa:\t5\n");
			printf("Rendezveny torlese:\t6\n");
			printf("Kilepes:\tPress CTRL + C to exit\n");
		}
		else if( strcmp(menu,"1") == 0 )
		{
			listazas();
		}
		else if( strcmp(menu,"2") == 0 )
		{
			jelentkezes();
		}
		else if( strcmp(menu,"3") == 0 )
		{
			modositas();
		}
		else if( strcmp(menu,"4") == 0 )
		{
			torles();
		}
		else if( strcmp(menu,"5") == 0 )
		{
			rendezveny_letrehozas();
		}
		else if( strcmp(menu,"6") == 0 )
		{
			rendezveny_torles();
		}
	}	
}

void rendezveny_letrehozas()
{
	struct Rendezveny r;

	printf("Add meg a rendezveny adatait:");
	printf("\nNev:\t");
	scanf("%s", &r.nev);
	printf("\nAzonosito:\t");
	scanf("%d", &r.id);

	rendezvenyek[rendezvenyek_szama++] = r;

	kiir_rend();

	printf("Rendezveny mentve: %s - %d \n", r.nev, r.id);
}


void rendezveny_torles()
{
	char nev[MAX_SIZE];
	printf("Add meg annak az esemenynek a nevet, amelyiket torolni szeretned:\t");
	scanf("%s",nev);

	int siker = 1;

	for (int i = 0; i < rendezvenyek_szama; i++)
	{
		if( strcmp(rendezvenyek[i].nev, nev) == 0 )
		{
			for (int k = i; k < rendezvenyek_szama; k++){   
    			rendezvenyek[k] = rendezvenyek[k+1];
			}

			rendezvenyek_szama--;
			siker = 0;
		}
	}

	if (siker == 0) {
		kiir_rend();
	}
}

void listazas()		//LISTÁZÁS
{
	for (int i = 0; i < vendegek_szama; i++)
	{
		printf(VEND_FORMAT, vendegek[i].nev, vendegek[i].email, vendegek[i].rend_id, vendegek[i].j_ido);
	}
}

void beker_vendeg(VendegS* v)
{
	printf("Add meg a vendeg adatait:");
	printf("\nNev:\t");
	scanf("%s", v->nev);
	printf("\nE-mail:\t");
	scanf("%s", v->email);
	printf("\nRendezveny azonositoja:\t");
	scanf("%d", &(v->rend_id));

	printf("Vendeg beolvasva\n");
}

void jelentkezes()		//BESZÚRÁS
{
	struct Vendeg v;

	beker_vendeg(&v);

	v.j_ido = time(NULL);

	vendegek[vendegek_szama++] = v;

	int num = 0;
	for (int i = 0; i < vendegek_szama; i++)
	{
		if(vendegek[i].rend_id == v.rend_id) {
			num++;
		}
	}

	kiir_vendeg();

	printf("Vendeg mentve: %s. Az esemenyre jelentkezettek szama: %d\n", v.nev, num);
}

void torles()		//TÖRLÉS
{
	
	char nev[MAX_SIZE];
	printf("Add meg annak a vendegnek a nevet, akit torolni szeretnel:\t");
	scanf("%s",nev);

	int siker = 1;

	for (int i = 0; i < vendegek_szama; i++)
	{
		if( strcmp(vendegek[i].nev, nev) == 0 )
		{
			for (int k = i; k < vendegek_szama; k++){   
    			vendegek[k] = vendegek[k+1];
			}

			vendegek_szama--;
			siker = 0;
		}
	}

	if (siker == 0) {
		kiir_vendeg();
	}
}

void modositas()		//MÓDOSÍTÁS
{
	char nev[MAX_SIZE];
	printf("Add meg annak a vendegnek a nevet, akinek az adatait modositani szeretned:\t");
	scanf("%s",nev);

	for (int i = 0; i < vendegek_szama; i++)
	{
		printf("%d\n", i);
		if( strcmp(vendegek[i].nev, nev) == 0 )
		{
			printf("%s %s %d\n", vendegek[i].nev, vendegek[i].email, vendegek[i].rend_id);

			beker_vendeg(&vendegek[i]);
		}
		printf("%d\n", i);
	}

	kiir_vendeg();
}

//private fos
void kiir_rend()
{
	FILE *tmp;
	tmp = fopen(TMP_FILE,"w");

	for (int i = 0; i < rendezvenyek_szama; i++)
	{
		fprintf(tmp, REND_FORMAT, rendezvenyek[i].id, rendezvenyek[i].nev);
	}

	fclose(tmp);

	remove(REND_FILE);
	rename(TMP_FILE, REND_FILE);
}

void beolvas_rend()
{
	FILE *f;
	f = fopen(REND_FILE,"r");
	char line[MAX_SIZE];

	while(fgets(line,sizeof(line),f) != NULL)
	{
		struct Rendezveny r;

		sscanf(line, REND_FORMAT, &r.id, &r.nev);

		rendezvenyek[rendezvenyek_szama++] = r;
	}

	fclose(f);
}

void kiir_vendeg()
{
	FILE *tmp;
	tmp = fopen(TMP_FILE,"w");

	for (int i = 0; i < vendegek_szama; i++)
	{
		fprintf(tmp, VEND_FORMAT, vendegek[i].nev, vendegek[i].email, vendegek[i].rend_id, vendegek[i].j_ido);
	}

	fclose(tmp);

	remove(VEND_FILE);
	rename(TMP_FILE, VEND_FILE);
}

void beolvas_vendeg()
{
	FILE *f;
	f = fopen(VEND_FILE,"r");
	char line[MAX_SIZE];

	while(fgets(line,sizeof(line),f) != NULL)
	{
		struct Vendeg v;

		sscanf(line, VEND_FORMAT, &v.nev, &v.email, &v.rend_id, &v.j_ido);

		vendegek[vendegek_szama++] = v;
	}

	fclose(f);
}