#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <time.h>
#include <string.h>

#define VENDEG "vendeg.txt"
#define RENDEZVENY "rendezveny.txt"
#define MAX_ID "max_id.txt"

struct vendeg{
	int  id;
	char nev[50];
	char email[50];
	int  rendezveny;
	char reg_dt[30];
};

struct rendezveny{
	int  azonosito;
	char nev[30];
	char idopont[10];
	char helyszin[30];
	char datum[20];
};


void kiir_vendeg( struct vendeg a)
{
	printf( "Regisztráció azonosító: %d, nev: %s, email: %s, rendezvény azonosító: %d, regisztráció időpontja: %s\n", a.id, a.nev, a.email, a.rendezveny, a.reg_dt );
}

void kiir_rendezveny( struct rendezveny a)
{
	printf( "Rendezvény azonosító: %d, név: %s, helyszín(város): %s, dátum: %s, időpont: %s \n", a.azonosito, a.nev, a.helyszin, a.datum, a.idopont);
}

int next_azon (const char* file_name)
{
	FILE* file = fopen (file_name, "r+");
	int i = 0;

	if (file == NULL)
	{
		i = 0;
	}
	else
	{
		fscanf (file, "%d\n", &i);
		fclose (file);
	}
	return i+1;
}

void refresh_azon (const char* file_name, int new_azon)
{
	int i;
	FILE* file = fopen (file_name, "w+");
	if (file == NULL)
	{
		i = -1;
	}
	else
	{
		fprintf(file, "%d", new_azon);
		fclose(file);
		i = 0;
	}
}


int elemszam(const char* file_name)
{
	int i;
	i = -1;
	FILE* file = fopen (file_name, "r+");
	if (file == NULL)
	{
		i = -1;
	}
	else
	{
		i = 0;
		if (file_name == VENDEG)
		{
			struct vendeg input;
			while (fread (&input, sizeof(struct vendeg), 1, file))
				i = i+1;
		}

		if (file_name == RENDEZVENY)
		{
			struct rendezveny input;
			while (fread (&input, sizeof(struct rendezveny), 1, file))
				i = i+1;
		}

		fclose(file);
	}
	return i;
}

int lista(const char* file_name, int v_azonosito)
{
	FILE *infile;
	int i;
	i=0;
	infile = fopen (file_name,"r+");

	if (infile == NULL)
	{
		i = -1;
	}
	else
	{
		if (file_name == VENDEG)
		{
			struct vendeg input;
			while (fread (&input, sizeof(struct vendeg), 1, infile))
			{
				if (input.rendezveny == v_azonosito || v_azonosito == -1)
				{
					kiir_vendeg(input);
					i=i+1;
				}
			}
		}

		if (file_name == RENDEZVENY)
		{
			struct rendezveny input;
			while (fread (&input, sizeof(struct rendezveny), 1, infile))
			{
				kiir_rendezveny(input);
				i=i+1;
			}
		}

		fclose(infile);
	}
	return i;
}

int letezo_azonosito(const char* file_name, int v_azonosito)
{
	FILE *infile;
	int i;
	i=0;
	infile = fopen (file_name,"r+");

	if (infile == NULL)
	{
		i = -1;
	}
	else
	{
		if (file_name == VENDEG)
		{
			struct vendeg input;
			while (fread (&input, sizeof(struct vendeg), 1, infile))
			{
				if (input.id == v_azonosito)
				{
					i=i+1;
				}
			}
		}

		if (file_name == RENDEZVENY)
		{
			struct rendezveny input;
			while (fread (&input, sizeof(struct rendezveny), 1, infile))
			{
				if (input.azonosito == v_azonosito)
				{
					i=i+1;
				}
			}
		}

		fclose(infile);
	}
	return i;
}

struct vendeg get_adatok(const char* file_name, int v_azonosito)
{
	FILE *infile;
	int i;
	struct vendeg v;
	infile = fopen (file_name,"r+");

	if (infile == NULL)
	{
		i = -1;
	}
	else
	{
		if (file_name == VENDEG)
		{
			struct vendeg input;
			while (fread (&input, sizeof(struct vendeg), 1, infile))
			{
				if (input.id == v_azonosito)
				{
					v.id = input.id;
					strcpy(v.nev,input.nev);
					strcpy(v.email,input.email);
					v.rendezveny = input.rendezveny;
					strcpy(v.reg_dt,input.reg_dt);
				}
			}
		}

		fclose(infile);
	}
	return v;
}

