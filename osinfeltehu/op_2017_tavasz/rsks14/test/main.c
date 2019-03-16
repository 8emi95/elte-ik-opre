#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

struct KerdesD {
	FILE* fajl;
	char tarolo[1000];
	char fajlnev[100];
} kerdesD;

struct Kerdes {
	int sorszam;
	char kerdes[1000];
	int valaszDB;
	char valaszok[4][500];
};

struct Valasz {
	int valaszado;
	int kerdes;
	int valasz;
	int meret;
};

static struct Kerdes valasztott[3];
static struct Kerdes kerdesek[200];
int kerdesekSzama = 0;

void waitForChild(int signum) {
	if (signum == SIGUSR1) {
		printf("MLSZ: A kerdezobiztos megerkezett!\n");
	}
	if (signum == SIGUSR2) {
		printf("kerdezobiztos: Az MLSZ megkapta a kitoltott kerdoiveket!\n");
		kill(getpid(), SIGKILL);
	}
}

void signalHandler(int signumber) {
    printf("Megerkezett a jel, ezzel a szammal: %i\n",signumber);
	if (signumber == 2)
		exit(1);
}

struct Kerdes* beolvasFajllbol(FILE* file) {
	kerdesekSzama = 0;
	char sor [200];
	
	while(fgets(sor, sizeof(sor) ,file) != NULL) {
		char* adat;
		struct Kerdes tempKerdes;
		int swiccs = -1;
		char* l = sor;
		while((adat = strsep(&l,"#")) != NULL) {
			switch( swiccs ) {
				case -1:
				tempKerdes.sorszam = atoi(adat);
				break;
				case 0:
				strcpy(tempKerdes.kerdes, adat);
				case 1:
				tempKerdes.valaszDB = atoi(adat);
				break;
				case 2:
				strcpy(tempKerdes.valaszok[0] ,adat);
				break;
				case 3:
				strcpy(tempKerdes.valaszok[1] ,adat);
				break;
				case 4:
				if(tempKerdes.valaszDB == 3){
					strcpy(tempKerdes.valaszok[2] ,adat);
				} else if (tempKerdes.valaszDB == 4){ // HIBA LEHET
					strcpy(tempKerdes.valaszok[2] ,adat);
				}
				break;
				case 5:
				if(tempKerdes.valaszDB == 4){
					strcpy(tempKerdes.valaszok[3] ,adat);
				}
				break;
			}
			++swiccs;
		}
		kerdesek[kerdesekSzama] = tempKerdes;
		++kerdesekSzama;
	}
	struct Kerdes* k = kerdesek;
	return k;
}

int randomgem(int min, int max){
    int diff = max-min;
    return (int) (((double)(diff+1)/RAND_MAX) * rand() + min);
}

struct Kerdes* getHaromR(struct Kerdes* beolvasottKerdesek){
	int randomIndex[3];
	randomIndex[0]=randomgem(0,kerdesekSzama-1);
	randomIndex[1]=randomgem(0,kerdesekSzama-1);
	while(randomIndex[0] == randomIndex[1]){
		randomIndex[1]=randomgem(0,kerdesekSzama-1);
	}
	int i = 0;
	while(i < kerdesekSzama-1){
		if(randomIndex[0] != i && randomIndex[1] != i){
			randomIndex[2]=i;
		}
		i++;
	}
	//ez nem a legszebb
	int j = 0;
	while(j<3){
		valasztott[j]=beolvasottKerdesek[randomIndex[j]];
		j++;
	}
	struct Kerdes* retV = valasztott;
	return retV;
}

