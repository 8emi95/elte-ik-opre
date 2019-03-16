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

#include <sys/shm.h>

//Forditas: gcc osztottmemoriaval.c -o osztottmemoriaval -l pthread
//Az alabbi program osztott memoria segitsegevel valositja meg a gyak4.doc-ban leirt feladatot.
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
sem_t* semShMem;														//Osztott memoria hasznalatat kezelo szemafor

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	signal(SIGUSR1, handler);											//Ellenor
	signal(SIGUSR2, handler);											//Pecsetelo
	unlink("compipe");
	
	//Osztott memoria letrehozasa
	key_t kulcs;
    int shMemID;
	int* numbers;
	
	kulcs = ftok(argv[0], 1);											//A parancs nevevel es az 1 verzioszammal kulcs generalasa
    shMemID = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR);		//Osztott memoria letrehozasa irasra es olvasasra, 500 bajt merettel
    numbers = shmat(shMemID, NULL, 0);									//Csatlakozas az osztott memoriahoz
	
	//Szemafor letrehozasa
    char* sem_nev="/sem810";	
	semLeave = szemafor_letrehozas(sem_nev, 1);
	semShMem = szemafor_letrehozas("/semShMem810", 0);
	
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
				numbers[i + 1] = IDs[i];
			}
			
			numbers[0] = numOfVoters;									//Az elso ertek jelzi a szavazok szamat
			numbers[numOfVoters + 1] = pecsetelo;						//Az ID-k utan kovetkezik a pecsetelo pid_t-je
			
			sem_post(semShMem);											//Elengedjuk a szemafort, az ellenor most mar olvashat a memoriabol
			
			pause();													//Megvarjuk, hogy megerkezzenek az adatok a pecsetelotol
			sem_wait(semShMem);											//Varunk, hogy a pecsetelo elengedje az osztott memoria szemaforjat
			
			//Megerkeztek az azonositok, a szavazati jogok es a megjelent / nem megjelent osszesitesek
			int valid = numbers[(2 * numOfVoters) + 1];					//Azok szama, akik ervenyesen szavazhattak
			int wasPresentValid = numbers[(2 * numOfVoters) + 2];		//Azok szama, akik megjelentek es ervenyesen szavazhattak
			int invalid = numbers[(2 * numOfVoters) + 3];				//Azok szama, akik ervenytelenul szavazhattak
			int wasPresentInvalid = numbers[(2 * numOfVoters) + 4];		//Azok szama, akik megjelentek es ervenytelenul szavazhattak
			
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
			printf("Elnok - A szavazas eredmenye:\n\t");
			
			int j = 0;
			
			for (j; j != numOfVoters; ++j)
			{
				//A -1-et nem irjuk ki, mivel az nem szavazas
				if (numbers[j] != -1)
				{
					printf("%i ", numbers[j]);
				}
			}
			
			printf("\n");
			
			leaveTheRoom();												//Meghivjuk a szoba elhagyasat kezelo fuggvenyt
			
			wait(NULL);													//Megvarjuk az ellenor es a pecsetelo befejezodeset
			
			free(IDs);													//Fel kell szabaditani az IDs-t
			shmdt(numbers);												//Elengedjuk az osztott memoriat
		}
		else
		{
			//2. gyermek folyamat (pecsetelo)
			printf("Pecsetelo - Keszen allok a munkara.\n");
			kill(getppid(), SIGUSR2);
			
			pause();													//Varunk a pecsetelo jelzesere
			sem_wait(semShMem);											//Varunk, hogy az ellenor elengedje az osztott memoria szemaforjat (igazabol mar nem kell varni, mert ekkor mar el van engedve, de a sem_wait-el zaroljuk is)
			
			//Megerkeztek az azonositok es a szavazati jogok
			int numOfIDs = numbers[0];									//Az azonositok szama az osztott memoria elso erteke
			
			printf("Pecsetelo - Az alabbi szemelyek szavazhatnak / nem szavazhatnak:\n");
			
			int valid = 0;												//Azok szama, akik ervenyesen szavazhattak
			int wasPresentValid = 0;									//Azok szama, akik megjelentek es ervenyesen szavazhattak
			int invalid = 0;											//Azok szama, akik ervenytelenul szavazhattak
			int wasPresentInvalid = 0;									//Azok szama, akik megjelentek es ervenytelenul szavazhattak
			
			int* voteResults = malloc(numOfIDs * sizeof(int));			//Szavazasok eredmenye
			
			int i = 0;
			
			for (i; i != numOfIDs; ++i)
			{
				if (numbers[i + numOfIDs + 1] == 1)						//Az elso ertek az ID-k szama, igy +1 es mivel az ID-k utan jonnek a szavazati jogok, igy +numOfIDs
				{
					printf("\t%i - szavazhat\n", numbers[i + 1]);		//Itt is +1, mivel az elso ertek a numOfIDs
					
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
					printf("\t%i - nem szavazhat\n", numbers[i + 1]);
					
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

			numbers[(2 * numOfIDs) + 1] = valid;						//Ervenyesen szavazok szamanak elkuldese az elnoknek (0. a numOfIDs, majd numOfIDs db ID, numOfIDs db szavazati jog, s ez utan johet az uj ertek)
			numbers[(2 * numOfIDs) + 2] = wasPresentValid;				//A megjelent, ervenyesen szavazok szamanak elkuldese az elnoknek
			numbers[(2 * numOfIDs) + 3] = invalid;						//Ervenytelenul szavazok szamanak elkuldese az elnoknek
			numbers[(2 * numOfIDs) + 4] = wasPresentInvalid;			//A nem megjelent, ervenytelenul szavazok szamanak elkuldese az elnoknek
			
			sem_post(semShMem);											//Elengedjuk a szemafort, az elnok most mar olvashat a memoriabol
			kill(getppid(), SIGUSR2);									//Jelzunk az elnoknek
			
			sleep(2);
			
			//A szavazas eredmenyenek elkuldese
			int j = 0;
			
			for(j; j != numOfIDs; ++j)
			{
				numbers[j] = voteResults[j];
			}
			
			kill(getppid(), SIGUSR2);									//Jelzunk az elnoknek
			
			leaveTheRoom();												//Meghivjuk a szoba elhagyasat kezelo fuggvenyt*/
			
			free(voteResults);
			shmdt(numbers);												//Elengedjuk az osztott memoriat
			
			exit(0);
		}
	}
	else
	{ 
		//1. gyermek folyamat (ellenor)
		printf("Ellenor - Keszen allok a munkara.\n");
		kill(getppid(), SIGUSR1);
		
		sem_wait(semShMem);												//Varunk, hogy az elnok elengedje az osztott memoria szemaforjat
		
		//Megerkeztek az azonositok
		int numOfIDs = numbers[0];										//Az azonositok szama az osztott memoria elso erteke
		
		printf("Ellenor - Megkaptam az azonositokat: ");
		
		int i = 0;
		
		for (i; i != numOfIDs; ++i)
		{
			(i < numOfIDs - 1) ? printf("%i, ", numbers[i + 1]) : printf("%i\n", numbers[i + 1]);
		}
		
		pid_t pecsetelo = (pid_t)numbers[numOfIDs + 1];					//Az ID-k utan talalhato a pecsetelo pid_t-je (+1, mivel az ID-k 1-tol kezdodnek es nem 0-tol)
		numbers[numOfIDs + 1] = 0;										//Visszaallitjuk 0-ra, hogy ne keruljon veletlenul mashova
		
		//Szavazas jogosultsaganak eldontese es elkuldese a pecsetelonek
		int j = 0;
		
		for (j; j != numOfIDs; ++j)
		{
			//20% valoszinuseggel hibas az azonosito (azaz ha az 5 szam kozul a 4-es jon ki, akkor hibas)
			if ((rand() % 5) == 4)
			{
				numbers[j + numOfIDs + 1] = 0;							//Ha 0, akkor nem tud szavazni (az elso ertek az ID-k szama, majd az ID-k jonnek, s csak ezek utan a 0)
			}
			else
			{
				numbers[j + numOfIDs + 1] = 1;							//Ha 1, akkor tud szavazni
			}
		}
		
		sem_post(semShMem);												//Elengedjuk a szemafort, a pecsetelo most mar olvashat a memoriabol
		kill(pecsetelo, SIGUSR1);
		
		shmdt(numbers);													//Elengedjuk az osztott memoriat
		
		leaveTheRoom();													//Meghivjuk a szoba elhagyasat kezelo fuggvenyt
		
		exit(0);
	}
    
	shmctl(shMemID, IPC_RMID, NULL);									//Osztott memoria torlese
	szemafor_torles(sem_nev);											//Szemafor torlese
	szemafor_torles("/semShMem810");									//Szemafor torlese
	
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