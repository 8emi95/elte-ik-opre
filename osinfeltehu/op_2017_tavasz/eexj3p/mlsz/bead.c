#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
	int kerdesId;
	char kerdes[100];
	char idoDatum[20];
	int benneVan; /*ha 1, benne van a kérdőívben, ha 0, akkor nem */
	char valasz1[50];
	char valasz2[50];
	char valasz3[50];
	char valasz4[50];
} Kerdes;

int kerdesekSzama;
Kerdes* kerdesek;

int sorokSzama(FILE* fajl);
void fajlokBeolvasasa(FILE* fajl);
void kerdesekBeolvasasa(FILE* fajl);
void kerdesekMentese(FILE* f);
void kerdesekListazasa();
void kerdesTorlese();
int kerdesKeresese(int id);
void kerdesKiirasaIdAlapjan(int id);
void menu();
void kerdesekVeglegesitese();
void veglegesKerdoiv();
void kerdesFelvetele();
void kerdesSzerkesztese();
int legnagyobbId();

int main() {
	FILE* fajl;
	printf("MLSZ kerdoiv alkalmazasban.\n");
	fajlokBeolvasasa(fajl);
	menu();
	kerdesekMentese(fajl);
	return 0;
}

void fajlokBeolvasasa(FILE* fajl) {
	
	if ((fajl = fopen("kerdesek.txt", "r")) == NULL) {
		perror("Hiba az kerdesek.txt fajl megnyitasakor!\n");
		exit(1);
	}
	kerdesekBeolvasasa(fajl);
	//printf("kerdesek szama: %d\n", kerdesekSzama);
	fclose(fajl);
	//kerdesekListazasa();
	/*if ((fajl = fopen("kerdesek.txt", "w")) == NULL) {
		perror("Hiba az kerdesek.txt fajl megnyitasakor!\n");
		exit(1);
	}
	kerdesTorlese();
	kerdesekMentese(fajl);
	fclose(fajl);*/
}

int sorokSzama(FILE* fajl) {
	int i = 0;
	int ch;
	do {
		ch = fgetc(fajl);
		if (ch == '\n') ++i;	
	} while(ch != EOF);
		rewind(fajl);
		if(i < 1) ++i;	
	return i;
}

void kerdesekBeolvasasa(FILE* fajl) {
	kerdesekSzama = sorokSzama(fajl);
	kerdesek = (Kerdes*) malloc(sizeof(Kerdes) * kerdesekSzama);
	int i;
	for(i = 0; i < kerdesekSzama; ++i) {
		fscanf(fajl, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];\n",
			&kerdesek[i].kerdesId,
			kerdesek[i].kerdes,
			kerdesek[i].idoDatum,
			&kerdesek[i].benneVan,
			kerdesek[i].valasz1,
			kerdesek[i].valasz2,
			kerdesek[i].valasz3,
			kerdesek[i].valasz4
		);
	}
		
		
}

void kerdesekMentese(FILE* fajl) {
	if ((fajl = fopen("kerdesek.txt", "w")) == NULL) {
		perror("Hiba az kerdesek.txt fajl megnyitasakor!\n");
		exit(1);
	}
	int i = 0;
	for(; i < kerdesekSzama; ++i) {
		fprintf(fajl, "%d;%s;%s;%d;%s;%s;%s;%s;\n",
		kerdesek[i].kerdesId,
		kerdesek[i].kerdes,
		kerdesek[i].idoDatum,
		kerdesek[i].benneVan,
		kerdesek[i].valasz1,
		kerdesek[i].valasz2,
		kerdesek[i].valasz3,
		kerdesek[i].valasz4
		);
	}
	fclose(fajl);
}

void kerdesekListazasa() {
  int i = 0;
  for(; i < kerdesekSzama; ++i) {
	  printf("Id: %d;  %s;  %s;  Benne van: %d;  \n",
		kerdesek[i].kerdesId,
		kerdesek[i].kerdes,
		kerdesek[i].idoDatum,
		kerdesek[i].benneVan
		);
		
	printf("\n");
	printf("	a, %s\n",kerdesek[i].valasz1);
	printf("	b, %s\n",kerdesek[i].valasz2);
	
	int igaz1 = strcmp(kerdesek[i].valasz3, "NULL");
	if(igaz1 != 0) {
		printf("	c, %s\n",kerdesek[i].valasz3);
	}
	int igaz2 = strcmp(kerdesek[i].valasz4, "NULL");
	if(igaz2 != 0) {
		printf("	d, %s\n",kerdesek[i].valasz4);
	}
	printf("\n");
  }
    
	
}

