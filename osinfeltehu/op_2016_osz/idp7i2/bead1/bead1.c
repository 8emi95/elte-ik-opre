#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

#define MAX_LEN 80

struct Rendezveny {
	int azon;
	char nev[MAX_LEN];
};

struct Latogato {
	char nev[MAX_LEN];
	char email[MAX_LEN];
	char ido[MAX_LEN];
};

char fomenupontok[][MAX_LEN] = { "Kilepes", "Latogatok", "Rendezvenyek" };
char latmenupontok[][MAX_LEN] = { "Vissza", "Uj latogato", "Latogatok listazasa", "Latogato adatainak modositasa", "Latogato torlese" };
char rendmenupontok[][MAX_LEN] = { "Vissza", "Uj rendezveny", "Rendezveny torlese" };

int menu(const char cim[], const char m[][MAX_LEN], int n);
void fomenu(const int val);
void latmenu(const int val);
void rendmenu(const int val);
void renduj();
void rendtorl();
int rendval(const char cim[MAX_LEN], struct Rendezveny rendek[MAX_LEN], const int maxazon, const int maxtombazon, const bool cikl, const bool inde); 
bool isnumber(char input[MAX_LEN]);
void latuj();
void latlist();
void latmod();
void lattorl();
void latmod();

int main() {
	fomenu(menu("King of Stands - Fomenu", fomenupontok, sizeof(fomenupontok) / MAX_LEN));
	system("clear");
	return 0;
}

int menu(const char cim[], const char m[][MAX_LEN], int n) {
	size_t i;
	char input[MAX_LEN];
	bool hiba = false;

	do {
		system("clear");
		printf("%s\n\n", cim);
		for(i = 1; i < n; i++) {
			printf("%d - %s\n", i, m[i]);
		}
		printf("%d - %s\n\n", 0, m[0]);
	
		if(hiba) printf("Hibas ertek! Adj meg egy szamot 0 es %d kozott!\n\n", n-1);

		scanf("%s", input);
		hiba = strlen(input) != 1 || !isdigit(input[0]) || atoi(input) >= n;
	} while(hiba);
	return atoi(input);
}

void fomenu(const int val) {
	switch(val) {
		case 0:
			return;
		case 1:
			latmenu(menu("King of Stands - Latogatok", latmenupontok, sizeof(latmenupontok) / MAX_LEN));
			break;
		case 2:
			rendmenu(menu("King of Stands - Rndezvenyek", rendmenupontok, sizeof(rendmenupontok) / MAX_LEN));
			break;
	}
}

void latmenu(const int val) {
	switch(val) {
		case 0:
			fomenu(menu("King of Stands - Fomenu", fomenupontok, sizeof(fomenupontok) / MAX_LEN ));
			break;
		case 1: //uj
			latuj();
			latmenu(menu("King of Stands - Latogatok", latmenupontok, sizeof(latmenupontok) / MAX_LEN));
			break;
		case 2: //lista
			latlist();
			latmenu(menu("King of Stands - Latogatok", latmenupontok, sizeof(latmenupontok) / MAX_LEN));
			break;
		case 3: //mod
			latmod();
			latmenu(menu("King of Stands - Latogatok", latmenupontok, sizeof(latmenupontok) / MAX_LEN));
			break;
		case 4: //torl
			lattorl();
			latmenu(menu("King of Stands - Latogatok", latmenupontok, sizeof(latmenupontok) / MAX_LEN));
			break;
	}
}

void rendmenu(const int val) {
	switch(val) {
		case 0:
			fomenu(menu("King of Stands - Fomenu", fomenupontok, sizeof(fomenupontok) / MAX_LEN));
			break;
		case 1:
			renduj();
			rendmenu(menu("King of Stands - Rendezvenyek", rendmenupontok, sizeof(rendmenupontok) / MAX_LEN));
			break;
		case 2:
			rendtorl();
			rendmenu(menu("King of Stands - Rendezvenyek", rendmenupontok, sizeof(rendmenupontok) / MAX_LEN));
			break;
	}
}

