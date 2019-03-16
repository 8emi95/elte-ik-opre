#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <poll.h> 		// poll
#include <fcntl.h> 		//O_RDONLY,
#include <unistd.h>		//close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>	//rand
#include <signal.h>		//SIGTERM

#include <errno.h>
#include <limits.h>

#include <semaphore.h>

//Forditas: gcc main.c -o main -l pthread
//Az alabbi program cso segitsegevel valositja meg a gyak4.doc-ban leirt feladatot.
//Ha tul gyorsan inditjuk ismet a programot, akkor elofordulhat, hogy a szavazasok eredmenye nem fog megjelenni, mivel az elozo futas utan meg nem
//szunt meg a compipe, amikor mi mar inditjuk az ujat, igy a cso a masodik inditas kozben fog megszunni, am ekkor mi mar az uj futtatasnal elhagytuk
//a cso letrehozasanak kodjat, igy nem fogunk ujat letrehozni.

void handler(int signumber);

sem_t* szemafor_letrehozas(char* nev, int szemafor_ertek);
void szemafor_torles(char* nev);
void leaveTheRoom();

int getIntFromConsole();
void printPressEnter(void);

sem_t* semLeave;														//Ez fogja kezelni, hogy egyszerre csak egy tag mehet ki a szobabol

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	signal(SIGUSR1, handler);											//Ellenor
	signal(SIGUSR2, handler);											//Pecsetelo
	unlink("compipe");
	
	int f = mkfifo("compipe", 0600);
	
	if (f < 0)
	{
		perror("error");
		exit(1);
	}
	
	f = open("compipe", O_RDWR);
	
	struct pollfd poll_fds[5];											//poll file leiro tomb
	poll_fds[0].fd = f;													//File leiro
	poll_fds[0].events = POLLIN;										//|POLLOUT; //POLLIN - van olvashato adat, POLLOUT - irhatunk blokkolas nelkul
	
	//Szemafor letrehozasa
    char* sem_nev="/sem810";	
	semLeave = szemafor_letrehozas(sem_nev, 1);
	
	pid_t ellenor = fork();												//1. gyermek folyamat letrehozasa (ellenor)
	
	if (ellenor > 0)
	{
		//Szulo folyamat
		pid_t pecsetelo = fork();										//2. gyermek folyamat letrehozasa (pecsetelo)
		
		if (pecsetelo > 0)
		{
			//Tenyleges szulo folyamat (elnok)
			pause();													//Megvarjuk, hogy jelezzen az ellenor
			pause();													//Megvarjuk, hogy jelezzen a pecsetelo
			
			printf("Elnok - Az ellenor es a pecsetelo is keszen all a munkara.\n");
			
			//Kerjuk be a szavazokat
			printf("\nMennyi szavazo fog erkezni?: ");
			
			int numOfVoters = getIntFromConsole();
			
			if (numOfVoters == -1)
			{
				kill(ellenor, SIGTERM);									//Leallitjuk az ellenor folyamatot
				kill(pecsetelo, SIGTERM);								//Leallitjuk a pecsetelo folyamatot
				
				exit(1);
			}
			
			//Random ID-k eloallitasa
			int* IDs = malloc(numOfVoters * sizeof(int));
			int i = 0;
			
			for (i; i != numOfVoters; ++i)
			{
				IDs[i] = rand() % 1000;
			}
			
			write(f, IDs, numOfVoters * sizeof(int));					//Azonositok elkuldese az ellenornek
			write(f, &pecsetelo, sizeof(pid_t));						//A pecsetelo pid-jenek elkuldese az ellenornek
			
			pause();													//Megvarjuk, hogy megerkezzenek az adatok a pecsetelotol
			
			//Megerkeztek az azonositok, a szavazati jogok es a megjelent / nem megjelent osszesitesek
			int IDsCanVoteAndPresents[200];
			
			int recData = read(f, IDsCanVoteAndPresents, 200 * sizeof(int));
			recData /= sizeof(int);
			
			int valid = IDsCanVoteAndPresents[recData - 4];				//Azok szama, akik ervenyesen szavazhattak (hatulrol a 4. ertek)
			int wasPresentValid = IDsCanVoteAndPresents[recData - 3];	//Azok szama, akik megjelentek es ervenyesen szavazhattak (hatulrol a 3. ertek)
			int invalid = IDsCanVoteAndPresents[recData - 2];			//Azok szama, akik ervenytelenul szavazhattak (hatulrol a 2. ertek)
			int wasPresentInvalid = IDsCanVoteAndPresents[recData - 1];	//Azok szama, akik megjelentek es ervenytelenul szavazhattak (hatulrol a 1. ertek)
			
			double wasPresentValidAndValid = (valid != 0) ? (((double)wasPresentValid / valid) * 100.0) : 0.0;
			double wasPresentInvalidAndInvalid = (invalid != 0) ? (((double)wasPresentInvalid / invalid) * 100.0) : 0.0;
			
			printf("Elnok - \n\tSzavazhatott es megjelent aranya: %g% \n\tNem szavazhatott es megjelent aranya: %g%\n", wasPresentValidAndValid, wasPresentInvalidAndInvalid);
			
			//Osszesites fajlba irasa
			FILE *file = fopen("file.txt", "w");
			
			if (file == NULL)
			{
				printf("Error opening file!\n");
				
				kill(ellenor, SIGTERM);									//Leallitjuk az ellenor folyamatot
				kill(pecsetelo, SIGTERM);								//Leallitjuk a pecsetelo folyamatot
				
				exit(1);
			}
			
			fprintf(file, "Elnok - \n\tSzavazhatott es megjelent aranya: %g% \n\tNem szavazhatott es megjelent aranya: %g%\n", wasPresentValidAndValid, wasPresentInvalidAndInvalid);
			fclose(file);												//Bezarjuk a fajlt
			
			pause();
			
			//Megerkezett a szavazas eredmenye
			int* voteResults = malloc(numOfVoters * sizeof(int));
			
			read(f, voteResults, numOfVoters * sizeof(int));
			
			printf("Elnok - A szavazas eredmenye:\n\t");
			
			int j = 0;
			
			for (j; j != numOfVoters; ++j)
			{
				//A -1-et nem irjuk ki, mivel az nem szavazas
				if (voteResults[j] != -1)
				{
					printf("%i ", voteResults[j]);
				}
			}
			
			printf("\n");
			
			leaveTheRoom();												//Meghivjuk a szoba elhagyasat kezelo fuggvenyt
			
			wait(NULL);
			
			free(IDs);													//Fel kell szabaditani az IDs-t
			free(voteResults);											//Fel kell szabaditani a voteResults-ot
		}
		else
		{
			//2. gyermek folyamat (pecsetelo)
			printf("Pecsetelo - Keszen allok a munkara.\n");
			kill(getppid(), SIGUSR2);
			
			pause();													//Varunk az ellenor jelzesere
			
			//Megerkeztek az azonositok es a szavazati jogok
			int IDsAndcanVote[200];
			
			int numOfIDs = read(f, IDsAndcanVote, 200 * sizeof(int));
			numOfIDs /= 2 * sizeof(int);								//Osztunk 2-vel, mivel 2x annyi adatot olvastunk (ID es a hozza tartozo szavazati jog), valamint az int meretevel
			
			printf("Pecsetelo - Az alabbi szemelyek szavazhatnak / nem szavazhatnak:\n");
			
			int valid = 0;												//Azok szama, akik ervenyesen szavazhattak
			int wasPresentValid = 0;									//Azok szama, akik megjelentek es ervenyesen szavazhattak
			int invalid = 0;											//Azok szama, akik ervenytelenul szavazhattak
			int wasPresentInvalid = 0;									//Azok szama, akik megjelentek es ervenytelenul szavazhattak
			
			int* voteResults = malloc(numOfIDs * sizeof(int));			//Szavazasok eredmenye
			
			int i = 0;
			
			for (i; i != numOfIDs; ++i)
			{
				if (IDsAndcanVote[i + numOfIDs] == 1)
				{
					printf("\t%i - szavazhat\n", IDsAndcanVote[i]);
					
					++valid;
					
					if ((rand() % 2) == 1)
					{
						++wasPresentValid;								//Ha szavazhat es megjelent, akkor noveljuk a szamlalot
						
						voteResults[i] = (rand() % 6);					//0 es 5 kozott szavazunk valakire
					}
					else
					{
						voteResults[i] = -1;							//Ha valaki nem szavazhatott, akkor -1 lesz az ertek
					}
				}
				else
				{
					printf("\t%i - nem szavazhat\n", IDsAndcanVote[i]);
					
					++invalid;
					
					if ((rand() % 2) == 1)
					{
						++wasPresentInvalid;							//Ha nem szavazhat es megjelent, akkor noveljuk a szamlalot
						voteResults[i] = -1;							//Ha valaki nem szavazhatott, akkor -1 lesz az ertek
					}
					else
					{
						voteResults[i] = -1;							//Ha valaki nem szavazhatott, akkor -1 lesz az ertek
					}
				}
			}
			
			//printf("valid: %i, wasPresentValid: %i, invalid: %i, wasPresentInvalid: %i\n", valid, wasPresentValid, invalid, wasPresentInvalid);
			
			write(f, IDsAndcanVote, 2 * numOfIDs * sizeof(int));		//Azonositok es a szavazati jogosultsagok elkuldese az elnoknek
			write(f, &valid, sizeof(int));								//Ervenyesen szavazok szamanak elkuldese az elnoknek
			write(f, &wasPresentValid, sizeof(int));					//A megjelent, ervenyesen szavazok szamanak elkuldese az elnoknek
			write(f, &invalid, sizeof(int));							//Ervenytelenul szavazok szamanak elkuldese az elnoknek
			write(f, &wasPresentInvalid, sizeof(int));					//A nem megjelent, ervenytelenul szavazok szamanak elkuldese az elnoknek
			
			kill(getppid(), SIGUSR2);									//Jelzunk az elnoknek
			
			sleep(2);
			
			write(f, voteResults, numOfIDs * sizeof(int));				//A szavazas eredmenyenek elkuldese
			kill(getppid(), SIGUSR2);									//Jelzunk az elnoknek
			
			leaveTheRoom();												//Meghivjuk a szoba elhagyasat kezelo fuggvenyt
			
			free(voteResults);
			
			exit(0);
		}
	}
	else
	{ 
		//1. gyermek folyamat (ellenor)
		printf("Ellenor - Keszen allok a munkara.\n");
		kill(getppid(), SIGUSR1);
		
		int result = poll(poll_fds, 1, 8000);
		
		if (result > 0) 
		{
			if (poll_fds[0].revents & POLLIN)
			{
				//Megerkeztek az azonositok
				int IDs[100];
				
				int numOfIDs = read(f, IDs, 100 * sizeof(int));
				numOfIDs /= sizeof(int);								//Leosztunk egy int meretevel, mert a read-bol az osszes olvasott byte-ok szamat kaptuk vissza
				numOfIDs -= 1;											//Levonunk 1-et, mivel az utolso ertek a pecsetelo pid-je
				
				printf("Ellenor - Megkaptam az azonositokat: ");
				
				int i = 0;
				
				for (i; i != numOfIDs; ++i)
				{
					(i < numOfIDs - 1) ? printf("%i, ", IDs[i]) : printf("%i\n", IDs[i]);
				}
				
				pid_t pecsetelo = IDs[numOfIDs];						//A csoben levo utolso ertek a pecsetelo pid-je volt
				IDs[numOfIDs] = 0;										//Visszaallitjuk 0-ra, hogy ne keruljon veletlenul mashova
				
				//Szavazas jogosultsaganak eldontese es elkuldese a pecsetelonek
				int* canVote = malloc(numOfIDs * sizeof(int));
				int j = 0;
				
				for (j; j != numOfIDs; ++j)
				{
					//20% valoszinuseggel hibas az azonosito (azaz ha az 5 szam kozul a 4-es jon ki, akkor hibas)
					if ((rand() % 5) == 4)
					{
						canVote[j] = 0;									//Ha 0, akkor nem tud szavazni
					}
					else
					{
						canVote[j] = 1;									//Ha 1, akkor tud szavazni
					}
				}
				
				write(f, IDs, numOfIDs * sizeof(int));					//Azonositok elkuldese a pecsetelonek
				write(f, canVote, numOfIDs * sizeof(int));				//Szavazati jogosultsagok elkuldese a pecsetelonek
				kill(pecsetelo, SIGUSR1);								//Jelzunk a pecsetelonek
				
				leaveTheRoom();											//Meghivjuk a szoba elhagyasat kezelo fuggvenyt
				
				free(canVote);											//Fel kell szabaditani a canVote-ot
			}
		}
		else
		{
			printf("Returned poll: %i", result);
		}
		
		exit(0);
	}
	
	unlink("compipe");
	szemafor_torles(sem_nev);											//Szemafor torlese
    
    return 0;
}

