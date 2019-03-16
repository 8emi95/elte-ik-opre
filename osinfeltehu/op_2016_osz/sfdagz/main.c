typedef int bool;
#define true 1
#define false 0

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_SIZE 256

struct vendeg
{
	char nev[MAX_SIZE];
	char email[MAX_SIZE];
	int razon;
	char* ido;
};

struct vendeg adatok[10000];
int rendezvenyek[10000];
int rendezvenynum = 0;
int vendegnum = 0;

void VendegJelentkezes(bool modosit)
{
    char tmp;
	printf("Kerem adja meg a nevet: ");
	fgets(adatok[vendegnum-1].nev,MAX_SIZE,stdin);
	if ((strlen(adatok[vendegnum-1].nev)>0) && (adatok[vendegnum-1].nev[strlen (adatok[vendegnum-1].nev) - 1] == '\n'))
    {
          adatok[vendegnum-1].nev[strlen (adatok[vendegnum-1].nev) - 1] = '\0';
    }
	printf("Kerem adja meg az e-mail cimet: ");
	fgets(adatok[vendegnum-1].email,MAX_SIZE,stdin);
	if ((strlen(adatok[vendegnum-1].email)>0) && (adatok[vendegnum-1].email[strlen (adatok[vendegnum-1].email) - 1] == '\n'))
    {
          adatok[vendegnum-1].email[strlen (adatok[vendegnum-1].email) - 1] = '\0';
    }
    bool sikertelen = true;
    while (sikertelen)
    {
        printf("Kerem adja meg a rendezveny azonositojat: ");
        char azon[MAX_SIZE];
        fgets(azon,MAX_SIZE,stdin);
        if ((strlen(azon)>0) && (azon[strlen (azon) - 1] == '\n'))
        {
              azon[strlen (azon) - 1] = '\0';
        }
        char* end;
        int rid;
        rid = strtol(azon,&end, 10);
        int j;
        for (j =0; j < rendezvenynum; j = j + 1)
        {
            if (rid == rendezvenyek[j])
            {
                sikertelen = false;
                adatok[vendegnum-1].razon = rid;
                break;
            }
        }
        if (sikertelen)
        {
            printf("Nincs ilyen azonositoju rendezveny!\n");
        }
    }
	if (!modosit)
    {
        time_t rawtime;
        struct tm* timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        adatok[vendegnum-1].ido = asctime(timeinfo);
        printf("\n");
        printf("On az ");
        printf("%d",vendegnum);
        printf(". vendeg!\n");
    }



}

void VendegListazas()
{
    int i;
    printf("Nev | E-mail | Rendezveny azonosito | Jelentkezesi ido\n");
	for (i = 0; i < vendegnum; i = i+1)
	{
	    printf("%s |",adatok[i].nev);
        printf(" %s |",adatok[i].email);
        printf(" %d |",adatok[i].razon);
        printf(" %s\n",adatok[i].ido);
	}

}

void VendegModositas()
{
    char nev[MAX_SIZE];
	printf("Kerem a vendeg nevet: ");
	fgets(nev,MAX_SIZE,stdin);
	if ((strlen(nev)>0) && (nev[strlen (nev) - 1] == '\n'))
    {
          nev[strlen (nev) - 1] = '\0';
    }
    int i = 0;
    while (strcmp(nev,adatok[i].nev) && i != vendegnum)
    {
        i = i+1;

    }
    if (!strcmp(nev,adatok[i].nev))
    {
        VendegJelentkezes(true);
        printf("Modositas sikeres!\n");
    }
    else
    {
        printf("Nincs ilyen nevu vendeg!\n");
    }
}

void UjRendezveny()
{
    printf("Kerem adja meg a rendezveny azonositojat: ");
	char azon[MAX_SIZE];
	fgets(azon,MAX_SIZE,stdin);
	if ((strlen(azon)>0) && (azon[strlen (azon) - 1] == '\n'))
    {
          azon[strlen (azon) - 1] = '\0';
    }
    char* end;
    int rid;
    rid = strtol(azon,&end, 10);
    rendezvenyek[rendezvenynum] = rid;
    rendezvenynum = rendezvenynum + 1;
}

void RendezvenyTorles()
{
	 printf("Kerem adja meg a rendezveny azonositojat: ");
	char azon[MAX_SIZE];
	fgets(azon,MAX_SIZE,stdin);
	if ((strlen(azon)>0) && (azon[strlen (azon) - 1] == '\n'))
    {
          azon[strlen (azon) - 1] = '\0';
    }
    char* end;
    int rid;
    rid = strtol(azon,&end, 10);
    int i = 0;
    while (rid != rendezvenyek[i] && i != rendezvenynum)
    {
        ++i;
    }
    if (i == rendezvenynum)
    {
        printf("Nincs ilyen azonositoju rendezveny!\n");
    }
    else
    {
        rendezvenyek[i] = rendezvenyek[rendezvenynum-1];
        rendezvenyek[rendezvenynum-1] = rendezvenyek[rendezvenynum];
        int j;
        for (j = 0; j < vendegnum; j = j + 1)
        {
            if (rid == adatok[j].razon)
            {
                adatok[j] = adatok[vendegnum-1];
                adatok[vendegnum-1] = adatok[vendegnum];
                vendegnum = vendegnum - 1;
                j = j - 1;
            }
        }
    }
}

