#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h> 
#include <math.h> 
#include <sys/ipc.h>
#include <sys/shm.h>

/* függvény deklarációk */
void initSignals();
void process();
void processParent(pid_t child);
void processChild(pid_t parent);

void ujUtasszam();
void ujVillamosokSzama();
void sigint();
void tick(int tickTime);

void createShm();
void writeshm(int* shm, int* buf);
int readshm(int* shm);
//void removeshm(int* shm);
//void changemode(int* shm, char *mode);

/* globális változók */
int gyerekOlvashat = 2;
int szuloOlvashat = 0;
int childVege = 0;
int szuloVege = 0;

key_t key;
int shmId; /* osztott memória ID-ja */
int* shm; /* osztott memória címe */
struct shm_ds *buf;

/* globális konstansok */
const int VILLAMOS_KAPACITAS = 100;
const int TIMER_DELAY = 2;
const int RANDOM_UTASOK = 100;
const int MAX_BUFFER_SIZE = 100;

/* függvények */
int main() {
	initSignals();
	key = ftok(".", 'S');  /* egyedi kulcs létrehozása */
	process();
	
	return 0;
}

void process() {
	pid_t child = fork();

	if (child < 0) {
        perror("fork");
        exit(1);
    }
	else if (child == 0) {	
		processChild(getppid());
	}
	else if (child > 0) {
		processParent(child);
	}
}

void processChild(pid_t parent) {
	createShm();
	//int fd1 = open("fifo", O_RDWR);	
	int villamosokSzama = -1;
	int aktualisUtasszam = -1;
	while (childVege == 0) {		
		if (gyerekOlvashat == 1) {
			printf("##gyerekolvas\n");
			printf("Gyerek:\n");

			aktualisUtasszam = readshm(shm);
			printf("\tUtasok száma: %d\n", aktualisUtasszam);
			
			if (aktualisUtasszam != VILLAMOS_KAPACITAS * villamosokSzama) {
				villamosokSzama = aktualisUtasszam / VILLAMOS_KAPACITAS + 1; /* TODO ceil nem mûködött ez szar most */
			}
			
			writeshm(shm, &villamosokSzama);			
			printf("\tVillamosok száma: %d\n", villamosokSzama);
			
			gyerekOlvashat = 0;	
			kill(parent, SIGUSR2);
		} 
		else if (gyerekOlvashat == 0) {
			printf("##gyerekir\n");	
			gyerekOlvashat = 2;
		}
		fflush(stdout);
	}
	//close(fd1);
}

void processParent(pid_t child) {
	createShm();
	//int fd2 = open("fifo", O_RDWR);
	int utasSzamValtozas = -1;	//mennyien vannak összesen az összes villmaoson
	int villamosokSzama = -1;
	int osszesUtasSzam = -1;
	while (szuloVege == 0) {
		if (szuloOlvashat == 1) {
			printf("##szuloolvas\n");
			villamosokSzama = readshm(shm);
			printf("Szülõ:\n");
			printf("\tUtasok száma: %d\n", osszesUtasSzam);
			printf("\tVillamosok száma: %d\n", villamosokSzama);
			szuloOlvashat = 0;
		}
		else if (szuloOlvashat == 0) {
			printf("##szuloir\n");
			signal(SIGALRM, tick);
			alarm(TIMER_DELAY);
			pause();
			utasSzamValtozas = rand() % RANDOM_UTASOK;
			int pluszVagyMinusz = rand() % 2;
			osszesUtasSzam += utasSzamValtozas * (pluszVagyMinusz == 1 ? 1 : -1);
			if (osszesUtasSzam < 0) /* nem mehet nulla alá az utasok száma */ {
				osszesUtasSzam = 0;
			}
			
			writeshm(shm, &osszesUtasSzam);
			kill(child, SIGUSR1); 
			szuloOlvashat = 2;
		}
		fflush(stdout);
	}
	//close(fd2);
}

void initSignals() {
	signal(SIGUSR1, ujUtasszam);
	signal(SIGUSR2, ujVillamosokSzama);
	signal(SIGINT, sigint);
}

void ujUtasszam() {
	gyerekOlvashat = 1;
	signal(SIGUSR1, ujUtasszam);
}

void ujVillamosokSzama() {
	szuloOlvashat = 1;
	signal(SIGUSR2, ujVillamosokSzama);
}

void sigint() {	
	exit(0);
}

void tick(int tickTime) {
    return;
}

void createShm() {
	/* megnyitás, szükség esetén létrehozás */
    if ((shmId = shmget(key, MAX_BUFFER_SIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1)  {
		printf("az osztott memoriazona letezik - megnyitom\n");
		if ((shmId = shmget(key, MAX_BUFFER_SIZE, 0)) == -1) {
			perror("shmget hiba");
			exit(1);
		}
	}
	else {
        printf("letrehozok egy uj osztott memoriazonat\n");
    }         
	
	/* memóriacím hozzárendelése */ 
	if ((shm = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("shmat hiba");
		exit(1);
	}
}

void writeshm(int* shm, int* buf) {
    memcpy(shm, buf, sizeof(buf));

}

int readshm(int* shm) {   
	int ertek = (int)*shm;
	printf("%d", ertek);
	return ertek;
}
/*
void removeshm(int* shm) {
    shmctl(*shm, IPC_RMID, 0);                     
    printf("torolve\n");
}
*/