// 4. gyakorlat. Feladatmegold�s
	// Feladat: V�lasztunk! Egy v�laszt�si k�rzetben 3 tagb�l �ll a v�laszt�si bizotts�g: az eln�kb�l (sz�l�) �s k�t tagb�l (gyerekek). Az els� gyerek ellen�rzi a v�laszt�k adatait, a m�sodik pedig lepecs�teli �s kiadja a v�laszt�si lapot.
// 1) Az eln�k megv�rja, am�g mindk�t tag nem jelzi, hogy k�sz a munk�ra (jelz�ssel), ezut�n a parancssori argumentumokon kereszt�l "fogadja" a szavaz�kat (h�ny darab �rkezik). A sz�l� gener�ljon minden szavaz�hoz egy v�letlen azonos�t�sz�mot! A v�laszt�k azonos�t�sz�mait egy cs�v�n kereszt�l tov�bb�tja az adatellen�rz� tagnak, aki ki�rja, hogy fogadta az adatokat �s ki�rja a k�perny�re. A sz�l� bev�rja a gyerekek befejez�s�t. 

// 2) Az ellen�rz�st v�gz� tag a v�laszt�i azonos�t�shoz adatellen�rz�st v�gez. Minden azonos�t� 20% es�llyel hib�s. A hib�s azonos�t�j� v�laszt� nem szavazhat. Az ellen�rz�st v�gz� gyerek �gy a szavazhat/nem szavazhat jelz�ssel cs�vezet�kbe �rja az adatokat, majd err�l jelz�st k�ld a v�laszt�si lapokat kezel� testv�r�nek, aki ezt a k�perny�re �rja. A v�laszt�si lapokat kezel� testv�r egy �sszes�t�st v�gez, megadja, hogy h�ny �rv�nyes/�rv�nytelen szavaz� jelent meg, majd ezt az �sszes�tett adatot tov�bb�tja cs�v�n kereszt�l a sz�l�nek, aki err�l jegyz�k�nyvet (�rv�nyes-�rv�nytelenek sz�zal�kos ar�nya) k�sz�t egy sz�veges f�jlba. A sz�l� ezt a k�perny�re is ki�rja. 

// 3) Az �rv�nyes iratokkal rendelkez� v�laszt�, v�laszt�si lapot kap, majd szavaz (a 2. gyerekben).  A valah�ny (4,5,6 b�rmi lehet, konstans) lehets�ges p�rt k�z�l v�letlenszer�en v�laszt egyet, majd az eredm�nyt cs�be �rja, err�l a bizotts�g eln�k�nek jelz�st k�ld. A szavaz�s eredm�ny�t a sz�l� innen olvassa ki �s kihirdeti (ki�rja). 

// 4) Id�nk�nt egyik�k (v�letlenszer�en) kimegy egy id�re - kett�nek azonban folyamatosan a teremben kell maradnia. (szemafor). A t�voz�sokat �s vissza�rkez�seket jegyz�k�nyvezni kell. (Ez m�r nincs�mivel szemafor m�g nem volt!)



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
		printf("%s Visszaj�ttem.\n", label);
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
		puts("A programot 1 argumentummal kell elind�tani: a szavaz�k sz�m�val.");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	
	int pipefd1[2];
	int pipefd2[2];
	int pipefd12[2];
	if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1 || pipe(pipefd12) == -1)
	{
		perror("Hiba a cs� nyit�sakor\n");
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
			printf("[SZ] �rv�nyes szavaz�k ar�nya: %f%%.\n", percent);
			
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
				perror("Hiba a f�jl nyit�sakor.\n");
			
			puts("\n[SZ] A szavaz�s eredm�nye:");
			write(output, "A szavaz�s eredm�nye:\n", strlen("A szavaz�s eredm�nye:\n"));
			for (i = 0; i < parties; ++i)
			{
				printf("%i. p�rt: %i szavazat\n", i, votes[i]);

				char buffer[10];
				sprintf(buffer, "%i", i);
				write(output, buffer, strlen(buffer));
				write(output, ". p�rt: ", strlen(". p�rt: "));
				sprintf(buffer, "%i", votes[i]);
				write(output, buffer, strlen(buffer));
				write(output, " szavazat\n", strlen(" szavazat\n"));
				have_a_break(semid, "[SZ]");
			}
			
			close(output);
			
			waitpid(child1);
			waitpid(child2);
			
			delete_semaphore(semid);
			
			puts("\n[SZ] V�ge.");
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
				// el�g norm�l UNIX jelz�s, mert csak egyet k�ld�nk
				// de: ne ugyanazt a jelz�st k�ldje a k�t gyerek!
				
			pause();
				
			int db;
			read(pipefd1[0], &db, sizeof(db));
			
			int ids[db];
			read(pipefd1[0], ids, sizeof(ids));
			close(pipefd1[0]);
			
			puts("\n[GY1] Adatok fogadva. A szavaz�k azonos�t�i:");
			
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
			
			puts("\n[GY1] V�ge.");
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
		
		puts("[GY2] V�ge.");
	}
}