void VendegTorles()
{
    char nev[MAX_SIZE];
	printf("Kerem a vendeg nevet: ");
	fgets(nev,MAX_SIZE,stdin);
	if ((strlen(nev)>0) && (nev[strlen (nev) - 1] == '\n'))
    {
          nev[strlen (nev) - 1] = '\0';
    }
    int i = 0;
    while (strcmp(nev,adatok[i].nev) && i != vendegnum)
    {
        i = i+1;

    }
    if (!strcmp(nev,adatok[i].nev))
    {
       adatok[i] = adatok[vendegnum-1];
       adatok[vendegnum-1] = adatok[vendegnum];
       vendegnum = vendegnum -1;
       printf("Torles sikeres!\n");
    }
    else
    {
        printf("Nincs ilyen nevu vendeg!\n");
    }

}

void Beolvasas()
{
    FILE* fr;
    fr = fopen("rendezvenyek.txt","r");
    char rnum[MAX_SIZE];
    fgets(rnum,MAX_SIZE, fr);
    if (strlen(rnum) == 0)
    {
        return;
    }
    rendezvenynum = atoi(rnum);

    int r;
    char raz[MAX_SIZE];
    for (r = 0; r < rendezvenynum; r = r + 1)
    {
        fgets(raz,MAX_SIZE,fr);
        rendezvenyek[r] = atoi(raz);
    }
    fclose(fr);

    FILE* fp;
    fp = fopen("adatok.txt", "r");
    char num[MAX_SIZE];
    fgets(num, MAX_SIZE, fp);
    if (strlen(num) == 0)
    {
        return;
    }
    vendegnum = atoi(num);
    int i;
    char adat[MAX_SIZE];
    for (i = 0; i < vendegnum; i = i + 1)
    {
        fgets(adat,MAX_SIZE,fp);
        int j = 0;
        while(adat[j+1] != '|')
        {
            adatok[i].nev[j] = adat[j];
            j = j + 1;
        }
        j = j + 3; //atugorjuk a |-t és a szóközt
        int k = 0;
        while(adat[j+1] != '|')
        {

            adatok[i].email[k] = adat[j];
            j = j+1;
            k = k+1;
        }
        j = j + 3;
        char rid[MAX_SIZE];
        k = 0;
        while(adat[j+1] != '|')
        {

            rid[k] = adat[j];
            j = j+1;
            k = k+1;
        }
        adatok[i].razon = atoi(rid);
        j = j + 3;
        adatok[i].ido = malloc(256*sizeof(char));
        k = 0;
        while(adat[j] != '\n')
        {
            adatok[i].ido[k] = adat[j];
            k = k + 1;
            j = j + 1;
        }
        adatok[i].ido[k] = '\0';
    }
    fclose(fp);
}

void Kiiras()
{
    if (rendezvenynum)
    {
        FILE* fp;
        fp = fopen("rendezvenyek.txt","w");
        fprintf(fp,"%d\n",rendezvenynum);
        int i;
        for (i = 0; i < rendezvenynum; i = i +1 )
        {
            fprintf(fp,"%d\n", rendezvenyek[i]);
        }
        fclose(fp);
    }
    if (vendegnum)
    {
        FILE* fp;
        fp = fopen("adatok.txt", "w");
        fprintf(fp,"%d\n",vendegnum);
        int i;
        for (i = 0; i < vendegnum; i = i + 1)
        {
            fprintf(fp,"%s |", adatok[i].nev);
            fprintf(fp," %s |", adatok[i].email);
            fprintf(fp," %d |", adatok[i].razon);
            fprintf(fp," %s\n", adatok[i].ido);
        }
        fclose(fp);
    }
}

int main(int argc, char **argv)
{
    Beolvasas();
	int vendegvendegnum = 0;
	printf("Udvozoljuk!\n");
	printf("\n");
	char funkcio = '0';
	while (true)
	{
		printf("Vendeg jelentkezes - j\n");
		printf("Vendegek listazasa - l\n");
		printf("Egy vendeg adatainak modositasa - m\n");
		printf("Egy vendeg adatainak torlese - x\n");
		printf("Uj rendezveny inditasa - u\n");
		printf("Egy rendezveny adatainak torlese - t\n");
		printf("Kilepes - k\n");
		printf("Kerem usse be hasznalni kivant funkcio betujelet : ");
		scanf(" %c", &funkcio);
		printf("\n");
		char tmp;
		tmp = getchar();
		bool hibasbemenet = false;
		if (tmp != '\n')
        {
            hibasbemenet = true;
        }
		while (tmp != '\n')
		{
			tmp = getchar();
		}
        tmp = 'a';
        if (hibasbemenet)
        {
            hibasbemenet = false;
            printf("Hiba! Az on altal megadott betujelhez nem tartozik funkcio\n");
            continue;
        }
		funkcio = toupper(funkcio);
		switch (funkcio)
		{
			case 'J':
				vendegnum = vendegnum + 1;
				VendegJelentkezes(false);
				break;
			case 'L':
				VendegListazas();
				break;
			case 'M':
				VendegModositas();
				break;
			case 'U':
				UjRendezveny();
				break;
			case 'T':
				RendezvenyTorles();
				break;
			case 'K':
				printf("Viszontlatasra!");
				Kiiras();
				int i;
				for (i = 0; i < vendegnum; i = i + 1)
                {
                    free(adatok[i].ido);
                }
				exit(0);
            case 'X':
                VendegTorles();
                break;
			default:
				printf("Hiba! Az on altal megadott betujelhez nem tartozik funkcio\n");
				break;
		}
		printf("\n");
	}
	return 0;
}