void renduj() {
	FILE* f;
	struct Rendezveny rendek[MAX_LEN];
	int maxazon = -1;
	int maxtombazon = -1;

	system("clear");
	printf("King of Stands - Uj rendezveny\n\n");

	if(f = fopen("rendezvenyek", "rb")) {
		fread(&maxazon, sizeof(int), 1, f);
		fread(&maxtombazon, sizeof(int), 1, f);
		fread(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);
		fclose(f);
	}

	f = fopen("rendezvenyek", "wb");
	maxazon = maxazon + 1;
	maxtombazon = maxtombazon + 1;
	rendek[maxtombazon].azon = maxazon;
	printf("Rendezveny neve:\n");
	scanf("%s", rendek[maxtombazon].nev);
	fwrite(&maxazon, sizeof(int), 1, f);
	fwrite(&maxtombazon, sizeof(int), 1, f);
	fwrite(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);
	fclose(f);
	printf("A rendezveny letrehozasa sikeres\n");

	fflush(stdout);
	sleep(3);
}

void rendtorl() {
	FILE* f;
	struct Rendezveny rendek[MAX_LEN];
	int maxazon, maxtombazon, i, val, valind;
	char input[MAX_LEN], fajlnev[MAX_LEN];

	if(f = fopen("rendezvenyek", "rb")) {
		fread(&maxazon, sizeof(int), 1, f);
		fread(&maxtombazon, sizeof(int), 1, f);
		fread(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);
		fclose(f);

		if(maxtombazon != -1) {
			system("clear");
			valind = rendval("King of Stands - Rendezveny torlese\n\nMelyik rendezvenyt szeretned torolni?", rendek, maxazon, maxtombazon, false, true);
			
			if(valind >= 0) {
				// torles
				f = fopen("rendezvenyek", "wb");
				maxtombazon = maxtombazon-1;
				sprintf(fajlnev, "%d.rend", rendek[valind].azon);
				for(i = valind; i <= maxtombazon; i++) {
					rendek[i] = rendek[i+1];
				}
				fwrite(&maxazon, sizeof(int), 1, f);
				fwrite(&maxtombazon, sizeof(int), 1, f);
				fwrite(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);

				remove(fajlnev);
				printf("Sikeres torles.\n");
			}
		} else {
			printf("Nincs torolheto rendezveny.\n");
		}
	} else {
		printf("Nincs torolheto rendezveny.\n");
	}

	fflush(stdout);
	sleep(3);
}

void lattorl() {
	FILE* f;
	struct Rendezveny rendek[MAX_LEN];
	struct Latogato latok[MAX_LEN];
	int maxazon, maxtombazon, i, val, meret, vallat;
	char input[MAX_LEN], fajlnev[MAX_LEN];
	bool hiba = false;

	printf("King of Stands - Latogato torlese\n\n");

	if(f = fopen("rendezvenyek", "rb")) {
		fread(&maxazon, sizeof(int), 1, f);
		fread(&maxtombazon, sizeof(int), 1, f);
		fread(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);
		fclose(f);

		if(maxtombazon != -1) {
			val = rendval("Melyik rendezvenyen?", rendek, maxazon, maxtombazon, false, false);

			if(val >= 0) {
				sprintf(fajlnev, "%d.rend", val);
				system("clear"); printf("King of Stands - Latogato torlese a %d azonositoju rendezvenyen\n\n", val);

				if(f = fopen(fajlnev, "rb")) {
					fread(&meret, sizeof(int), 1, f);
					fread(&latok, sizeof(struct Latogato), MAX_LEN, f);
					fclose(f);

					if(meret > 0) {
						printf("Melyik latogatot akarod torolni?\n\n");
						for(i = 0; i < meret; i++) {
							printf("%d - %s\n", i, latok[i].nev);
						}
						scanf("%s", input);
						hiba = !isnumber(input);
						if(!hiba) {
							vallat = atoi(input);
							hiba = hiba || vallat < 0 || vallat >= meret;
						}
						if(hiba) {
							printf("Hibas ertek! A torleshez egy latogato azonositojat kell megadni!\n");
						} else {
							meret = meret - 1;
							for(i = vallat; i < meret; i++) {
								latok[i] = latok[i+1];
							}

							f = fopen(fajlnev, "wb");
							fwrite(&meret, sizeof(int), 1, f);
							fwrite(&latok, sizeof(struct Latogato), MAX_LEN, f);
							fclose(f);
							
							printf("Latogato torlese sikeres.\n");
						}
					} else {
						printf("Nincs latogato ezen a rendezvenyen.\n");
					}
				} else {
					printf("Nincs latogato ezen a rendezvenyen.\n");
				}
			}
		} else {
			printf("Jelenleg nincs rendezveny.\n");
		}
	} else {
		printf("Jelenleg nincs rendezveny.\n");
	}

	fflush(stdout);
	sleep(3);
}

