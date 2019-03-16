// 4. gyakorlat. Feladatmegoldás
	// Feladat: Választunk! Egy választási körzetben 3 tagból áll a választási bizottság: az elnökbõl (szülõ) és két tagból (gyerekek). Az elsõ gyerek ellenõrzi a választók adatait, a második pedig lepecsételi és kiadja a választási lapot.
// 1) Az elnök megvárja, amíg mindkét tag nem jelzi, hogy kész a munkára (jelzéssel), ezután a parancssori argumentumokon keresztül "fogadja" a szavazókat (hány darab érkezik). A szülõ generáljon minden szavazóhoz egy véletlen azonosítószámot! A választók azonosítószámait egy csövön keresztül továbbítja az adatellenõrzõ tagnak, aki kiírja, hogy fogadta az adatokat és kiírja a képernyõre. A szülõ bevárja a gyerekek befejezését. 

// 2) Az ellenõrzést végzõ tag a választói azonosításhoz adatellenõrzést végez. Minden azonosító 20% eséllyel hibás. A hibás azonosítójú választó nem szavazhat. Az ellenõrzést végzõ gyerek így a szavazhat/nem szavazhat jelzéssel csõvezetékbe írja az adatokat, majd errõl jelzést küld a választási lapokat kezelõ testvérének, aki ezt a képernyõre írja. A választási lapokat kezelõ testvér egy összesítést végez, megadja, hogy hány érvényes/érvénytelen szavazó jelent meg, majd ezt az összesített adatot továbbítja csövön keresztül a szülõnek, aki errõl jegyzõkönyvet (érvényes-érvénytelenek százalékos aránya) készít egy szöveges fájlba. A szülõ ezt a képernyõre is kiírja. 

// 3) Az érvényes iratokkal rendelkezõ választó, választási lapot kap, majd szavaz (a 2. gyerekben).  A valahány (4,5,6 bármi lehet, konstans) lehetséges párt közül véletlenszerûen választ egyet, majd az eredményt csõbe írja, errõl a bizottság elnökének jelzést küld. A szavazás eredményét a szülõ innen olvassa ki és kihirdeti (kiírja). 

// 4) Idõnként egyikük (véletlenszerûen) kimegy egy idõre - kettõnek azonban folyamatosan a teremben kell maradnia. (szemafor). A távozásokat és visszaérkezéseket jegyzõkönyvezni kell. (Ez már nincs…mivel szemafor még nem volt!)



#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>

void handler(int signumber) {}

