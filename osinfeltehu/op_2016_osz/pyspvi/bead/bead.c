#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

struct Jelentkezo {
	char nev[80];
	char email[80];
	time_t jelentkezesiIdo;
};

void vendegModosit();
void ujRendezveny();
void ujVendeg();
void vendegekListazasa();
int letezikERendezveny(const char * eleres);
void eleresKeszito(char * eleres, const char * rendezvenyAzon);
int rendezvenySzamlalo();
void vendegTorles();
void rendezvenyTorles();
off_t fileMeret(char * eleres);
void init();

int main() {
	init();
	int valasztott = 0;
	while(1) {
		printf("Menü:\n");
		printf("1. új rendezvény\n");
		printf("2. új vendég\n");
		printf("3. jelentkezettek listázása\n");
		printf("4. vendég adatainak módósítása\n");
		printf("5. vendég törlése\n");
		printf("6. rendezvény törlése\n");
		scanf("%d", &valasztott);
		switch (valasztott) {
			case 1:
				ujRendezveny();
				break;
			case 2:
				ujVendeg();
				break;
			case 3:
				vendegekListazasa();
				break;
			case 4:
				vendegModosit();
				break;
			case 5:
				vendegTorles();
				break;
			case 6:
				rendezvenyTorles();
		}
	}
	return 0;
}

void ujRendezveny(){
	char rendezvenyAzon[80];
	printf("Adja meg a rendezvény azonosítóját:");
	scanf("%s", rendezvenyAzon);
	char eleres[90];
	eleresKeszito(eleres, rendezvenyAzon);
	if (letezikERendezveny(eleres) != -1) {
		printf("Van már ilyen azonosítójú rendezvény!!!\n");
		return;
	}
	int fd = creat(eleres, S_IRUSR|S_IWUSR);
	close(fd);
	
}

void ujVendeg() {
	char rendezvenyAzon[80];
	printf("Adja meg a rendezvény azonosítóját:");
	scanf("%s", rendezvenyAzon);
	char eleres[90];
	eleresKeszito(eleres, rendezvenyAzon);
	if (letezikERendezveny(eleres) == -1) {
		printf("Nem létezik ilyen azonosítójú rendezvény!!!\n");
		return;
	}
	fileMeret(eleres);
	int sorszam = (fileMeret(eleres) / sizeof(struct Jelentkezo));
	char nev[80];
	char email[80];
	time_t ido;
	printf("Adja meg a nevét:\n");
	scanf("%s", nev);
	printf("Adja meg a mail címét:\n");
	scanf("%s", email);
	time(&ido);
	int fd = open(eleres, O_APPEND|O_WRONLY);
	struct Jelentkezo ember ;
	strcpy(ember.nev, nev);
	strcpy(ember.email, email);
	ember.jelentkezesiIdo = ido;
	write(fd, &ember, sizeof(struct Jelentkezo));
	close(fd);
	printf("Sikeres jelentkezés, sorszám: %d\n", (sorszam + 1));
}

void vendegekListazasa(){
	char rendezvenyAzon[80];
	printf("Adja meg a rendezvény azonosítóját:");
	scanf("%s", rendezvenyAzon);
	char eleres[90];
	eleresKeszito(eleres, rendezvenyAzon);
	if (letezikERendezveny(eleres) == -1) {
		printf("Nem létezik ilyen azonosítójú rendezvény!!!\n");
		return;
	}
	int fd = open(eleres, O_RDONLY);
	struct Jelentkezo ember;
	int i = 0;
	struct tm * timeinfo;
	do
	{
		read(fd, &ember, sizeof(struct Jelentkezo));
		timeinfo = localtime(&ember.jelentkezesiIdo);
		printf("%d %s %s %s", (i+1), ember.nev, ember.email, asctime(timeinfo));
		i++;
	} while (i < (fileMeret(eleres) / sizeof(struct Jelentkezo)));
	close(fd);
}
	
int letezikERendezveny(const char * eleres){
	return access(eleres, F_OK);
}

void eleresKeszito(char * eleres, const char * rendezvenyAzon) {
	strcpy(eleres, "adatok/");
	strcat(eleres, rendezvenyAzon);
}

