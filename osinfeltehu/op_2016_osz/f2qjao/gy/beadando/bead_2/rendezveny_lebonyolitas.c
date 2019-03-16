#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include "strukturak.h"

void handler(int signumber) {
	//printf("Signal was arrived!\n");
}

int main() {
	srand(time(NULL));
	signal(SIGUSR1,handler);
	int status;
	
	int pipe_sz[2]; // A szülő erre ír
	int pipe_gy[2]; // A gyerek erre ír
	
	if(pipe(pipe_sz) == -1) {
		printf("Hiba a pipe letrehozasakor!\n");
		exit(-3);
	}
	
	if(pipe(pipe_gy) == -1) {
		printf("Hiba a pipe letrehozasakor!\n");
		exit(-4);
	}
	
	int child = fork();
	if(child < 0) {
		printf("Hiba a fork letrehozasakor!\n");
		exit(-1);
	}
	
	if(child > 0) {
		// Szülő
		
		// Adatok
		struct Jelentkezo_struct jelentkezo_struct;
		struct Rendezveny_struct rendezveny_struct;
		
		// Adatok beolvasása
			// Jelentkezettek file beolvasása
			int file_jelentkezett = open("jelentkezettek.txt", O_RDONLY); // O_RDONLY
			if(file_jelentkezett < 0) {
				// Nem létezik az állomány
				printf("Hiba a \'jelentkezettek.txt\' allomany megnyitasakor!\n");
				exit(-2);
			} else {
				// Létezik az állomány
				read(file_jelentkezett, &jelentkezo_struct, sizeof(jelentkezo_struct));
				close(file_jelentkezett);
			}
			
			// File beolvasása
			int file_rendezveny = open("rendezvenyek.txt", O_RDONLY); // O_RDONLY
			if(file_rendezveny < 0) {
				// Nem létezik az állomány
				printf("Hiba a \'rendezvenyek.txt\' allomany megnyitasakor!\n");
				exit(-3);
			} else {
				// Létezik az állomány
				read(file_rendezveny, &rendezveny_struct, sizeof(rendezveny_struct));
				close(file_rendezveny);
			}
		
		
		// Rendezvényeken való végigiterálás
		int i;
		for(i = 0; i < rendezveny_struct.rendezvenyek_szama; i++) {
			char rendezveny[100]; // Helyszín
			int rendezveny_id = rendezveny_struct.rendezvenyek[i].id;
			strcpy(rendezveny, rendezveny_struct.rendezvenyek[i].hely);
			
			printf("[SZULO] A rendezveny helye: %s\n", rendezveny);
			
			int resztvevok_db = 0;
			struct ResztvevokListaja resztvevok;
			
			// Résztvevők kikeresése
			int j;
			for(j = 0; j < jelentkezo_struct.jelentkezettek_szama; j++) {
				if(jelentkezo_struct.jelentkezok[j].rendezveny_azon == rendezveny_id) {
					strcpy(resztvevok.resztvevo[resztvevok_db].nev, jelentkezo_struct.jelentkezok[j].nev);
					resztvevok_db++;
				}
				
            }
			resztvevok.db = resztvevok_db;
			
			// Lényegi rész - Kommunikáció
			close(pipe_sz[0]);
			close(pipe_gy[1]);
			
			// Helyszín nevének elküldése csövön
			printf("[SZULO] Elkuldi a rendezveny nevet a gyereknek csovon keresztul!\n");
			write(pipe_sz[1], rendezveny, sizeof(rendezveny));
			fflush(NULL);
			
			// Várja, hogy kapjon signal-t a gyerektől
			pause();
			printf("[SZULO] Megkapta a signal-t!\n");
			
			// Elküldi a gyereknek, hogy ki a résztvevők
			printf("[SZULO] Elkuldi a gyereknek, hogy kik a resztvevok!\n");
			write(pipe_sz[1], &resztvevok, sizeof(resztvevok));
			fflush(NULL);
			
			// A jelentésre várakozik
			struct Jelentes jelentes;
			read(pipe_gy[0], &jelentes, sizeof(jelentes));
			printf("[SZULO] Megkapja a jelentest!\n");
			
			printf("[SZULO] Jelentes: \n");
			printf("Siker: %d%\n", jelentes.sikeres);
			for(j = 0; j < jelentes.resztvevok.db; j++) {
				printf("%s", jelentes.resztvevok.resztvevo[j].nev);
				if(jelentes.resztvevok.resztvevo[j].megjelent == 0) {
					// Nem jelent meg
					printf(" nem jelent meg!");
				} else {
					// Megjelent
					printf(" megjelent!");
				}
				printf("\n");
			}
			printf("\n");
			
			// Ellenőrzés, hogy lesz-e több
			int vege;
			if(i == rendezveny_struct.rendezvenyek_szama-1) {
				// Ha
				vege = 1;
				//printf("\n");
			} else {
				vege = 0;
				printf("================================================\n\n");
			}
			
			write(pipe_sz[1], &vege, sizeof(vege));
			fflush(NULL);
		}
		
		waitpid(child, &status, 0);
		close(pipe_sz[1]);
		close(pipe_gy[0]);
	} else {
		// Gyerek
		int parent = getppid();
		
		close(pipe_sz[1]);
		close(pipe_gy[0]);
		
		char rendezveny[100]; // Hely
		struct ResztvevokListaja resztvevok;
		
		int vege = 0;
		while(vege != 1) {
			// Megkapja a rendezvény nevét
			read(pipe_sz[0], rendezveny, sizeof(rendezveny));
			printf("[GYEREK] Megkapta a rendezveny nevet: %s\n", rendezveny);
			
			// Helyszínre utazik
			printf("[GYEREK] A helyszinre utazik...\n");
			sleep(2);
			
			// Megérkezett, signal-t küld a szülőnek
			printf("[GYEREK] Megerkezett a helyszinre, elkuldi a jelzest a szulonek!\n");
			kill(parent, SIGUSR1);
			
			// Várja, hogy megkapja a szülőtől a résztvevők listáját
			read(pipe_sz[0], &resztvevok, sizeof(resztvevok));
			printf("[GYEREK] Megkapta a resztvevok listajat!\n");
			
			
			// Képernyőre írja a résztvevők neveit
			printf("[GYEREK] Resztvevok: \n");
			int i;
			for(i = 0; i < resztvevok.db; i++) {
				printf("%s\n", resztvevok.resztvevo[i].nev);
			}
			printf("\n");
			
			// Várja a vendégeket és levezényli a rendezvényt
			printf("[GYEREK] Varja a vendegeket es levezenyli a rendezvenyt...\n");
			sleep(2);
			
			// Jelentést készít
				// Mennyire volt sikeres
				int sikeres = rand() % 101;
				
				// Ki jelent meg
				for(i = 0; i < resztvevok.db; i++) {
					int random = rand() % 10; // 0..9
					if(random == 0) {
						// Nem jelent meg
						resztvevok.resztvevo[i].megjelent = 0;
					} else {
						// Megjelent
						resztvevok.resztvevo[i].megjelent = 1;
					}
				}
				
				struct Jelentes jelentes;
				jelentes.sikeres = sikeres;
				jelentes.resztvevok = resztvevok;
				
				// Jelentés elküldése a szülőnek
				printf("[GYEREK] Elkuldi a jelentest a szulonek!\n");
				write(pipe_gy[1], &jelentes, sizeof(jelentes));
				fflush(NULL);
			
			// Megkapjuk, hogy van-e még több rendezvény
			read(pipe_sz[0], &vege, sizeof(vege));
		}
		
		close(pipe_sz[0]);
		close(pipe_gy[1]);
		
		exit(0);
	}
	return 0;
}