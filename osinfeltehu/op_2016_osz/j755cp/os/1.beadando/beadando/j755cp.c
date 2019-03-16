#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct vendeg
{
	char vnev[50];
	char mailcim[50];
	int rendezveny;
	time_t idopont;
	struct vendeg *next;
};

struct rendezveny
{
	int azon;
	char rnev[50];
	struct rendezveny *next;
};

struct vendeg *vendegek = 0; 
struct rendezveny *rendezvenyek = 0;

void addrendezveny(int azon, char rnev[])
{
	struct rendezveny *rend;
	struct rendezveny *seged = rendezvenyek;
	rend = malloc( sizeof(struct rendezveny) );
	rend->azon = azon;
	int i;
	for (i = 0; i < 50; i++) rend->rnev[i] = rnev[i];
	rend->next=NULL;
	if (seged !=0)
	{
		while ((seged->next != 0))
		{
			seged = seged->next;
		}
		seged->next = rend;
	}
	else rendezvenyek = rend;
}

//Uj rendezveny
void ujrendezveny()
{
	char rnev[50];
	int azon;
	printf("\n Uj rendezveny inditasa:\n");
	printf("Add meg a rendezveny azonosito szamat:");
	scanf("%d", &azon);
	getchar();
	printf("Add meg a rendezveny nevet:");
	scanf("%[^\n]s", &rnev);
	getchar();
	addrendezveny(azon, rnev);
	
	printf("\nA bejegyzes beszurva!\n");
}

int van_e_rendezveny(int razon)
{
	int l=0;
	struct rendezveny *seged = rendezvenyek;
	struct rendezveny *elozo = 0;
		
	while (seged != 0 && seged->azon != razon && l!=1)
	{
		if(seged->azon == razon) l=1;
		elozo = seged;
		seged = seged->next;
	}
}

void addvendeg(char vnev[50], char mailcim[50], int azon, time_t idopont)
{
	struct vendeg *vend;
	struct vendeg *seged = vendegek;
	vend = malloc( sizeof(struct vendeg) );
	int i;
	for (i = 0; i < 50; i++) vend->vnev[i] = vnev[i];
	for (i = 0; i < 50; i++) vend->mailcim[i] = mailcim[i];
	vend->rendezveny = azon;
	vend->idopont=idopont;
	vend->next=NULL;
		
	if (seged !=0)
	{
		while ((seged->next != 0))
		{
			seged = seged->next;
		}
		seged->next = vend;
	}
	else vendegek=vend;  
}

//Uj vendeg	
void ujvendeg()
{
	int i;
	char vnev[50];
	char mailcim[50]; 
	int azon;
	time_t idopont;
	struct vendeg *seged = vendegek;
	int sorsz=1;
	
	printf("\n Uj vendeg jelentkezese:\n");
	printf("Add meg a vendeg nevet:");
	scanf("%[^\n]s", &vnev);
	getchar();
	printf("Add meg a vendeg e-mail cimet:");
	scanf("%[^\n]s", &mailcim);
	getchar();
	printf("Add meg a rendezveny azonosito szamat:");
	scanf("%d", &azon);
	getchar();
	idopont=time(NULL);
	
	if (seged !=0)
	{
		sorsz=2;
		while ((seged->next != 0))
		{
				seged = seged->next;
				sorsz=sorsz+1;
		}
	}
	addvendeg(vnev, mailcim, azon, idopont);
	
	printf("\nA %d. sorszamu vendeg beszurva!\n", sorsz);
	
	if(van_e_rendezveny(azon)==0)
	{
		char rnev[50];
		printf("\nEz a rendezvenyazonosito meg nincs rogzitve!:\n");
		printf("Add meg a rendezveny nevet:");
		scanf("%[^\n]s", &rnev);
		getchar();
		addrendezveny(azon, rnev);
	
	printf("\nA bejegyzes beszurva!\n");
	}
}