/*void kerdesKiirasaIdAlapjan(int id) {
	printf("%d;  %s;  %04d.%02d.%02d.%02d.%02d.%02d;  %d;  %s\n",
		kerdesek[id].kerdesId,
		kerdesek[id].kerdes,
		kerdesek[id].ev,
        kerdesek[id].honap,
        kerdesek[id].nap,
		kerdesek[id].ora,
		kerdesek[id].perc,
		kerdesek[id].masodperc,
		kerdesek[id].benneVan,
		kerdesek[id].valasz
		);
}*/

void kerdesTorlese() {
	printf("\nAdja meg a törlendő kérdés id-t: \n");
	int torolId;
	scanf("%d", &torolId);
	
	printf("A következő kérdés törlése: kerdesId = %d\n", torolId);
	
	int i = kerdesKeresese(torolId);
	
	if(i == kerdesekSzama) {
		printf("  Nem volt ezzel az id-val kérdés!\n");
		return;
	}
	
	Kerdes* tmp = (Kerdes*)malloc(sizeof(Kerdes) * (kerdesekSzama - 1));
	int j = 0;
	for(; j < kerdesekSzama; ++j) {
		if(i == j) {
			continue;
		} else if(j < i) {
			tmp[j].kerdesId = kerdesek[j].kerdesId;
			strcpy(tmp[j].kerdes, kerdesek[j].kerdes);
			strcpy(tmp[j].idoDatum, kerdesek[j].idoDatum);
			tmp[j].benneVan = kerdesek[j].benneVan;
			strcpy(tmp[j].valasz1 ,kerdesek[j].valasz1);
			strcpy(tmp[j].valasz2 ,kerdesek[j].valasz2);
			strcpy(tmp[j].valasz3 ,kerdesek[j].valasz3);
			strcpy(tmp[j].valasz4 ,kerdesek[j].valasz4);
		} else {
			tmp[j-1].kerdesId = kerdesek[j].kerdesId;
			strcpy(tmp[j-1].kerdes, kerdesek[j].kerdes);
			strcpy(tmp[j-1].idoDatum, kerdesek[j].idoDatum);
			tmp[j-1].benneVan = kerdesek[j].benneVan;
			strcpy(tmp[j-1].valasz1 ,kerdesek[j].valasz1);
			strcpy(tmp[j-1].valasz2 ,kerdesek[j].valasz2);
			strcpy(tmp[j-1].valasz3 ,kerdesek[j].valasz3);
			strcpy(tmp[j-1].valasz4 ,kerdesek[j].valasz4);
		}
	}
	kerdesek = tmp;
	--kerdesekSzama;
	
	//kerdesKiirasaIdAlapjan(i);
	
	//int i = kerdesKerdes(torolId);
}

int kerdesKeresese(int id) {
	int i = 0;
	for(; i < kerdesekSzama; ++i) {
		if(id == kerdesek[i].kerdesId){
			return i;
		}
	}
}

void menu() {
	int menuSzama = -1;
	printf("MLSZ kerdoiv alkalmazasban.\n");
	while(menuSzama != 0) {
		printf("\n");
		printf("Irja be a kivan menu szamat: \n");
		printf("1) Osszes kerdes megtekintese\n");
		printf("2) Kerdoiv veglegesitese\n");
		printf("3) Vegleges kerdoiv megtekintese\n");
		printf("4) Kerdes felvetele\n");
		printf("5) Kerdes szerkesztese\n");
		printf("6) Kerdes torlese\n");
		printf("0) Kilepes\n");
		scanf("%i",&menuSzama);
		switch(menuSzama) {
			case 0:;
				break;
			case 1:
				kerdesekListazasa();
				break;
			case 2:
				kerdesekVeglegesitese();
				break;
			case 3:
				veglegesKerdoiv();
				break;
			case 4:
				kerdesFelvetele();
				break;
			case 5:
				kerdesSzerkesztese();
				break;
			case 6:
				kerdesTorlese();
				break;
			default:
				printf("Nem lehetseges menu!!\n");
		}
	}
}

