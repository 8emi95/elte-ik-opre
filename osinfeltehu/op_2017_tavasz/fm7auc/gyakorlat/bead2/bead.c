#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 
#include <unistd.h>  //fork

struct Kerdes {
    int id;
    char kerdes[100];
    char valaszok[10][100];
    time_t felvetel_datum;
};


void menu_megjelenit();
void listaz(struct Kerdes *adatok);
void kerdes_megjelenit(struct Kerdes *adatok);
void kerdes_modosit(struct Kerdes *adatok);
void kerdes_torles(struct Kerdes *adatok, int *vegleges);
void veglegesit(struct Kerdes *adatok, int *vegleges);
void mentes(struct Kerdes *adatok, int *vegleges, char *fajlnev);
void uj_kerdes(struct Kerdes *adatok, int *vegleges);

void mlsz(int counter, char *varos, int *vegleges, struct Kerdes *adatok);

void beolvas(char *fajl, struct Kerdes *adatok, int *vegleges);
time_t convertTime(char *month, int day, int hour, int min, int sec, int year);



int main (int argc, char *argv[]) {
    
    int menu;
    struct Kerdes adatok[20];
    int vegleges[20];
    char fajl[100];
    char varos[100];

    if (argc > 1) {
        strcpy(fajl,argv[1]);
        beolvas(fajl,adatok,vegleges);
    }
    if (argc > 2) {
        strcpy(varos,argv[2]);
    }
    
    menu_megjelenit();

    do {
        scanf("%d", &menu);

        switch (menu) {
        case 1:
            if (adatok[0].id != 0) {
                listaz(adatok);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 2:
            if (adatok[0].id != 0) {
                kerdes_megjelenit(adatok);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 3:
            if (adatok[0].id != 0) {
                kerdes_modosit(adatok);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 4:
            if (adatok[0].id != 0) {            
                kerdes_torles(adatok,vegleges);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 5:
            if (adatok[0].id != 0) {
                veglegesit(adatok,vegleges);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }            
            break;
        case 6:
            if (adatok[0].id != 0) {
                mentes(adatok,vegleges,fajl);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }           
            break;
        case 7:
            uj_kerdes(adatok,vegleges);
            break;
        case 9:
            menu_megjelenit();
            break;
        }
        if(menu!=0){printf("*** Emlekezteto: menu megjelenites 9-es gomb ***\n");}

        int counter = 0;
        while (vegleges[counter] != 0) {
            counter++;
        }
        if (counter > 3) {
            mlsz(counter,varos,vegleges,adatok);
        }

    } while (menu!=0);

}

void menu_megjelenit() {
    printf("MLSZ Kerdoiv\n\
    A menupontok szamainak megadasaval navigalhat a funkciok kozott\n\
    1) Kerdesek listazas\n\
    2) Kerdes megjelenites\n\
    3) Kerdes modositas\n\
    4) Kerdes torles\n\
    5) Veglegesites\n\
    6) Mentes\n\
    7) Uj kerdes letrehozas\n\
    9) Menu megjelenites\n\
    0) Kilep\n");
}


void listaz(struct Kerdes *adatok) {
    int i = 0;
    while ( adatok[i].id != 0 ) {
        printf("#%d - %s - %s", adatok[i].id, adatok[i].kerdes, ctime(&adatok[i].felvetel_datum));        
        i++;
    }
}

void kerdes_megjelenit(struct Kerdes *adatok) {
    printf("Melyik kerdest jelenitsuk meg? (Azonosito szam)\n");
    int i;
    scanf("%d", &i);
    i--;
    printf("#%d - %s - %s", adatok[i].id, adatok[i].kerdes, ctime(&adatok[i].felvetel_datum));
    int j = 0;
        while ( strlen(adatok[i].valaszok[j]) != 0) {
            printf("%d) %s\n",j+1,adatok[i].valaszok[j]);
            j++;
        }
    
}

void kerdes_modosit(struct Kerdes *adatok) {
    printf("Adja meg a modositando kerdes sorszamat!\n");
    int i;
    scanf("%d", &i);
    i--;
    printf("%s", adatok[i].kerdes);
    printf("Adja meg az uj kerdest!\n");
    char kerdes[100];
    fflush(stdin);
    scanf("%[^\n]s",kerdes);
    strcpy(adatok[i].kerdes,kerdes);

}

void kerdes_torles(struct Kerdes *adatok, int *vegleges) {
    printf("Adja meg a torlendo kerdes sorszamat!\n");
    int i;
    scanf("%d", &i);
    i--;
    printf("#%d - %s - %s", adatok[i].id, adatok[i].kerdes, ctime(&adatok[i].felvetel_datum));
    int j = 0;
        while ( strlen(adatok[i].valaszok[j]) != 0) {
            printf("%d) %s\n",j+1,adatok[i].valaszok[j]);
            j++;
        }
    printf("Biztosan torli? y / n");
    char valasz;
    scanf("%c", valasz);
    if ( valasz == 'y') {
        do {
            adatok[i].id = adatok[i+1].id;
            strcpy(adatok[i].kerdes,adatok[i+1].kerdes);
            adatok[i].felvetel_datum = adatok[i+1].felvetel_datum;
            int j = 0;
        while ( strlen(adatok[i].valaszok[j]) != 0) {
            strcpy(adatok[i].valaszok[j],adatok[i+1].valaszok[j]);
            j++;
            }
        }
        while ( i <= 18 && adatok[i].id != 0 );
    }
}

void veglegesit(struct Kerdes *adatok, int *vegleges) {
    int i = 0;
    while ( adatok[i].id != 0 ) {
        printf("#%d - %s - %s", adatok[i].id, adatok[i].kerdes, ctime(&adatok[i].felvetel_datum));             
        i++;
    }
    printf("Mely kerdes(ek) keruljenek veglegesitesre? Csak szamot irjon szokozzel elvalasztva!\n");
    fflush(stdin);
    int marszerepel;
    int buffer;
    do {
        buffer = getchar();
        if (buffer == ' ') {continue;}
        else if (buffer == '\n') {break;}
        i = 0;
        marszerepel = 0;
        while (vegleges[i] != 0) {
            if (vegleges[i] == buffer) {
                marszerepel = 1;
                break;
            }
            i++;
        }
        if (marszerepel == 0) {
            vegleges[i] = buffer - '0';

        }
    } while (1);
}

void mentes(struct Kerdes *adatok, int *vegleges, char *fajlnev) {
    FILE *f;
    if (strlen(fajlnev) == 0) {
        printf("Adja meg a fajl nevet (pl: valami.txt)\n");
        char fajl[100];
        fflush(stdin);
        scanf("%[^\n]s",fajl);        
        f = fopen(fajl, "w+");
    } else {
        f = fopen(fajlnev, "w+");
    }
    // fprintf(f, "%c\n[ %d ]\n\n", name, number);
    int i = 0;    
    while (adatok[i].id != 0) {
        fprintf(f, "%d\t%s\n%s", adatok[i].id, adatok[i].kerdes, ctime(&adatok[i].felvetel_datum));
        int j = 0;
        while ( strlen(adatok[i].valaszok[j]) != 0) {
            fprintf(f, "%d) %s\t", j+1, adatok[i].valaszok[j]);
            j++;
        }
        fprintf(f,"\n");
        i++;
    }
    i = 0;
    fprintf(f,"vegleges\n");
    while (vegleges[i] != 0) {
        fprintf(f, "%d\t", vegleges[i]);
        i++;
    }
   fclose(f);
}

void uj_kerdes(struct Kerdes *adatok, int *vegleges) {
    int i = 0;
    while (adatok[i].id != 0)
    {
        i++;
    }
    adatok[i].id=i+1;
    printf("Adja meg a kerdest!\n");
    char kerdes[100];
    fflush(stdin);
    //fgets(kerdes,100,stdin);
    //scanf("%s",kerdes);
    scanf("%[^\n]s",kerdes);
    strcpy(adatok[i].kerdes,kerdes);

    printf("Hany darab valasz lehetoseg tartozik a kerdeshez?\n");
    int valaszdb;
    fflush(stdin);
    scanf("%d", &valaszdb);
    int j = 0;
    while ( j < valaszdb) {
        printf("Adja meg a %d. valasz lehetoseget\n", j+1);
        char valasz[100];
        //fgets(valasz,100,stdin);
        fflush(stdin);
        scanf("%[^\n]s",valasz);        
        strcpy(adatok[i].valaszok[j],valasz);
        j++;
    }

    adatok[i].felvetel_datum = time(0);
    //printf("#%d - %s - %s", adatok[0].id, adatok[0].kerdes, ctime(&adatok[0].felvetel_datum));

}

void mlsz(int counter,char *fajl, int *vegleges, struct Kerdes *adatok)
{
    int randomKerdes[3];
    int i;
    if (counter == 3) {
        for (i = 0; i<3;i++) {
            randomKerdes[i] = i;
        }
    } else {
        for (i = 0; i<3; i++){
            randomKerdes[i] = rand() % counter;
        }
    }
    
    int status;
    int namedpipe=mkfifo("wasd.asd", S_IRUSR|S_IWUSR);
    int fd;

    pid_t  child=fork();
    
    if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
    if (child>0) //the parent process
    {
        fd=open("wasd.asd", O_RDWR);
        write(fd,randomKerdes,3*sizeof(int));
        close(fd);
        wait(NULL);
        fd=open("wasd.asd", O_RDWR);
        int eredmeny[3][4];
        read(fd,eredmeny,sizeof(eredmeny));
        printf("Az eredmeny:\n");
        int i,j;
        for ( i = 0; i< 3; i++) {
            printf("%s\nVoksok (db szam)\n", adatok[i].kerdes);
            for ( j = 0; j<4; j++) {
                printf("%d", eredmeny[i][j]);
            }
        }
        close(fd);
        unlink("wasd.asd");

    
    }
    else //child process
    {
        int kerdoiv[3][4];
        fd=open("wasd.asd", O_RDWR);
        int kerdesek[3];
        read(fd,kerdesek,3*sizeof(int));
        close(fd);

        int kitolto = (rand() % 20) +1;
        int i,j;
        for (i = 0; i < kitolto; i++) {
            j = 0;
            while (j<3) {
                int valasz = rand() % sizeof(adatok[j].valaszok);
                kerdoiv[j][valasz] = kerdoiv[j][valasz] + 1;
                j++;
            }
        }
        fd=open("wasd.asd", O_RDWR);
        write(fd,kerdoiv,sizeof(kerdoiv));
        close(fd);
    }

}

void beolvas(char *fajl, struct Kerdes *adatok, int *vegleges) {
    FILE *f;
    f = fopen(fajl, "r");

    if (f){
        int i = 0;
        int j;
        char buffer[50];
        while (fscanf(f, "%d\t%[^\n]s", adatok[i].id, adatok[i].kerdes) == 2) {
					// ***************************************** TIME
					char dayoftheweek[3];
					char month[3];
					int day, hour, min, sec, year;
					fscanf(f, "%s %s %d %d:%d:%d %d", dayoftheweek, month, &day, &hour, &min, &sec, &year);
					adatok[i].felvetel_datum = convertTime(month, day, hour, min, sec, year);
					// *****************************************
			j = 0;
            while ((fscanf(f, "%d) %s", buffer, adatok[i].valaszok[j])) == 2)
            {
                j++;
            }
            i++;
            if ( fscanf(f, "%s", buffer) == 1 ) {
                j = 0;
                while ((fscanf(f, "%d", vegleges[j]) == 1)) {
                    j++;
                }
            }
        }
    } else {
        printf("Hiba a fajl megnyitaskor!");
        exit(1);
    }

    fclose(f);
}

time_t convertTime(char *month, int day, int hour, int min, int sec, int year) {
	struct tm ido;

	int ret;
	char buffer[10];
	int honap = 0;
	if (strcmp(month, "Jan")) { honap = 1; }
	else if (strcmp(month, "Feb")) { honap = 2; }
	else if (strcmp(month, "Mar")) { honap = 3; }
	else if (strcmp(month, "Apr")) { honap = 4; }
	else if (strcmp(month, "May")) { honap = 5; }
	else if (strcmp(month, "Jun")) { honap = 6; }
	else if (strcmp(month, "Jul")) { honap = 7; }
	else if (strcmp(month, "Aug")) { honap = 8; }
	else if (strcmp(month, "Sep")) { honap = 9; }
	else if (strcmp(month, "Oct")) { honap = 10; }
	else if (strcmp(month, "Nov")) { honap = 11; }
	else { honap = 12; }
	
	ido.tm_year = year - 1900;
	ido.tm_mon = honap;
	ido.tm_mday = day;
	ido.tm_hour = hour;
	ido.tm_min = min;
	ido.tm_sec = sec;

	ret = mktime(&ido);
	if (ret == -1) {
		printf("Nem sikerult az ido konvertalas!\n");
	}
	else
	{
		return ret;
	}
}