//Vendégek listázása:
void vendeg_listazas()
{
	printf("\nAz eddig jelentkezett vendegek:\n\n");
	struct vendeg *seged = vendegek;
	int i=1;
		
	if (seged != 0)
	{
		do 
		{
			printf("\n%d. ", i);
			printf(" Nev: %s", seged->vnev);
			printf(", Mailcim: %s", seged->mailcim);
			printf(", Rendezveny: %d", seged->rendezveny);
			printf(", Datum: %s", ctime(&seged->idopont));
			seged = seged->next;
			i++;
		}
		while (seged != 0);
	}
	else printf("Ures lista \n");
	
}

//Rendezvények listázása:
void rendezveny_listazas()
{
	printf("\nA rendezvények:\n\n");
	struct rendezveny *seged = rendezvenyek;
		
	if (seged != 0)
	{
		do 
		{
			printf("\nAzonosito: %d ", seged->azon);
			printf(" Nev: %s", seged->rnev);
			seged = seged->next;
		}
		while (seged != 0);
	}
	else printf("Ures lista \n");
	
}

//Vendég adatainak módosítása:
void vendeg_modositas()
{
	char vnev[50];
	char mailcim[50]; 
	int rendezveny;
	int kit;
	vendeg_listazas();
	printf("\nAdd meg, melyik vendeg adatat szeretned modositani! ");
	scanf("%d", &kit);
	getchar();
	printf("\nMost add meg az uj parametereket:\n");
	printf("Add meg a vendeg nevet:");
	scanf("%[^\n]s", &vnev);
	getchar();
	printf("Add meg a vendeg e-mail cimet:");
	scanf("%[^\n]s", &mailcim);
	getchar();
	printf("Add meg a rendezveny azonosito szamat:");
	scanf("%d", &rendezveny);
	getchar();
	
	struct vendeg *seged = vendegek;
		
	int i = 1;
			while (seged != 0 && i != kit)
			{
				seged = seged->next;
				i++;
			}
			if (seged == 0) printf("Nincs ilyen sorszamu elem a listaban!\n");
			else
			{
				for (i = 0; i < 50; i++) seged->vnev[i] = vnev[i];
				for (i = 0; i < 50; i++) seged->mailcim[i] = mailcim[i];
				seged->rendezveny = rendezveny;
			}
			
	if(van_e_rendezveny(rendezveny)==0)
	{
		char rnev[50];
		printf("\nEz a rendezvenyazonosito meg nincs rogzitve!:\n");
		printf("Add meg a rendezveny nevet:");
		scanf("%[^\n]s", &rnev);
		getchar();
		addrendezveny(rendezveny, rnev);
	}
	
	printf("\nA bejegyzes modositva!\n");
}
	
//Vendég adatainak törlése:
void vendeg_torles()
{
	int kit;
	vendeg_listazas();
	printf("\nAdd meg, melyik vendeg adatat szeretned torolni!");
	scanf("%d", &kit);
	getchar();
	
	struct vendeg *seged = vendegek;
	struct vendeg *elozo = 0;
		
	int i = 1;
			while (seged != 0 && i != kit)
			{
				elozo = seged;
				seged = seged->next;
				i++;
			}
			if (seged == 0) printf("Nincs ilyen sorszamu elem a listaban!\n");
			else
			{
				if (elozo == 0) vendegek = seged->next;
				else elozo->next = seged->next;
				free(seged);
				printf("\nA bejegyzes torolve!\n");
			}
	

}