void fomenu()
{
printf("\nKérem válasszon az alábbi menüpontok közül:\n");
printf("[1]Jelentkezés rendezvényre\n");
printf("[2]Jelentkezettek listázása\n");
printf("[3]Vendég adatmódosítása\n");
printf("[4]Vendég törlése\n");
printf("[5]Új rendezvény felvétele\n");
printf("[6]Rendezvény listázása\n");
printf("[7]Rendezvény törlése\n");
printf("[0]Kilépés\n");
} 

void modosit( const char* file_name, struct vendeg a, int regisztracio)
{
	FILE *outfile;
	FILE *infile;
	struct vendeg input;
	int tomb_meret, i, j;
	i = 0;
	tomb_meret = elemszam(file_name);
	struct vendeg adat[tomb_meret];
	
	infile = fopen (file_name,"r");
	if (infile == NULL)
	{
		i = -1;
	}
	else
	{
		while (fread (&input, sizeof(struct vendeg), 1, infile))
		{
			if (input.id == regisztracio)
			{
			adat[i].id = input.id;
			strcpy(adat[i].nev,a.nev);
			strcpy(adat[i].email,a.email);
			adat[i].rendezveny = a.rendezveny;
			strcpy(adat[i].reg_dt,input.reg_dt);
			}
			else
			{
			adat[i].id = input.id;
			strcpy(adat[i].nev,input.nev);
			strcpy(adat[i].email,input.email);
			adat[i].rendezveny = input.rendezveny;
			strcpy(adat[i].reg_dt,input.reg_dt);
			}
			i = i + 1;
		}
		fclose(infile);

		j = 0;
		outfile = fopen (file_name,"w");
	   
		for(j=0; j<i; j++)
		{
			fwrite (&adat[j], sizeof(struct vendeg), 1, outfile);
		}
		fclose(outfile);
	}
}

void torol( const char* file_name, int v_azonosito)
{
	FILE *infile;
	FILE *outfile;
	int tomb_meret, i, j, torol;
	tomb_meret = elemszam(file_name);
	infile = fopen (file_name,"r");

	if (infile == NULL)
	{
		torol = -1;
	}
	else
	{
		i = 0;
		torol = 0;

		if (file_name == VENDEG)
		{
			struct vendeg input;
			struct vendeg adat[tomb_meret];

			while (fread (&input, sizeof(struct vendeg), 1, infile))
			{
					adat[i].id = input.id;
					strcpy(adat[i].nev,input.nev);
					strcpy(adat[i].email,input.email);
					adat[i].rendezveny = input.rendezveny;
					strcpy(adat[i].reg_dt,input.reg_dt);
					i = i + 1;
			}
			fclose(infile);
			remove(file_name);

			outfile = fopen (file_name,"a+");
			for(j=0; j<tomb_meret; j++)
			{
			  if (adat[j].id == v_azonosito)
				{
					printf("Törlésre került:\n");
					kiir_vendeg(adat[j]);
					torol = torol + 1;
				}
				else
				{
					fwrite (&adat[j], sizeof(struct vendeg), 1, outfile);
				}
			}
			fclose(outfile);
		}
		
		if (file_name == RENDEZVENY)
		{
			struct rendezveny input;
			struct rendezveny adat[tomb_meret];

			while (fread (&input, sizeof(struct rendezveny), 1, infile))
			{
					adat[i].azonosito = input.azonosito;
					strcpy(adat[i].nev,input.nev);
					strcpy(adat[i].helyszin,input.helyszin);
					strcpy(adat[i].datum,input.datum);
					strcpy(adat[i].idopont,input.idopont);
					i = i + 1;
			}
			fclose(infile);
			remove(file_name);

			outfile = fopen (file_name,"a+");
			for(j=0; j<tomb_meret; j++)
			{
			  if (adat[j].azonosito == v_azonosito)
				{
					printf("Törlésre került:\n");		
					kiir_rendezveny(adat[j]);
					torol = torol + 1;
				}
				else
				{
					fwrite (&adat[j], sizeof(struct rendezveny), 1, outfile);
				}
			}
			fclose(outfile);
		}
	}
}