void handler(int signumber)
{
	//A fuggveny direkt maradt uresen (esetleg ki lehet irni az alabb levo uzenetet, de ez nem fontos)
	//printf("   Signal from Partner has arrived.\n");
}

sem_t* szemafor_letrehozas(char*nev, int szemafor_ertek)
{
    sem_t *semid=sem_open(nev, O_CREAT, S_IRUSR | S_IWUSR, szemafor_ertek);
	
	if (semid==SEM_FAILED)
	{
		perror("sem_open");
	}
       
    return semid;
}

void szemafor_torles(char* nev)
{
      sem_unlink(nev);
}

void leaveTheRoom()
{
	//Random kivalasztunk egy tagot, s ha lehetseges, elhagyjuk vele a termet, majd visszajovunk
	int person = rand() % 3;											//0 - elnok, 1 - ellenor, 2 - pecsetelo
	char* report;
	
	switch(person)
	{
		case 0:
			report = "Az elnok elhagyta a termet, majd visszatert.\n";
			break;
		case 1:
			report = "Az ellenor elhagyta a termet, majd visszatert.\n";
			break;
		case 2:
			report = "A pecsetelo elhagyta a termet, majd visszatert.\n";
			break;
	}
	
	sem_wait(semLeave);													//Varunk, hogy a szemafor szabad legyen
	
	FILE *file = fopen("report.txt", "a+");
			
	if (file == NULL)
	{
		printf("Error opening file!\n");
		
		exit(1);
	}
	
	//printf(report);
	fprintf(file, report);
	fclose(file);
	
	sem_post(semLeave);													//Elengedjuk a szemafort
}

int getIntFromConsole()
{
	int number;
	char cNumber[100];
	
	fgets(cNumber, sizeof(cNumber), stdin);
	
	if (cNumber[0] == '\n') return -1;									//Ha nem adott meg szamot, akkor visszaterunk
	
	char* endptr;														//Ide kerul a strtol szoveg resze 
	errno = 0;															//Hiba eseten ez atallitodik
	
	number = strtol(cNumber, &endptr, 10);								//long-a, majd int-e konvertaljuk
	
	if ((endptr == cNumber) || ((number == LONG_MAX || number == LONG_MIN) && errno == ERANGE))
	{
		printf("\nInvalid number.\n");
		printPressEnter();
		return -1;
	}
	
	return number;
}

void printPressEnter(void)
{
	printf("\nPress ENTER to Continue");
	while(getchar() != '\n');
}