int rendezvenySzamlalo(){
	int fileSzama = 0;
	DIR * dirp;
	struct dirent * entry;
	
	dirp = opendir("adatok");
	if (dirp == NULL) {
		printf("Hiba a az adatok könyvtár megnyitása során!!!\n");
		return -1;
	}
	while ((entry = readdir(dirp)) != NULL) {
		if (entry->d_type == DT_REG) {
			fileSzama++;
		}
	}
	closedir(dirp);
	return fileSzama;
}

void vendegModosit(){
	char rendezvenyAzon[80];
	printf("Adja meg a rendezvény azonosítóját:");
	scanf("%s", rendezvenyAzon);
	char eleres[90];
	eleresKeszito(eleres, rendezvenyAzon);
	if (letezikERendezveny(eleres) == -1) {
		printf("Nem létezik ilyen azonosítójú rendezvény!!!\n");
		return;
	}
	int fd = open(eleres, O_RDONLY);
	struct Jelentkezo emberek [fileMeret(eleres) / sizeof(struct Jelentkezo)];
	struct Jelentkezo ember;
	int i = 0;
	struct tm * timeinfo;
	do
	{
		read(fd, &ember, sizeof(struct Jelentkezo));
		timeinfo = localtime(&ember.jelentkezesiIdo);
		printf("%d %s %s %s", (i+1), ember.nev, ember.email, asctime(timeinfo));
		emberek[i] = ember;
		i++;
	} while (i < (fileMeret(eleres) / sizeof(struct Jelentkezo)));
	printf("Melyik sorszámú vendéget szeretné módosítani:");
	scanf("%d", &i);
	i--;
	char nev[80];
	char email[80];
	printf("Adja meg a nevét:\n");
	scanf("%s", nev);
	printf("Adja meg a mail címét:\n");
	scanf("%s", email);
	strcpy(emberek[i].nev, nev);
	strcpy(emberek[i].email, email);
	close(fd);
	fd = open(eleres, O_WRONLY);
	int j = 0;
	while (j <= i) {
		write(fd, &emberek[j], sizeof(struct Jelentkezo));
		j++;
	}
	close(fd);
}

void vendegTorles() {
	char rendezvenyAzon[80];
	printf("Adja meg a rendezvény azonosítóját:");
	scanf("%s", rendezvenyAzon);
	char eleres[90];
	eleresKeszito(eleres, rendezvenyAzon);
	if (letezikERendezveny(eleres) == -1) {
		printf("Nem létezik ilyen azonosítójú rendezvény!!!\n");
		return;
	}
	int fd = open(eleres, O_RDONLY);
	struct Jelentkezo emberek [fileMeret(eleres) / sizeof(struct Jelentkezo)];
	struct Jelentkezo ember;
	int i = 0;
	struct tm * timeinfo;
	do
	{
		read(fd, &ember, sizeof(struct Jelentkezo));
		timeinfo = localtime(&ember.jelentkezesiIdo);
		printf("%d %s %s %s", (i+1), ember.nev, ember.email, asctime(timeinfo));
		emberek[i] = ember;
		i++;
	} while (i < (fileMeret(eleres) / sizeof(struct Jelentkezo)));
	printf("Melyik sorszámú vendéget szeretné törölni:");
	scanf("%d", &i);
	i--;
	int j = 0;
	close(fd);
	unlink(eleres);
	fd = open(eleres, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
	printf("%d", (sizeof(emberek) / sizeof(struct Jelentkezo)));
	while (j < (sizeof(emberek) / sizeof(struct Jelentkezo))) {
		if(j != i){
			write(fd, &emberek[j], sizeof(struct Jelentkezo));
		}
		j++;
	}
	close(fd);
}

void rendezvenyTorles(){
	char rendezvenyAzon[80];
	printf("Adja meg a rendezvény azonosítóját:");
	scanf("%s", rendezvenyAzon);
	char eleres[90];
	eleresKeszito(eleres, rendezvenyAzon);
	if (letezikERendezveny(eleres) == -1) {
		printf("Nem létezik ilyen azonosítójú rendezvény!!!\n");
		return;
	}
	unlink(eleres);
}
	

off_t fileMeret(char * eleres) {
	struct stat st;
	
	if (stat(eleres, &st) == 0) {
		return st.st_size;
	} else {
		return -1;
	}
}

void init() {
	mkdir("adatok", 0777);
}	
