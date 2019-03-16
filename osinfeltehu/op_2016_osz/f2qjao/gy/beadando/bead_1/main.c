#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

void menuKiir() {
    printf("1 - Új rendezvény indítása\n");
    printf("2 - Rendezvények kilistázása\n");
    printf("3 - Összes rendezvény törlése\n");
    printf("4 - Egy rendezvény törlése\n");
    printf("5 - Vendég jelentkezése\n");
    printf("6 - Összes jelentkező kilistázása\n");
    printf("7 - Egy vendég adatainak módosítása\n");
    printf("8 - Egy vendég törlése\n");
    printf("9 - Összes vendég törlése\n");
    printf("0 - Kilépés\n\n");
}

struct Jelentkezo {
    int id;
    char nev[100];
    char mail[100];
    int rendezveny_azon;
    time_t time;
};

struct Jelentkezo_struct {
    int jelentkezettek_szama;
    int kov_jelentkezett_id;
    struct Jelentkezo jelentkezok[1000];
};

struct Rendezveny {
    int id;
    char nev[100];
};

struct Rendezveny_struct {
    int rendezvenyek_szama;
    int kov_rendezveny_id;
    struct Rendezveny rendezvenyek[1000];
};

int letezik_rendezveny(const struct Rendezveny_struct rendezveny_struct, int rendezveny_id) {
    int count = 0;
    int i = 0;
    for(i = 0; i < rendezveny_struct.rendezvenyek_szama; i++) {
        if(rendezveny_struct.rendezvenyek[i].id == rendezveny_id) {
            count++;
        }
    }
    return count;
}

int letezik_vendeg(const struct Jelentkezo_struct jelentkezo_struct, int vendeg) {
    int i = 0;
    for(i = 0; i < jelentkezo_struct.jelentkezettek_szama; i++) {
        if(jelentkezo_struct.jelentkezok[i].id == vendeg) {
            return i;
        }
    }
    return -1;
}

void vendeg_torlese(struct Jelentkezo_struct* jelentkezo_struct, int vendeg) {
    int id = letezik_vendeg(*jelentkezo_struct, vendeg);

    if(id != -1) {
        int i;
        for(i = id; i < jelentkezo_struct->jelentkezettek_szama - 1; i++) {
            jelentkezo_struct->jelentkezok[i].id = jelentkezo_struct->jelentkezok[i+1].id;
            strcpy(jelentkezo_struct->jelentkezok[id].nev, jelentkezo_struct->jelentkezok[i+1].nev);
            strcpy(jelentkezo_struct->jelentkezok[id].mail, jelentkezo_struct->jelentkezok[i+1].mail);
            jelentkezo_struct->jelentkezok[i].rendezveny_azon = jelentkezo_struct->jelentkezok[i+1].rendezveny_azon;
            jelentkezo_struct->jelentkezok[i].time = jelentkezo_struct->jelentkezok[i+1].time;
        }

        jelentkezo_struct->jelentkezettek_szama--;
    } else {
        printf("Érvénytelen vendég azonosító!\n");
    }
}

void sortores_poz_csere(char* text) {
    while(*text != '\n' && *text != '\0') {
        text++;
    }

    *text = '\0';
}