void veglegesites(char* varos, struct Kerdes* beolvasottKerdesek){
	struct Kerdes* harom_random_kerdes;
	harom_random_kerdes = getHaromR(beolvasottKerdesek);
	int pip1[2];
	int pip2[2];
	int varosCso[2];
	int kerdesCso[2];
	char *uzenet = "MEGERKEZETT";
	char varosBuff[1000];
	struct Kerdes kerdesBuff[1];
	struct Valasz valaszBuff[1];
	pid_t cpid;
	int n;
	int i = 0;
	struct sigaction act;
	union sigval ertek;
	act.sa_flags = SA_SIGINFO;
	
	if (pipe(varosCso) == -1) {
        perror("cso hiba");
        exit(EXIT_FAILURE);
    }
    if (pipe(kerdesCso) == -1) {
        perror("cso hiba");
        exit(EXIT_FAILURE);
    }
    if (pipe(pip1) == -1){
		perror("cso hiba");
        exit(EXIT_FAILURE);
	}
	if (pipe(pip2) == -1) {
        perror("cso hiba");
        exit(EXIT_FAILURE);
    }
	signal(SIGTERM,signalHandler);
    signal(SIGINT,signalHandler);
	cpid = fork();
	if (cpid == -1){
        perror("elagazas hiba");
        exit(EXIT_FAILURE);
    }
	printf("Eredmenyek lekerese ebbol a varosbol: %s...\n", varos);
	if(cpid == 0){
		close(pip1[1]);
		close(pip2[0]);
		close(varosCso[1]);
		ertek.sival_ptr = uzenet;
		sleep(3);
		sigqueue(getppid(), SIGUSR1, ertek);
		read(varosCso[0],varosBuff,sizeof(varosBuff));
		close(varosCso[0]);
		printf("kerdezobiztos - megerkeztem ide: %s\n", varosBuff);
		close(kerdesCso[1]);
		struct Valasz valaszok[3][20];
		int v[3];
		i = 0;
		while(0 < (n = read(kerdesCso[0], kerdesBuff, sizeof(kerdesBuff)))){
			printf("kerdezobiztos - megerkezett kerdes: %d: %s\n", kerdesBuff[0].sorszam, kerdesBuff[0].kerdes);
			//valasz keszites
			v[i] = valaszKrealas(kerdesBuff[0],valaszok[i]);
			i++;
		}
		close(kerdesCso[0]);
		printf("kerdezobiztos - Befejeztuk a valaszok gyujteset... \n");
		printf("kerdezobiztos - Valaszok kuldese az MLSZ-nek... \n");
		
		i = 0;
		int j = 0;
		while(i<3){
			j = 0;
			while(j<v[i]){
				write(pip2[1], &valaszok[i][j], sizeof(valaszok[i][j]));
				j++;
			}
			i++;
		}
		close(pip1[0]);
		close(pip2[1]);
		printf("kerdezobiztos - várakozas arra, hogy az MLSZ megkapja a valaszokat...\n");
		signal(SIGUSR2, waitForChild);
		pause();
	}else{
		close(pip1[0]);
        close(pip2[1]);
        close(varosCso[0]);
		close(kerdesCso[0]);
		printf("MLSZ - varakozas a kerdezobiztosra...\n");
		signal(SIGUSR1, waitForChild);
		pause();
		write(varosCso[1], varos, sizeof(varosBuff));
        close(varosCso[1]);
		write(kerdesCso[1], &harom_random_kerdes[0], sizeof(harom_random_kerdes[0]));
        write(kerdesCso[1], &harom_random_kerdes[1], sizeof(harom_random_kerdes[1]));
        write(kerdesCso[1], &harom_random_kerdes[2], sizeof(harom_random_kerdes[2]));
        close(kerdesCso[1]);
		sleep(3);
		printf("MLSZ - Kerdesek fogadasa a biztostol...\n");
		
		while(0 < (n = read(pip2[0], valaszBuff, sizeof(valaszBuff)))){
			if(valaszBuff[i].valasz != 0){
				printf("MLSZ - Kerdessorszam: %d, valaszado: %d, valasz: %d\n", valaszBuff[0].kerdes, valaszBuff[0].valaszado , valaszBuff[0].valasz);
			}
		}
		sigqueue(cpid, SIGUSR2, ertek);
		printf("MLSZ - Befejezodott a muvelet...\n");
		sleep(2);
		close(pip1[1]);
		close(pip2[0]);
	}
}




int valaszKrealas(struct Kerdes k, struct Valasz* valaszok) {
	//10 es 20 is lehet(?)
	int valaszadok = randomgem(10, 20);
	int i = 0;
	for (i = 0; i < valaszadok; i++) {
		valaszok[i].valaszado = i+1; //hogy ne 0-val kezdodjon a szamozasuk
		valaszok[i].kerdes = k.sorszam;
		valaszok[i].valasz = randomgem(1, k.valaszDB);
		valaszok[i].meret = valaszadok;
	}
	return valaszadok;
}

void kiiras(){
	printf("\n");
	printf(">> Ezt olvastuk be: <<\n");
	printf("\n");
	int i; 
	for (i = 0; i < kerdesekSzama; ++i) {
		if (kerdesek[i].valaszDB == 2)
			printf("%d.: Kerdes: %s Valasz: %s || %s\n", i, kerdesek[i].kerdes, kerdesek[i].valaszok[0], kerdesek[i].valaszok[1]);
		else if (kerdesek[i].valaszDB == 3)
			printf("%d.: Kerdes: %s Valasz: %s  || %s || %s\n", i, kerdesek[i].kerdes, kerdesek[i].valaszok[0], kerdesek[i].valaszok[1], kerdesek[i].valaszok[2]);
		else if(kerdesek[i].valaszDB == 4)
			printf("%d.: Kerdes: %s Valasz: %s || %s || %s || %s\n", i, kerdesek[i].kerdes, kerdesek[i].valaszok[0], kerdesek[i].valaszok[1], kerdesek[i].valaszok[2],kerdesek[i].valaszok[3] );
	}
}


int main(int argc, char** argv) { 
	if (argc < 2) {
		printf("Egy argumentumot kell megadni: a varos nevet ahova menjen a biztos \n");
		return -1;
	}
	if (2 < argc) {
		printf("Egy argumentumot kell megadni: a varos nevet ahova menjen a biztos \n");
		return -1;
	}
	printf(">> Beolvastuk a fajlbol a kerdeseket <<\n");
	int sviccs = -1;
	do {
		strcpy(kerdesD.fajlnev, "kerdeseim.txt");
		kerdesD.fajl = fopen(kerdesD.fajlnev, "ab+");
		struct Kerdes* beolvasottKerdesek = beolvasFajllbol(kerdesD.fajl); 
		
		
		printf(">> Menu <<\n");
		printf(">> 1: kerdesek veglegesitese <<\n");
		printf(">> 2: kiiratasa az összes kerdeseknek <<\n");
		printf(">> 3: kilepes <<\n");
	
		scanf("%d", &sviccs);
		
		switch(sviccs) {
			case 1:
			printf(">> ...veglegesites... << \n");
			veglegesites(argv[1],beolvasottKerdesek);
			break;
			case 2:
			kiiras();
			break;
			case 3:
			printf(">> ...Kilepes... << \n");
			break;
		}
	} while (sviccs != 3);
}