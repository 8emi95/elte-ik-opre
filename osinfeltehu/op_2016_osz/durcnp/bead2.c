// Szabo Bendeguz, DURCNP
// OP. 1. bead
//Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez. Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)! 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

static int jelentkezo_darab = 0;
static int rendezveny_darab = 0;
// store events

const int MAX = 10;

void rendezveny_letrehoz()
{
	printf("\n Rendezveny azonosito szama: ");
	
	char azon[MAX];
	scanf("%s", &azon);

	int fid=mkfifo(azon, S_IRUSR|S_IWUSR );

	if (fid==-1)
	{
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
	}

	printf("\n Letrehozva a %s nevu rendezveny! \n", azon);

	++rendezveny_darab;
}

void rendezveny_listazas()
{
	if (rendezveny_darab > 0)
	{
		char rend_azon[MAX];
	
		printf("\n Adja meg a listazando rendezvenyt: ");
		scanf(" %s", &rend_azon);

		char buf[MAX * 100];
    		int fd = open(rend_azon, O_RDONLY);
    		read(fd, buf, sizeof buf);
    		printf("%s\n", buf);
    		close(fd);
	}
	else
	{
		printf("\n Nincs listazhato rendezveny!");
	}
}

void rendezveny_urites()
{
	if (rendezveny_darab > 0)
	{
		char rend_azon[MAX];
	
		printf("\n Adja meg az uritendo rendezvenyt: ");
		scanf(" %s", &rend_azon);
		
		unlink(rend_azon);

		int fid=mkfifo(rend_azon, S_IRUSR|S_IWUSR );

		if (fid==-1)
		{
			printf("Error number: %i",errno);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		printf("\n Nincs uritheto rendezveny!");
	}
}

void jelentkezo_felvetel()
{
	if (rendezveny_darab > 0)
	{
		char nev[MAX], mail[MAX], rend_azon[MAX];
	
		printf("\n Adja meg a jelentkezo nevet: ");
		scanf(" %s", &nev);
		printf("\n Adja meg a jelentkezo mail cimet: ");
		scanf(" %s", &mail);
		printf("\n Adja meg a rendezveny azonositojat: ");
		scanf(" %s", &rend_azon);

		char message[MAX * 2 + 4];
		snprintf(message, sizeof message, "%s %s\n", nev, mail);
		int fd=open(rend_azon, O_WRONLY);
        	write(fd, message, sizeof message);

		++jelentkezo_darab;
		printf("%i jelentkezo van eddig felveve.", jelentkezo_darab);
	}
	else
	{
		printf("\n Nincs parosithato rendeveny!");
	}
}

void jelenentkezo_modosit()
{
	if (jelentkezo_darab > 0)
	{
		char nev[MAX], rend_azon[MAX];

		printf("\n Adja meg a rendezveny azonositojat: ");
		scanf(" %s", &rend_azon);	
		printf("\n Adja meg a jelentkezo nevet: ");
		scanf(" %s", &nev);

		// jelentkezo torol

		char nev_uj[MAX], mail_uj[MAX];

		printf("\n Adja meg a modositott uj nevet: ");
		scanf(" %s", &nev_uj);
		printf("\n Adja meg a modositott uj mail cimet: ");
		scanf(" %s", &mail_uj);

		char message[MAX * 2 + 4];
		snprintf(message, sizeof message, "%s %s\n", nev_uj, mail_uj);
		int fd=open(rend_azon, O_WRONLY);
        	write(fd, message, sizeof message);
        	close(fd);
	}
	else
	{
		printf("\n Nincs modosithato jelentkezo!");
	}
}

void jelentkezo_torles()
{
	if (jelentkezo_darab > 0)
	{
		char nev[MAX], rend_azon[MAX];

		printf("\n Adja meg a rendezveny azonositojat: ");
		scanf(" %s", &rend_azon);	
		printf("\n Adja meg a jelentkezo nevet: ");
		scanf(" %s", &nev);

		// jelentkezo torol
	}
	else
	{
		printf("\n Nincs torolheto jelentkezo!");
	}
}

int main(int argc, char* argv[])
{
	printf("King of Stands nyilvantartas. \n");

	char answer;
	while (answer != 'k' && answer != 'K')
	{
		printf("\nMenu:\n");

		if (jelentkezo_darab > 0)
		{
			printf(" Vendeg modositasa (v)\n");
			printf(" Vendeg torlese (t)\n");
		}
		if (rendezveny_darab > 0)
		{
			printf(" Jelentkezes (j) \n");
			printf(" Jelentkezettek Listazasa (l)\n");
			printf(" Rendezveny urites (u) \n");
		}

		printf(" Rendezveny letrehozasa (r)\n");
		printf(" Kilep (k) \n");

		printf("  Valasz: ");

		scanf(" %c", &answer);

		if (answer == 'j' || answer == 'J')
		{
			jelentkezo_felvetel();
		}
		else if (answer == 'l' || answer == 'L')
		{
			rendezveny_listazas();
		}
		else if (answer == 'v' || answer == 'V')
		{
			jelenentkezo_modosit();
		}
		else if (answer == 't' || answer == 'T')
		{
			jelentkezo_torles();
		}
		else if (answer == 'r' || answer == 'R')
		{
			rendezveny_letrehoz();
		}
		else if (answer == 'u' || answer == 'U')
		{
			rendezveny_urites();
		}
		if (answer != 'k' && answer != 'K')
		{
			printf("\n Helytelen parancs! \n");
		}
	}

	printf("\n Alkalmazasbol kilep.");

//	unlink("fifo.ftc");

//    printf("Mkfifo vege, fork indul!\n");





    //pid = fork();
    
    //if(pid>0)
	//{
	//char s[1024]="Semmi";		
	//printf("Csonyitas eredmenye szuloben: %d!\n",fid);
	//fd=open("fifo.ftc",O_RDONLY);
	//read(fd,s,sizeof(s));
	//printf("Ezt olvastam a csobol: %s \n",s);
	//close(fd);
	// remove fifo.ftc
	//unlink("fifo.ftc");
        //}
	//else // child
	//{
	//printf("Gyerek vagyok, irok a csobe!\n");
	//printf("Csonyitas eredmenye: %d!\n",fid);
        //fd=open("fifo.ftc",O_WRONLY);
        //write(fd,"Hajra Fradi!\n",12);
        //close(fd);
	//printf("Gyerek vagyok, beirtam, vegeztem!\n");
    	//}	

    return 0; 
}