//Rendezvény és kapcsolódó vendégek törlése
void rendezveny_torles()
{
	int melyiket;
	rendezveny_listazas();
	printf("\nAdd meg, hanyas azonositoju rendezveny adatat szeretned torolni!");
	scanf("%d", &melyiket);
	getchar();
	
	struct rendezveny *seged = rendezvenyek;
	struct rendezveny *elozo = 0;
	struct vendeg *seged2 = vendegek;
	struct vendeg *elozo2 = 0;
		
	while (seged != 0 && seged->azon != melyiket)
	{
		elozo = seged;
		seged = seged->next;
	}
	if (seged == 0) printf("Nincs ilyen sorszamu elem a listaban!\n");
	else
	{
		if (elozo == 0) rendezvenyek = seged->next;
		else elozo->next = seged->next;
		free(seged);
		while (seged2 != 0)
		{
			if (seged2->rendezveny == melyiket)
			{
				if (elozo2 == 0)
				{
					vendegek = seged2->next;
					free(seged2);
					seged2 = vendegek;
				}
				else
				{
					elozo2->next = seged2->next;
					free(seged2);
					seged2=elozo2->next;
				
				}
			}
			else
			{
				elozo2 = seged2;
				seged2 = seged2->next;
			}
		}
		printf("\nA rendezveny es a kapcsolodo vendegek torolve!\n");
	}
	
}

//Adatok betöltése
void betoltes() 
{
	if( access("vendeg.bin", 0 ) != -1 )
	{
		FILE *vendegfile;
		vendegfile = fopen("vendeg.bin", "r");
		if (vendegfile != NULL)
		{
			struct vendeg seged;
			while (fread(&seged, sizeof(seged), 1, vendegfile))
			{
				addvendeg(seged.vnev, seged.mailcim, seged.rendezveny, seged.idopont);

			}
		}
		fclose(vendegfile);
	}


	if( access("rendezveny.bin", 0 ) != -1 )
	{
		FILE *rendezvenyfile;
		rendezvenyfile = fopen("rendezveny.bin", "r");
		if (rendezvenyfile != NULL)
		{
			struct rendezveny seged2;
			while (fread(&seged2, sizeof(seged2), 1, rendezvenyfile))
			{
				addrendezveny(seged2.azon, seged2.rnev);
			}
		}
		fclose(rendezvenyfile);
	}

}

//Adatok kimentése
void mentes()
{
	FILE *vendegfile;
	vendegfile = fopen("vendeg.bin", "w");
	struct vendeg *seged = vendegek;
	while (seged != 0)
	{
		fwrite(seged, sizeof(*seged), 1, vendegfile);
		seged = seged->next;
	}
	fclose(vendegfile);
	
	FILE *rendezvenyfile;
	rendezvenyfile = fopen("rendezveny.bin", "w");
	struct rendezveny *seged2 = rendezvenyek;
	while (seged2 != 0)
	{
		fwrite(seged2, sizeof(*seged2), 1, rendezvenyfile);
		seged2 = seged2->next;
	}
	fclose(rendezvenyfile);
}

int main()
{
	betoltes();
	printf("\nKing of Stands latogatoi nyilvantartas\n");
	int vege;	
	do {
		printf("\n---------------\nMenu\n\n");
		printf("1. Uj vendeg jelentkezese\n");
		printf("2. Vendegek listazasa\n");
		printf("3. Vendeg adatainak modositasa\n");
		printf("4. Vendeg torlese\n");
		printf("5. Uj rendezveny inditasa\n");
		printf("6. Rendezvenyek listazasa\n");
		printf("7. Rendezveny adatainak torlese\n");		
		printf("8. Kilepes\n");
		printf("\n---------------\n");
		scanf("%d", &vege);
		getchar();
		switch (vege) {
			case 1:
				//Jelentkezes
				ujvendeg();
			break;
			case 2:
				//Listazas
				vendeg_listazas();
			break;
			case 3:
				//Modositas
				vendeg_modositas();
			break;
			case 4:
				//Torles
				vendeg_torles();
			break;
			case 5:
				//Uj rendezveny
				ujrendezveny();
			break;
			case 6:
				//Rendezvenyek listazasa
				rendezveny_listazas();
			break;
			case 7:
				//Rendezveny torles
				rendezveny_torles();
			break;
			case 8:
			break;
			default:
				printf("\n\nNem letezo menupont!\n");
			break;
		}
	}
	while (vege != 8);
	mentes();
	return 0;
}