int torol2( const char* file_name, int v_rendezveny)
{
	FILE *infile;
	FILE *outfile;
	int tomb_meret, i, j, torol;
	tomb_meret = elemszam(file_name);
	infile = fopen (file_name,"r");

	if (infile == NULL)
	{
		torol = -1;
	}
	else
	{
		i = 0;
		torol = 0;

		struct vendeg input;
		struct vendeg adat[tomb_meret];

		while (fread (&input, sizeof(struct vendeg), 1, infile))
		{
				adat[i].id = input.id;
				strcpy(adat[i].nev,input.nev);
				strcpy(adat[i].email,input.email);
				adat[i].rendezveny = input.rendezveny;
				strcpy(adat[i].reg_dt,input.reg_dt);
				i = i + 1;
		}
		fclose(infile);
		remove(file_name);

		outfile = fopen (file_name,"a+");

		for(j=0; j<tomb_meret; j++)
		{
		  if (adat[j].rendezveny == v_rendezveny)
			{
				printf("Törlésre került:\n");
				kiir_vendeg(adat[j]);
				torol = torol + 1;
			}
			else
			{
				fwrite (&adat[j], sizeof(struct vendeg), 1, outfile);
			}
		}
		fclose(outfile);
	}
	return torol;
}

main()
{
int menu, menu_arg, n_mod_azon, result, hiba, hiba2, v_elemszam, v_torol_db, v_letezik, v_elemszam_vendeg;
char term, v_yes_no;
struct tm ido;
time_t t;
menu = -1;
struct vendeg v1, v2, v3;
struct rendezveny r1;
char v_mod_azon[20];
char v_v_nev[20];
char v_k_nev[20];
char v_email[30];
char v_rendezveny[20];
char r_id[5];
char r_nev[30];
char r_helyszin[30];
char r_datum[20];
char r_idopont[10];

FILE *outfile;
	
while (menu != 0)
{

fomenu();
result = scanf("%i",&menu_arg);

if( result == 1) {
    menu = menu_arg;

		switch (menu)
		{
		//jelentkezés a részvételre
		case 1:
			printf("********************************Regisztráció********************************\n");

			v_elemszam = elemszam(RENDEZVENY);
			
			if (v_elemszam > 0)
			{

				printf("Kérjük adja meg az alábbi adatokat:\nvezetéknév: ");
				scanf ("%s", v_v_nev);
				printf("keresztnév: ");
				scanf ("%s", v_k_nev);
				printf("e-mail cím: ");
				scanf ("%s", v_email);
				printf("Válasszon egy rendezvény azonosítót az alábbiak közül:\n");
				lista(RENDEZVENY,-1);
				scanf ("%s", v_rendezveny);

				hiba2 = letezo_azonosito(RENDEZVENY,atoi(v_rendezveny));
					
				if (hiba2 > 0)
				{
					v1.id = next_azon(MAX_ID);
					refresh_azon(MAX_ID,v1.id);

					strcpy(v1.nev,strcat(strcat(v_v_nev," "),v_k_nev));
					strcpy(v1.email,v_email);
					v1.rendezveny = atoi(v_rendezveny);

					time(&t);
					asctime_r(localtime_r(&t, &ido), v1.reg_dt);
						
					outfile = fopen (VENDEG,"a+");
					fwrite (&v1, sizeof(struct vendeg), 1, outfile);
					fclose(outfile);
					
					printf("Ön a következő adatokat adta meg:\n");
					kiir_vendeg(v1);
					printf("Adatai felvételre kerültek!\n");
				}
				else
				{
						printf("Nem létező rendezvény azonosítót adott meg!\n");
						break;
				}
			}
			else
			{
				printf("Jelenleg nincs a rendszerben rögzítve rendezvény, ezért nem tud regisztrálni!\n");				
				printf("Vendég regisztrációhoz kérjük előbb rögzítsen rendezvényt!\n");
				break;
			}
			break;
		case 2:
			printf("********************************Vendég lista********************************\n");

			v_elemszam = elemszam(RENDEZVENY);
			
			if (v_elemszam > 0)
			{
				printf("Kérjük válasszon az alábbi rendezvények közül ([-1] = Összes):\n");
				printf("RENDEZVÉNYEK:\n\n");
				hiba = lista(RENDEZVENY,-1);
				
				if (hiba > 0)
				{
					result = scanf("%s", v_rendezveny);
					hiba2 = letezo_azonosito(RENDEZVENY,atoi(v_rendezveny));
					if (hiba2 == 0 && atoi(v_rendezveny) != -1)
					{
						printf("Nem létező rendezvény azonosítót adott meg!\n");
					}
					else
					{
						printf("REGISZTRÁLT VENDÉGEK:\n\n");
						v_elemszam_vendeg = lista(VENDEG,atoi(v_rendezveny));
						if (v_elemszam_vendeg <= 0)
						{
							printf("Nincs megjeleníthető adat a megadott paraméterek alapján!\n");
						}
					}
				}
			}
			else
			{
				printf("Jelenleg nincs rendezvény rögzítve, így nem tud vendeglistát sem lekérni!\n");				
			}
		break;
		case 3:
			printf("*****************************Vendég adatmódosítás*****************************\n");

			v_elemszam = elemszam(VENDEG);
			
			if (v_elemszam > 0)
			{

				printf("Kérjük adja meg a regisztrációjakor kapott azonosítóját, amelynek adatait módosítani szeretné!\n"); 
				printf("Regisztrációs azonosító: ");
				scanf ("%s", v_mod_azon);

				n_mod_azon = atoi(v_mod_azon);
				hiba = 0;
				hiba = letezo_azonosito(VENDEG,n_mod_azon);
				if (hiba == 0)
				{
					printf("Nem létező regisztrációazonosítót adott meg!\n\n");
					break;
				}
				else
				{
					v3 = get_adatok(VENDEG,n_mod_azon);
					printf("Jelenleg ezek az adatok vannak a rendszerben:\n");
					kiir_vendeg(v3);
				}

				v2.id = v3.id;
				strcpy(v2.reg_dt,v3.reg_dt);					
				
				printf("Szeretné módosítani az alábbi adatokat? Kérjük jelezze (i/n)!\n"); 
				printf("Szeretné módosítani a nevet (i/n)?\n");
				scanf (" %c", &v_yes_no);


				if (v_yes_no == 'i')
				{
					printf("Mire módosítja a vezetéknevet?\n");
					scanf ("%s", v_v_nev);
					printf("Mire módosítja a keresztnevet?\n");
					scanf ("%s", v_k_nev);
					strcpy(v2.nev,strcat(strcat(v_v_nev," "),v_k_nev));
				}
				else
				{
					strcpy(v2.nev,v3.nev);					
				}


				printf("Szeretné módosítani az e-mail címet (i/n)?\n");
				scanf(" %c", &v_yes_no);

				if (v_yes_no == 'i')
				{
					printf("Mire módosítja az e-mail címet?\n");
					scanf ("%s", v_email);
					strcpy(v2.email,v_email);
				}
				else
				{
					strcpy(v2.email,v3.email);
				}

				printf("Szeretné módosítani a rendezvény azonosítót (i/n)?\n");
				scanf(" %c", &v_yes_no);

				if (v_yes_no == 'i')
				{
					printf("\nVálasszon egy rendezvény azonosítót az alábbiak közül:\n");
					lista(RENDEZVENY,-1);
					scanf ("%s", v_rendezveny);

					hiba2 = letezo_azonosito(RENDEZVENY,atoi(v_rendezveny));
					if (hiba2 <= 0)
					{
						printf("Nem létező rendezvény azonosítót adott meg!\n");
						break;
					}
					v2.rendezveny = atoi(v_rendezveny);
				}
				else
				{
					v2.rendezveny = v3.rendezveny;
				}
				
				modosit(VENDEG,v2,n_mod_azon);
				printf("Az adatokat módosítottuk!\n");				
			}
			else
			{
				printf("Jelenleg nincs a rendszerben vendég rögzítve, így nem tud adatot módosítani!\n");				
			}
		break;
		
		case 4:
			printf("*****************************Vendég adat törlés*****************************\n");

			v_elemszam = elemszam(VENDEG);
			
			if (v_elemszam > 0)
			{
				printf("Kérjük adja meg a regisztrációjakor kapott azonosítót, amelyet törölni szeretne:\n"); 
				result = scanf("%s",v_mod_azon);

				if (result =! 1)
				{
					printf("Nem jól adta meg az adatokat!\n");
				}
				else
				{
					n_mod_azon = atoi(v_mod_azon);
					hiba = 0;
					hiba = letezo_azonosito(VENDEG,n_mod_azon);
					if (hiba == 0)
					{
						printf("Nem létező regisztrációazonosítót adott meg!\n\n");					
					}
					else
					{
						torol(VENDEG,n_mod_azon);
					}
				}
			}
			else
			{
				printf("Jelenleg nincs a rendszerben vendég rögzítve, így nem tud törölni!\n");				
			}
		break;
		case 5:
			printf("********************************Új rendezvény********************************\n");

			printf("Kérjük adja meg az alábbi adatokat:\nRendezvény azonosító: ");
			scanf ("%s", r_id);

			v_letezik = letezo_azonosito(RENDEZVENY,atoi(r_id));

			if (v_letezik >= 1)
			{
				printf("Létező rendezvényazonosítót adott meg!\n");
				break;
			}
			else
			{
				printf("Név: ");
				scanf ("%s", r_nev);
				printf("Helyszín(város): ");
				scanf ("%s", r_helyszin);
				printf("Dátum (yyyy.mm.dd): ");
				scanf ("%s", r_datum);
				printf("Időpont (hh:mm): ");
				scanf ("%s", r_idopont);

				r1.azonosito = atoi(r_id);
				strcpy(r1.nev,r_nev);
				strcpy(r1.helyszin,r_helyszin);
				strcpy(r1.datum,r_datum);
				strcpy(r1.idopont,r_idopont);

				outfile = fopen (RENDEZVENY,"a+");
				fwrite (&r1, sizeof(struct rendezveny), 1, outfile);
				fclose(outfile);
				
				printf("Ön a következő adatokat adta meg:\n\n");
				kiir_rendezveny(r1);
				printf("\nAdatokat rögzítettük!\n");
			}
			break;
			
		case 6:
			printf("********************************Rendezvény lista********************************\n");
			v_elemszam = elemszam(RENDEZVENY);
			
			if (v_elemszam > 0)
			{
			lista(RENDEZVENY,-1);
			}
			else
			{
				printf("Jelenleg nincs a rendszerben megjeleníthető rendezvény! Kérjük rögzítsen!\n");				
			}

		break;
		case 7:
			printf("*****************************Rendezvény törlése*****************************\n");

			v_elemszam = elemszam(RENDEZVENY);
			
			if (v_elemszam > 0)
			{
				printf("Kérjük adja meg a törlendő rendezvény azonosítóját:\n"); 
				printf("\nRENDEZVÉNYEK:\n"); 
				lista(RENDEZVENY,-1);
				result = scanf("%s",v_mod_azon);

				if (result =! 1)
				{
					printf("Nem jól adta meg az adatokat!\n");
				}
				else
				{
					n_mod_azon = atoi(v_mod_azon);
					hiba = 0;
					hiba = letezo_azonosito(RENDEZVENY,n_mod_azon);
					if (hiba == 0)
					{
						printf("Nem létező rendezvényazonosítót adott meg!\n\n");					
					}
					else
					{
						torol(RENDEZVENY,n_mod_azon);

						printf("\nA rendezvény törlése miatt az alábbi vendégeket törölte a rendszer:\n");		

						v_torol_db = torol2(VENDEG,n_mod_azon);
						
						if (v_torol_db <= 0)
						{
							printf("A rendezvényhez nem tartozott regisztrált vendég!\n");				
						}
					}
				}
			}
			else
			{
				printf("Jelenleg nincs a rendszerben rendezvény rögzítve, így nem tud törölni!\n");				
			}
		break;
		case 0:
		printf("Ön kiléptetésre kerül! Viszlát!\n");
		break;
		default:
		printf("Ön helytelen menüpontot adott meg. Kérjük próbálja újra!\n");
		break;
		}
	
}else{
	printf("Ön kilépett! Viszlát!\n");
	break;
}

};

}