void kerdesekVeglegesitese(){
	printf("Irja be a kivan menu szamat: \n");
};
void veglegesKerdoiv(){
	int i = 0;
	for(; i < kerdesekSzama; ++i) {
			if(kerdesek[i].benneVan == 1) {
						printf("Id: %d;  %s;  %s;  Benne van: %d;  \n",
					kerdesek[i].kerdesId,
					kerdesek[i].kerdes,
					kerdesek[i].idoDatum,
					kerdesek[i].benneVan
					);
					
				printf("\n");
				printf("	a, %s\n",kerdesek[i].valasz1);
				printf("	b, %s\n",kerdesek[i].valasz2);
				
				int igaz1 = strcmp(kerdesek[i].valasz3, "NULL");
				if(igaz1 != 0) {
					printf("	c, %s\n",kerdesek[i].valasz3);
				}
				int igaz2 = strcmp(kerdesek[i].valasz4, "NULL");
				if(igaz2 != 0) {
					printf("	d, %s\n",kerdesek[i].valasz4);
				}
				printf("\n");
		}
	}
};

void kerdesFelvetele(){
	printf("Uj kerdes felvetele: \n");
	char kerdesTmp[100];
	char idoDatumTmp[20];
	int benneVanTmp = 1; /*ha 1, benne van a kérdőívben, ha 0, akkor nem */
	char valasz1Tmp[50];
	char valasz2Tmp[50];
	char valasz3Tmp[50] = "NULL";
	char valasz4Tmp[50] = "NULL";
	
	printf("Kerdes: \n");
	scanf("%s",&kerdesTmp);
	
	int valaszokSzama = 0;
	printf("Hany valasz lehetoseget szeretne megadni/2,3,4/: \n");
	while(valaszokSzama < 2 || valaszokSzama > 4) {
		scanf("%i",&valaszokSzama);
		switch(valaszokSzama) {
				case 2:
					printf("1. valasz: \n");
					scanf("%s",&valasz1Tmp);
					printf("2. valasz: \n");
					scanf("%s",&valasz2Tmp);
					break;
				case 3:
					printf("1. valasz: \n");
					scanf("%s",&valasz1Tmp);
					printf("2. valasz: \n");
					scanf("%s",&valasz2Tmp);
					printf("3. valasz: \n");
					scanf("%s",&valasz3Tmp);
					break;
				case 4:
					printf("1. valasz: \n");
					scanf("%s",&valasz1Tmp);
					printf("2. valasz: \n");
					scanf("%s",&valasz2Tmp);
					printf("3. valasz: \n");
					scanf("%s",&valasz3Tmp);
					printf("4. valasz: \n");
					scanf("%s",&valasz4Tmp);
					break;
				default:
					printf("Nem lehetseges szam!!\n Adja meg meg egyszer: \n");	
		}
	}
	
	int benneLegyen = -1;
	printf("Benne legyen a vegleges kerdoivben? /1 = igen / 0 = nem\n");
	while(benneLegyen < 0 || benneLegyen > 1) {
		scanf("%i",&benneLegyen);
		switch(benneLegyen) {
			case 0:
				benneVanTmp = 0;
				break;
			case 1:
				benneVanTmp = 1;
				break;
			default:
				printf("0: nem, 1:igen\n");
		}
	}
	printf("bennee: %d\n", benneVanTmp);
	
	time_t time_raw_format;
    struct tm * ptr_time;
    char buffer[20];
    time ( &time_raw_format );
    ptr_time = localtime ( &time_raw_format );
    strftime(buffer,20,"%Y.%m.%d.%H.%M.%S.",ptr_time);
	
	//&idoDatumTmp = buffer;
	
	Kerdes* tmp1 = (Kerdes*)malloc(sizeof(Kerdes) * (kerdesekSzama + 1));
	int j = 0;
	for(; j < kerdesekSzama; ++j) {
			tmp1[j].kerdesId = kerdesek[j].kerdesId;
			strcpy(tmp1[j].kerdes, kerdesek[j].kerdes);
			strcpy(tmp1[j].idoDatum, kerdesek[j].idoDatum);
			tmp1[j].benneVan = kerdesek[j].benneVan;
			strcpy(tmp1[j].valasz1 ,kerdesek[j].valasz1);
			strcpy(tmp1[j].valasz2 ,kerdesek[j].valasz2);
			strcpy(tmp1[j].valasz3 ,kerdesek[j].valasz3);
			strcpy(tmp1[j].valasz4 ,kerdesek[j].valasz4);
	}
	free(kerdesek);
	kerdesek = tmp1;
	
	
	strcpy(kerdesek[kerdesekSzama].idoDatum, buffer);
	
	int legId = legnagyobbId();
	kerdesek[kerdesekSzama].kerdesId = (legId+1);
	printf("id: %d\n", kerdesek[kerdesekSzama].kerdesId);
	strcpy(kerdesek[kerdesekSzama].kerdes,kerdesTmp);
	printf("kerdes: %s\n",kerdesek[kerdesekSzama].kerdes);
	//strcpy(kerdesek[kerdesekSzama+1].idoDatum, idoDatumTmp);
	printf("Ido: %s\n",kerdesek[kerdesekSzama].idoDatum);
	kerdesek[kerdesekSzama].benneVan = benneLegyen;
	printf("bennevan: \n",kerdesek[kerdesekSzama].benneVan);
	strcpy(kerdesek[kerdesekSzama].valasz1,valasz1Tmp);
	printf("valasz1: %s\n",kerdesek[kerdesekSzama].valasz1);
	strcpy(kerdesek[kerdesekSzama].valasz2,valasz2Tmp);
	printf("valasz1: %s\n",kerdesek[kerdesekSzama].valasz2);
	strcpy(kerdesek[kerdesekSzama].valasz3,valasz3Tmp);
	printf("valasz1: %s\n",kerdesek[kerdesekSzama].valasz3);
	strcpy(kerdesek[kerdesekSzama].valasz4,valasz4Tmp);
	printf("valasz1: %s\n",kerdesek[kerdesekSzama].valasz4);
	kerdesekSzama++;
};
void kerdesSzerkesztese(){
	kerdesekListazasa();
	printf("Melyik kerdes akarja szerkeszteni? Adja meg a kerdes id-t!\n");
	int id;
	scanf("%d",&id);
	int index = kerdesKeresese(id);
	
	char kerdesTmp[100];
	char valasz1Tmp[50];
	char valasz2Tmp[50];
	char valasz3Tmp[50];
	char valasz4Tmp[50];
	
	int almenu = -1;
	printf("Mit szerkeszt?: \n");
	printf("1, kerdes \n");
	printf("2, Valasz1 \n");
	printf("3, Valasz2 \n");
	printf("4, Valasz3 \n");
	printf("5, Valasz4 \n");
	while(almenu < 0 ) {
		scanf("%i",&almenu);
		switch(almenu) {
				case 1:
					printf("Adja meg az uj kerdest: \n");
					scanf("%s",&kerdesTmp);
					strcpy(kerdesek[index].kerdes,kerdesTmp);
					break;
				case 2:
					printf("Adja meg a uj valaszt: \n");
					scanf("%s",&valasz1Tmp);
					strcpy(kerdesek[index].valasz1,valasz1Tmp);
					break;
				case 3:
					printf("Adja meg a uj valaszt: \n");
					scanf("%s",&valasz2Tmp);
					strcpy(kerdesek[index].valasz2,valasz2Tmp);
					break;
				case 4:
					printf("Adja meg a uj valaszt: \n");
					scanf("%s",&valasz3Tmp);
					strcpy(kerdesek[index].valasz3,valasz3Tmp);
					break;
				case 5:
					printf("Adja meg a uj valaszt: \n");
					scanf("%s",&valasz4Tmp);
					strcpy(kerdesek[index].valasz4,valasz4Tmp);
					break;
				default:
					printf("Nem lehetseges szam!!\n Adja meg meg egyszer: \n");	
		}
	}
};

int legnagyobbId(){
	int legnagyobbId = 0;
	int j = 0;
	for(; j < kerdesekSzama; ++j) {
		if (kerdesek[j].kerdesId > legnagyobbId) {
			legnagyobbId = kerdesek[j].kerdesId;
		}
	
	}
	return legnagyobbId;
};