void latmod() {
	FILE* f;
	struct Rendezveny rendek[MAX_LEN];
	struct Latogato latok[MAX_LEN];
	int maxazon, maxtombazon, i, val, meret, vallat;
	char input[MAX_LEN], fajlnev[MAX_LEN];
	bool hiba = false;

	printf("King of Stands - Latogato adatainak modositasa\n\n");

	if(f = fopen("rendezvenyek", "rb")) {
		fread(&maxazon, sizeof(int), 1, f);
		fread(&maxtombazon, sizeof(int), 1, f);
		fread(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);
		fclose(f);

		if(maxtombazon != -1) {
			val = rendval("Melyik rendezvenyen?", rendek, maxazon, maxtombazon, false, false);

			if(val >= 0) {
				sprintf(fajlnev, "%d.rend", val);
				system("clear"); printf("King of Stands - Latogato adatainak modositasa a %d azonositoju rendezvenyen\n\n", val);

				if(f = fopen(fajlnev, "rb")) {
					fread(&meret, sizeof(int), 1, f);
					fread(&latok, sizeof(struct Latogato), MAX_LEN, f);
					fclose(f);

					if(meret > 0) {
						printf("Melyik latogato adatait akarod modositani?\n\n");
						for(i = 0; i < meret; i++) {
							printf("%d - %s\n", i, latok[i].nev);
						}
						scanf("%s", input);
						hiba = !isnumber(input);
						if(!hiba) {
							vallat = atoi(input);
							hiba = hiba || vallat < 0 || vallat >= meret;
						}
						if(hiba) {
							printf("Hibas ertek! A modositashoz egy latogato azonositojat kell megadni!\n");
						} else {
							printf("Nev (%s): ", latok[vallat].nev); scanf("%s", latok[vallat].nev);
							printf("E-mail (%s): ", latok[vallat].email); scanf("%s", latok[vallat].email);

							f = fopen(fajlnev, "wb");
							fwrite(&meret, sizeof(int), 1, f);
							fwrite(&latok, sizeof(struct Latogato), MAX_LEN, f);
							fclose(f);
							
							printf("Latogato adatainak modositasa sikeres.\n");
						}
					} else {
						printf("Nincs latogato ezen a rendezvenyen.\n");
					}
				} else {
					printf("Nincs latogato ezen a rendezvenyen.\n");
				}
			}
		} else {
			printf("Jelenleg nincs rendezveny.\n");
		}
	} else {
		printf("Jelenleg nincs rendezveny.\n");
	}

	fflush(stdout);
	sleep(3);
}

void latuj() {
	FILE* f;
	struct Latogato latok[MAX_LEN];
	struct Rendezveny rendek[MAX_LEN];
	int maxazon, maxtombazon, val, meret = 0;
	char fajlnev[MAX_LEN];
	time_t nyers;
	struct tm * info;

	system("clear");

	if(f = fopen("rendezvenyek", "rb")) {
		fread(&maxazon, sizeof(int), 1, f);
		fread(&maxtombazon, sizeof(int), 1, f);
		fread(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);
		fclose(f);

		if(maxtombazon != -1) {
			val = rendval("King of Stands - Uj latogato\n\nMelyik rendezvenyre?", rendek, maxazon, maxtombazon, false, false);
	
			if(val >= 0) {
				sprintf(fajlnev, "%d.rend", val);
	
				if(f = fopen(fajlnev, "rb")) {
					fread(&meret, sizeof(int), 1, f);
					fread(&latok, sizeof(struct Latogato), MAX_LEN, f);
					fclose(f);
				}

				printf("Nev: "); scanf("%s", latok[meret].nev);
				printf("E-mail: "); scanf("%s", latok[meret].email);

				time(&nyers);
				info = localtime(&nyers);
				strcpy(latok[meret].ido, asctime(info));

				f = fopen(fajlnev, "wb");
				meret = meret + 1;
				fwrite(&meret, sizeof(int), 1, f);
				fwrite(&latok, sizeof(struct Latogato), MAX_LEN, f);
				fclose(f);
				printf("A latogato felvetele %d. helyen sikeresen megtortent.", meret);
			}
		} else {
			printf("Jelenleg nincs rendezveny.\n");
		}
	} else {
		printf("Jelenleg nincs rendezveny.\n");
	}

	fflush(stdout);
	sleep(3);
}

