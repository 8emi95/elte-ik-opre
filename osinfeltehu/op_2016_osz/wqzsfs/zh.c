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

#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

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
int *procCreate(int n);

typedef struct Uzenet 
{ 
    long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
    char status[100];
	int maxMetros;
	int metroID;
} Uzenet; 

// sendig a message
int kuld(int uzenetsor, Uzenet uz) 
{ 
    int status; 
     
    status = msgsnd(uzenetsor, &uz, sizeof(Uzenet), 0); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
    if ( status < 0 )
	{
		perror("msgsnd"); 
	}
	
    return 0; 
} 
     
// receiving a message. 
int fogad(int uzenetsor)
{ 
    struct Uzenet uz; 
    int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
    status = msgrcv(uzenetsor, &uz, sizeof(Uzenet), 5, 0); 
     
    if ( status < 0 ) 
        perror("msgsnd"); 
    else
        printf( "En a %i szerelveny vagyok es osszesen %i szerelveny van.\n", uz.mtype, uz.status ); 
    return 0; 
}

int uzenetsor;
sem_t* semMetro;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	signal(SIGUSR1, handler);
	
	//Uzenetsor letrehozasanak
	int /*uzenetsor, */status; 
	key_t kulcs; 

	// msgget needs a key, amelyet az ftok generál 
	//. 
	kulcs = ftok(argv[0],1); 
	//printf ("A kulcs: %d\n",kulcs);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
	
	if ( uzenetsor < 0 ) 
	{ 
	  perror("msgget"); 
	  return 1; 
	}
	
	//////////////////////////////////////////////////////////////////////
	//a,
	/*pid_t gyerek = fork();
	
	if (gyerek > 0)
	{
		//Program
		int numberOfMetros = 1;												//Kezdetben 1 metro van
		
		if (argc == 2)
		{
			numberOfMetros = digit_to_int(argv[1]);							//Parancssorbol megkapjuk, hogy mennyi a max metro
		}
		
		//printf("%i\n", numberOfMetros);
		
		int metroID = rand() % numberOfMetros;
		Uzenet uz = (Uzenet){ 5, "", numberOfMetros, metroID };
		
		kuld(uzenetsor, uz);
		
		pause();															//Varunk a gyerek valaszara
		
		//Megerkezett a valasz
		status = msgrcv(uzenetsor, &uz, sizeof(Uzenet), 5, 0); 
				 
		if (status < 0)
		{
			perror("msgsnd");
		}
		else
		{
			printf("A %i metro allapota: %s.\n", uz.metroID, uz.status);
		}
		
		wait(NULL);
	}
	else
	{
		//Egy szerelveny
		struct Uzenet uz; 
		int status;
		int successfullWork = rand() % 10;							//Ha 9-es fog kijonni, akkor sikertelen volt az uzem
		
		status = msgrcv(uzenetsor, &uz, sizeof(Uzenet), 5, 0); 
		 
		if (status < 0)
		{
			perror("msgsnd");
		}
		else
		{
			if (successfullWork != 9)
			{
				printf("En a %i. szerelveny vagyok es meg %i szerelveny van hatra.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
				uz = (Uzenet){ 5, "Beert", uz.maxMetros, uz.metroID };
			}
			else
			{
				printf("En a %i. szerelveny vagyok es meg %i szerelveny van hatra.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
				uz = (Uzenet){ 5, "Bevontattak", uz.maxMetros, uz.metroID };
			}
			
			kuld(uzenetsor, uz);
			kill(getppid(), SIGUSR1);
		}
		
		exit(0);
	}*/
	//////////////////////////////////////////////////////////////////////
	
	//Osztott memoria letrehozasa
	//key_t kulcs2;
    int shMemID;
	int* numbers;
	
	//kulcs2 = ftok(argv[0], 1);											//A parancs nevevel es az 1 verzioszammal kulcs generalasa
    shMemID = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR);		//Osztott memoria letrehozasa irasra es olvasasra, 500 bajt merettel
    numbers = shmat(shMemID, NULL, 0);									//Csatlakozas az osztott memoriahoz
	
	//Szemafor letrehozasa
    char* sem_nev="/sem810";	
	semMetro = szemafor_letrehozas(sem_nev, 1);
	
	//Program
	int numberOfMetros = 1;												//Kezdetben 1 metro van
	
	if (argc == 2)
	{
		numberOfMetros = digit_to_int(argv[1]);							//Parancssorbol megkapjuk, hogy mennyi a max metro
	}
	
	int i = 0;
	int numOfSpare = 0;
	
	for (i; i != numberOfMetros; ++i)
	{
		int metroStatus;
		pid_t pid = fork();

		if (pid > 0)
		{
			//Szulo
			Uzenet uz = (Uzenet){ 5, "", numberOfMetros, i };
			
			kuld(uzenetsor, uz);
			
			pause();															//Varunk a gyerek valaszara
			
			//Megerkezett a valasz
			status = msgrcv(uzenetsor, &uz, sizeof(Uzenet), 5, 0); 
					 
			if (status < 0)
			{
				perror("msgsnd");
			}
			
			if (strcmp(uz.status, "Bevontattak") == 0)
			{
				//Ha bevontattak, akkor inditunk egy ujat
				++numOfSpare;															//Szamoljuk a helyettesitok szamat
				
				pid_t pid2 = fork();
				
				if (pid2 > 0)
				{
					//Szulo
					Uzenet uz = (Uzenet){ 5, "", numberOfMetros, i };
					
					kuld(uzenetsor, uz);
			
					pause();															//Varunk a gyerek valaszara
					
					//Megerkezett a valasz
					status = msgrcv(uzenetsor, &uz, sizeof(Uzenet), 5, 0); 
							 
					if (status < 0)
					{
						perror("msgsnd");
					}
					else
					{
						printf("A %i metro allapota: %s.\n\n", uz.metroID, uz.status);
					}
					
					wait(NULL);
				}
				else
				{
					//Gyerek (uj szerelveny)
					sem_wait(semMetro);
					numbers[i] = -1;
					
					Uzenet uz; 
					int status;
					int successfullWork = rand() % 10;							//Ha 9-es fog kijonni, akkor sikertelen volt az uzem
					
					status = msgrcv(uzenetsor, &uz, sizeof(Uzenet), 5, 0); 
					 
					if (status < 0)
					{
						perror("msgsnd");
					}
					else
					{
						if (successfullWork != 9)
						{
							printf("En a %i. szerelvenyt helyettesitem.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
							uz = (Uzenet){ 5, "A helyettesito beert", uz.maxMetros, uz.metroID };
						}
						else
						{
							printf("En a %i. szerelvenyt helyettesitem.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
							uz = (Uzenet){ 5, "A helyettesitot bevontattak", uz.maxMetros, uz.metroID };
						}
						
						kuld(uzenetsor, uz);
						kill(getppid(), SIGUSR1);
					}
					
					sem_post(semMetro);
					
					exit(0);
				}
			}
			else
			{
				//Megerkezett a valasz
				if (status < 0)
				{
					perror("msgsnd");
				}
				else
				{
					printf("A %i metro allapota: %s.\n\n", uz.metroID, uz.status);
				}
			}
			
			printf("%i. jarat utazasi ideje: %i\n", i, numbers[i]);
			
			wait(NULL);
		}
		else if (pid == 0)
		{
			//Gyerek (hagyomanyos szerelveny)
			sem_wait(semMetro);
			numbers[i] = rand() % 30;
			
			Uzenet uz; 
			int status;
			int successfullWork = rand() % 10;							//Ha 9-es fog kijonni, akkor sikertelen volt az uzem
			
			status = msgrcv(uzenetsor, &uz, sizeof(Uzenet), 5, 0); 
			 
			if (status < 0)
			{
				perror("msgsnd");
			}
			else
			{
				if (successfullWork != 9)
				{
					printf("En a %i. szerelveny vagyok es meg %i szerelveny van hatra.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
					uz = (Uzenet){ 5, "Beert", uz.maxMetros, uz.metroID };
				}
				else
				{
					printf("En a %i. szerelveny vagyok es meg %i szerelveny van hatra.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
					uz = (Uzenet){ 5, "Bevontattak", uz.maxMetros, uz.metroID };
				}
				
				kuld(uzenetsor, uz);
				kill(getppid(), SIGUSR1);
			}
			
			sem_post(semMetro);

			exit(0);
		}
	}
	
	printf("A napi mentesito jaratok szama: %i.\n", numOfSpare);
	
	int j = 0;
	int sum;
	
	for (j; j != numberOfMetros; ++j)
	{
		sum += numbers[j];
	}
	
	printf("Az atlagos futasi ido: %i.\n", (sum / numberOfMetros));
	
	/*int* childs = procCreate(numberOfMetros);							//A gyermek folyamatok pid-jet tarolo tomb
	
	int metroID = rand() % numberOfMetros;
	Uzenet uz = (Uzenet){ 5, "", numberOfMetros, metroID };
	
	kuld(uzenetsor, uz);
	
	pause();															//Varunk a gyerek valaszara
	
	//Megerkezett a valasz
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0); 
			 
	if ( status < 0 )
	{
		perror("msgsnd");
	}
	else
	{
		printf("A %i metro allapota: %s.\n", uz.metroID, uz.status);
	}*/
	
	shmctl(shMemID, IPC_RMID, NULL);									//Osztott memoria torlese
	szemafor_torles(sem_nev);											//Szemafor torlese
    
    return 0;
}

int *procCreate(int n) 
{
    int *childs = malloc(sizeof *childs * n);
	int i = 0;
	
    for (i; i < n; i++) 
	{
        int pid = fork();
		
        if (pid < 0) 
		{
            perror("Fork Failed!");
            exit(1);
        } 
		else if (pid == 0) 
		{
            return NULL;
        } 
		else 
		{
			struct Uzenet uz; 
			int status;
			int successfullWork = rand() % 10;							//Ha 9-es fog kijonni, akkor sikertelen volt az uzem
			
			status = msgrcv(uzenetsor, &uz, 1024, 5, 0); 
			 
			if ( status < 0 )
			{
				perror("msgsnd");
			}
			else
			{
				if (successfullWork != 9)
				{
					printf("En a %i. szerelveny vagyok es meg %i szerelveny van hatra.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
					uz = (Uzenet){ 5, "Beert", uz.maxMetros, uz.metroID };
				}
				else
				{
					printf("En a %i. szerelveny vagyok es meg %i szerelveny van hatra.\nA muszakomat sikeresen elvegeztem.\n", uz.metroID, (uz.maxMetros - uz.metroID));
					uz = (Uzenet){ 5, "Bevontattak", uz.maxMetros, uz.metroID };
				}
				
				kill(getppid(), SIGUSR1);
			}
			
			exit(0); 
        }
    }

    return childs;
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