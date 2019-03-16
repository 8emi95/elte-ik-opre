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

int digit_to_int(char* d);

int main(int argc, char *argv[])
{
	int numberOfMetros = 0;
	
	if (argc == 1)
	{
		numberOfMetros = digit_to_int(argv[1]);
	}
    
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

int digit_to_int(char* d)
{
 char str[2];

 str[0] = d[0];
 str[1] = '\0';
 return (int) strtol(str, NULL, 10);
}

void printPressEnter(void)
{
	printf("\nPress ENTER to Continue");
	while(getchar() != '\n');
}