int main() {
    struct Jelentkezo_struct jelentkezo_struct;
    struct Rendezveny_struct rendezveny_struct;

    // File beolvasása
    int file_jelentkezett = open("jelentkezettek.txt", O_RDONLY); // O_RDONLY
    if(file_jelentkezett < 0) {
        // Nem létezik az állomány
        jelentkezo_struct.jelentkezettek_szama = 0;
        jelentkezo_struct.kov_jelentkezett_id = 1;
    } else {
        // Létezik az állomány
        read(file_jelentkezett, &jelentkezo_struct, sizeof(jelentkezo_struct));
        close(file_jelentkezett);
    }
  

    // File beolvasása
    int file_rendezveny = open("rendezvenyek.txt", O_RDONLY); // O_RDONLY
    if(file_rendezveny < 0) {
        // Nem létezik az állomány
        rendezveny_struct.rendezvenyek_szama = 0;
        rendezveny_struct.kov_rendezveny_id = 1;
    } else {
        // Létezik az állomány
        read(file_rendezveny, &rendezveny_struct, sizeof(rendezveny_struct));
        close(file_rendezveny);
    }


    // MENÜ
    int menu;
    do {
        menuKiir();

        printf("Menu: ");
        scanf("%d", &menu);
        fgetc(stdin);
        printf("\n");

        if(menu == 1) {
            // Új rendezvény
            if(rendezveny_struct.rendezvenyek_szama < 1000) {
                printf("Név: ");
                fgets(rendezveny_struct.rendezvenyek[rendezveny_struct.rendezvenyek_szama].nev, 100, stdin);
                rendezveny_struct.rendezvenyek[rendezveny_struct.rendezvenyek_szama].id = rendezveny_struct.kov_rendezveny_id;
                rendezveny_struct.rendezvenyek_szama++;
                rendezveny_struct.kov_rendezveny_id++;
            } else{
                printf("A program már nem tud több rendezvényt eltárolni!\n");
            }
            printf("\n");
        } else if(menu == 2) {
            // Összes rendezvények kilistázása 
            int i = 0;
            printf("id\tNév\n");
            for(i = 0; i < rendezveny_struct.rendezvenyek_szama; i++) {
                printf("%d\t%s", rendezveny_struct.rendezvenyek[i].id, rendezveny_struct.rendezvenyek[i].nev);
            }

            printf("\n");
        } else if(menu == 3) {
            // Össszes rendezvény törlése
            rendezveny_struct.rendezvenyek_szama = 0;
            jelentkezo_struct.jelentkezettek_szama = 0;

            printf("Az összes rendezvény (és vendég) törölve lett!\n\n");
        } else if(menu == 4) {
            // Egy rendezvény törlése
            
            int rendezveny;

            printf("Rendezvény azonosító: ");
            scanf("%d", &rendezveny);

            if(letezik_rendezveny(rendezveny_struct, rendezveny) == 1) {
                int id = 0;
                for(id = 0; id < jelentkezo_struct.jelentkezettek_szama; id++) {
                    if(jelentkezo_struct.jelentkezok[id].rendezveny_azon == rendezveny) {
                        vendeg_torlese(&jelentkezo_struct, jelentkezo_struct.jelentkezok[id].id);
                        id--;
                    }
                }  

                rendezveny_struct.rendezvenyek_szama--;
                printf("A rendezvény (és a rá jelentkezett vendégek) törölve lettek\n");
            } else {
                printf("Érvénytelen rendezvény azonosító!\n");
            }   
            printf("\n");
        } else if(menu == 5) {
            // Vendég jelentkezése 
            if(jelentkezo_struct.jelentkezettek_szama < 1000) {
                jelentkezo_struct.jelentkezok[jelentkezo_struct.jelentkezettek_szama].id = jelentkezo_struct.kov_jelentkezett_id; 

                printf("Név: ");
                fgets(jelentkezo_struct.jelentkezok[jelentkezo_struct.jelentkezettek_szama].nev, 100, stdin);
                sortores_poz_csere(jelentkezo_struct.jelentkezok[jelentkezo_struct.jelentkezettek_szama].nev);

                printf("E-mail: ");
                fgets(jelentkezo_struct.jelentkezok[jelentkezo_struct.jelentkezettek_szama].mail, 100, stdin);
                sortores_poz_csere(jelentkezo_struct.jelentkezok[jelentkezo_struct.jelentkezettek_szama].mail);

                int rendezveny;

                printf("Rendezvény azonosító: ");
                scanf("%d", &rendezveny);
            
                jelentkezo_struct.jelentkezok[jelentkezo_struct.jelentkezettek_szama].rendezveny_azon = rendezveny;
            
                jelentkezo_struct.jelentkezok[jelentkezo_struct.jelentkezettek_szama].time = time(NULL);

                if(letezik_rendezveny(rendezveny_struct, rendezveny) == 1) {
                    jelentkezo_struct.jelentkezettek_szama++;
                    jelentkezo_struct.kov_jelentkezett_id++;
                } else {
                    printf("Érvénytelen rendezvény azonosító!\n");
                }
            } else {
                printf("A program már nem tud több tagot eltárolni!\n");
            }
            printf("\n");
        } else if(menu == 6) {
            // Összes jelentkező kilistázása
            printf("Id\tNév\tE-mail\tRendezvény azonosító\tJelentkezési idő\n");
            int i = 0;
            for(i = 0; i < jelentkezo_struct.jelentkezettek_szama; i++) {
                printf("%d\t%s\t%s\t%d\t\t\t", jelentkezo_struct.jelentkezok[i].id, jelentkezo_struct.jelentkezok[i].nev, jelentkezo_struct.jelentkezok[i].mail, jelentkezo_struct.jelentkezok[i].rendezveny_azon);
                printf(ctime(&jelentkezo_struct.jelentkezok[i].time));
            }

            printf("\n");
        } else if(menu == 7) {
            // Egy vendég adatainak módosítása
            int vendeg;

            printf("Vendég azonosítója: ");
            scanf("%d", &vendeg);
            fgetc(stdin);

            int id = letezik_vendeg(jelentkezo_struct, vendeg);
            if(id != -1) {
                printf("Új név: ");
                fgets(jelentkezo_struct.jelentkezok[id].nev, 100, stdin);
                sortores_poz_csere(jelentkezo_struct.jelentkezok[id].nev);

                printf("Új e-mail: ");
                fgets(jelentkezo_struct.jelentkezok[id].mail, 100, stdin);
                sortores_poz_csere(jelentkezo_struct.jelentkezok[id].mail);
            } else {
                printf("Érvénytelen vendég azonosító!\n");
            }
            printf("\n");
        } else if(menu == 8) {
            // Egy vendég törlése

            int vendeg;
            printf("Vendég azonosítója: ");
            scanf("%d", &vendeg);
            fgetc(stdin);

            vendeg_torlese(&jelentkezo_struct, vendeg);
            printf("A vendég törölve lett a rendezvényről!\n\n");
        } else if(menu == 9) {
            // Összes vendég törlése
            jelentkezo_struct.jelentkezettek_szama = 0;
            printf("Az összes vendég törölve lett az összes rendezvényről!\n\n");
        } else if(menu == 0) {
            // Kilépés
        } else {
            printf("Érvénytelen menü!\n\n");
        }
    } while(menu != 0);
    
    
    // File kiírása
        file_jelentkezett = open("jelentkezettek.txt", O_WRONLY | O_CREAT, S_IRWXU	); // Lehetne O_WRONLY
		lseek(file_jelentkezett,0,SEEK_SET);
		write(file_jelentkezett, &jelentkezo_struct, sizeof(jelentkezo_struct));
        close(file_jelentkezett);

    // File kiírása
		file_rendezveny = open("rendezvenyek.txt", O_WRONLY | O_CREAT, S_IRWXU); // Lehetne O_WRONLY
		lseek(file_rendezveny,0,SEEK_SET);
		write(file_rendezveny, &rendezveny_struct, sizeof(rendezveny_struct));
        close(file_rendezveny);
		
    return 0;
}
