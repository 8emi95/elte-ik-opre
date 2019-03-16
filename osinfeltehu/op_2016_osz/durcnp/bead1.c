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

void rendezveny_letrehoz() {
	printf("\n Rendezveny azonosito szama: ");
	
	char azon[100];
	scanf(" %i", &azon);

	int fid=mkfifo(azon, S_IRUSR|S_IWUSR );

	if (fid==-1)
	{
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
	}

	printf("\n Letrehozva a %s nevu rendezveny! \n");
}

int main(int argc, char* argv[])
{
	printf("King of Stands nyilvantartas. \n");

	char answer;
	while (answer != 'k' && answer != 'K')
	{
		printf("\n Menu:\n Jelentkezes (j)\n Jelentkezettek Listazasa (l)\n Vendeg modositasa (v)\n Vendeg torlese (t)\n Rendezveny letrehozasa (r)\n Rendezveny torlese (o) \n Kilep (k)\n Valasz: ");

		scanf(" %c", &answer);

		if (answer == 'j' || answer == 'J')
		{
		}
		else if (answer == 'l' || answer == 'L')
		{
		}
		else if (answer == 'v' || answer == 'V')
		{
		}
		else if (answer == 't' || answer == 'T')
		{
		}
		else if (answer == 'r' || answer == 'R')
		{
			rendezveny_letrehoz();
		}
		else if (answer == 'o' || answer == 'O')
		{
		}
		else if (answer != 'k' && answer != 'K')
		{
			printf("\n Helytelen parancs! \n");
		}
	}

	printf("\n Alkalmazasbol kilep. \n");

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