int create_semaphore(const char* pathname, int semaphore_value)
{
	int semid;
	
	key_t key = ftok(pathname, 1);
	if ((semid = semget(key, 1, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0)
		perror("semget");
	
	if (semctl(semid, 0, SETVAL, semaphore_value) < 0)
		perror("semctl");
	
	return semid;
}

void semaphore_operation(int semid, int op)
{
	struct sembuf operation;
	
	operation.sem_num = 0;
	operation.sem_op = op;
	operation.sem_flg = 0;
	
	if (semop(semid, &operation, 1) < 0)
		perror("semop");
}

void have_a_break(int semid, const char* label)
{
	if (rand() % 10 == 0)
	{
		semaphore_operation(semid, -1);
		printf("%s Kimentem.\n", label);
		sleep(3);
		printf("%s Visszajöttem.\n", label);
		semaphore_operation(semid, 1);
	}
}

void delete_semaphore(int semid)
{
	semctl(semid, 0, IPC_RMID);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		puts("A programot 1 argumentummal kell elindítani: a szavazók számával.");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	
	int pipefd1[2];
	int pipefd2[2];
	int pipefd12[2];
	if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1 || pipe(pipefd12) == -1)
	{
		perror("Hiba a csõ nyitásakor\n");
		exit(EXIT_FAILURE);
	}
	
	const int parties = 6;
	
	key_t key = ftok(NULL, 1);
	int sh_mem_id = shmget(key, IPC_CREAT|S_IRUSR|S_IWUSR);
	// char* s = shmat(sh_mem_id, NULL, 0);
	int semid = create_semaphore(argv[0], 1);

	pid_t child2 = fork();
	
	if (child2 > 0) // parent
	{
		pid_t child1 = fork();
		
		if (child1 > 0) // parent
		{
			puts("[SZ] Elindult.");
			
			close(pipefd1[0]);
			close(pipefd2[1]);
			close(pipefd12[0]);
			close(pipefd12[1]);
			
			sigpause(SIGUSR1);
			sigpause(SIGUSR2);

			int db = atoi(argv[1]);

			int i;
			int ids[db];
			srand(getpid()); // getpid() vagy time(0)
			for (i = 0; i < db; ++i)
			{
				ids[i] = rand();
				have_a_break(semid, "[SZ]");
			}
			
			write(pipefd1[1], &db, sizeof(db));
			write(pipefd1[1], ids, sizeof(ids));
			close(pipefd1[1]);
			
			sleep(1);
			kill(child1, SIGUSR1);
						
			int valid, invalid;
			read(pipefd2[0], &valid, sizeof(valid));
			read(pipefd2[0], &invalid, sizeof(invalid));
			
			double percent = (double)100 * valid / db;
			printf("[SZ] Érvényes szavazók aránya: %f%%.\n", percent);
			
			pause();
			int votes[parties];
			for (i = 0; i < parties; ++i)
			{
				votes[i] = 0;
				have_a_break(semid, "[SZ]");
			}
			
			for (i = 0; i < valid; ++i)
			{
				int vote;
				read(pipefd2[0], &vote, sizeof(vote));
				++votes[vote];
				have_a_break(semid, "[SZ]");
			}
			close(pipefd2[0]);
			
			int output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
			if (output < 0)
				perror("Hiba a fájl nyitásakor.\n");
			
			puts("\n[SZ] A szavazás eredménye:");
			write(output, "A szavazás eredménye:\n", strlen("A szavazás eredménye:\n"));
			for (i = 0; i < parties; ++i)
			{
				printf("%i. párt: %i szavazat\n", i, votes[i]);

				char buffer[10];
				sprintf(buffer, "%i", i);
				write(output, buffer, strlen(buffer));
				write(output, ". párt: ", strlen(". párt: "));
				sprintf(buffer, "%i", votes[i]);
				write(output, buffer, strlen(buffer));
				write(output, " szavazat\n", strlen(" szavazat\n"));
				have_a_break(semid, "[SZ]");
			}
			
			close(output);
			
			waitpid(child1);
			waitpid(child2);
			
			delete_semaphore(semid);
			
			puts("\n[SZ] Vége.");
		}
		else // child1, data validation (is aware of child2)
		{
			puts("[GY1] Elindult.");
			
			close(pipefd1[1]);
			close(pipefd2[0]);
			close(pipefd2[1]);
			close(pipefd12[0]);
			
			sleep(1);
			kill(getppid(), SIGUSR1);
				// elég normál UNIX jelzés, mert csak egyet küldünk
				// de: ne ugyanazt a jelzést küldje a két gyerek!
				
			pause();
				
			int db;
			read(pipefd1[0], &db, sizeof(db));
			
			int ids[db];
			read(pipefd1[0], ids, sizeof(ids));
			close(pipefd1[0]);
			
			puts("\n[GY1] Adatok fogadva. A szavazók azonosítói:");
			
			write(pipefd12[1], &db, sizeof(db));
			
			int i;
			srand(time(NULL));
			for (i = 0; i < db; ++i)
			{
				char is_valid = ids[i] % 5 != 0 ? 'v' : 'i';
				write(pipefd12[1], &is_valid, sizeof(is_valid));
				printf("%i\n", ids[i]);
				have_a_break(semid, "[GY1]");
			}
			close(pipefd12[1]);
			
			sleep(1);
			kill(child2, SIGUSR1);
			
			puts("\n[GY1] Vége.");
		}
	}
	else // child2, election sheets
	{
		puts("[GY2] Elindult.");
		
		close(pipefd1[0]);
		close(pipefd1[1]);
		close(pipefd2[0]);
		close(pipefd12[1]);
		
		sleep(1);
		kill(getppid(), SIGUSR2);
		
		pause();
		puts("[GY2] GY1 befejezte.");
		
		int db;
		read(pipefd12[0], &db, sizeof(db));
		
		int i;
		int valid = 0;
		int invalid = 0;
		srand(time(NULL));
		for (i = 0; i < db; ++i)
		{
			char is_valid;
			read(pipefd12[0], &is_valid, sizeof(is_valid));
			if (is_valid == 'v')
				++valid;
			else
				++invalid;
			
			have_a_break(semid, "[GY2]");
		}
		close(pipefd12[0]);
		
		write(pipefd2[1], &valid, sizeof(valid));
		write(pipefd2[1], &invalid, sizeof(invalid));
		
		for (i = 0; i < valid; ++i)
		{
			int vote = rand() % parties;
			write(pipefd2[1], &vote, sizeof(vote));
			have_a_break(semid, "[GY2]");
		}
		close(pipefd2[1]);
		
		sleep(1);
		kill(getppid(), SIGUSR2);
		
		puts("[GY2] Vége.");
	}
}