void latlist() {
	FILE *f;
	struct Latogato latok[MAX_LEN];
	struct Rendezveny rendek[MAX_LEN];
	int val, i, maxazon, maxtombazon, meret;
	char fajlnev[MAX_LEN];

	if(f = fopen("rendezvenyek", "rb")) {
		fread(&maxazon, sizeof(int), 1, f);
		fread(&maxtombazon, sizeof(int), 1, f);
		fread(&rendek, sizeof(struct Rendezveny), MAX_LEN, f);
		fclose(f);

		if(maxtombazon != -1) {
			val = rendval("King of Stands - Latogatok\n\nMelyik rendezvenyhez?", rendek, maxazon, maxtombazon, false, false);
			
			if(val >= 0) {
				sprintf(fajlnev, "%d.rend", val);

				if(f = fopen(fajlnev, "rb")) {
					fread(&meret, sizeof(int), 1, f);
					fread(&latok, sizeof(struct Latogato), MAX_LEN, f);
					fclose(f);
				
					if(meret != 0) {
						system("clear");
						printf("King of Stands - Latogatok\n\n%d azonositoju rendezveny\n\n#\t\tNev\t\tE-mail\t\tIdo\n", val);
						for(i = 0; i < meret; i++) {
							printf("%d\t\t%s\t\t%s\t\t%s\n", i+1, latok[i].nev, latok[i].email, latok[i].ido);
						}
						printf("\nNyomd meg az entert a visszalepeshez...\n");
						getchar();
						getchar();
						return;
					} else {
						printf("Nincs latogato ezen a rendezvenyen.\n");
					}
				} else {
					printf("Nincs latogato ezen a rendezvenyen.\n");
				}
			}
		} else {
			printf("King of Stands - Latogatok\n\nJelenleg nincs rendezveny.\n");
		}
	} else {
		printf("King of Stands - Latogatok\n\nJelenleg nincs rendezveny.\n");
	}

	fflush(stdout);
	sleep(3);
}

int rendval(const char cim[MAX_LEN], struct Rendezveny rendek[MAX_LEN], const int maxazon, const int maxtombazon, const bool cikl, const bool inde) {
	char input[MAX_LEN];
	bool megvan, hiba = false;
	int i, val, valind = -1;

	do {
		system("clear");
		printf("%s\n\n", cim);
		for(i = 0; i <= maxtombazon; i++) printf("%d - %s\n", rendek[i].azon, rendek[i].nev);
		printf("\n");

		scanf("%s", input);
		hiba = !isnumber(input);
		if(!hiba) {
			val = atoi(input);
			megvan = rendek[0].azon == val;
			if(megvan) valind = 0;
			for(i = 1; (i <= maxtombazon) && !megvan && val > rendek[i-1].azon; i++) {
				printf("%d\n", i); fflush(stdout);
				megvan = megvan || (rendek[i].azon==val);
				if(rendek[i].azon == val) valind = i;
			}
			hiba = hiba || !megvan;
		}
		if(hiba) printf("Hibas ertek!\nA fenti azonositoszamok kozul adj meg egyet!\n\n");
	} while(!hiba && cikl);

	if(inde) return valind;
	else return val;
}

bool isnumber(char input[MAX_LEN]) {
	int i;
	int hossz = strlen(input);

	for(i = 0; i < hossz; i++) {
		if(!isdigit(input[i])) {
			return false;
		}
	}
	return true;
}

