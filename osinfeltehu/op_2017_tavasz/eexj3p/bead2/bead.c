#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>   
#include <signal.h> 

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_KERDES_SZ 100
#define MAX_VALASZ_SZ 50

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

typedef struct {
	int kerdes1[4]; //valasz az 1. kerdesre
	int kerdes2[4];
	int kerdes3[4];
} Valasz;

int kerdesekSzama;
Kerdes* kerdesek;

Kerdes* validKerdesek;
Kerdes* kerdoiv;

int sorokSzama(FILE* fajl);
void fajlokBeolvasasa(FILE* fajl);
void kerdesekBeolvasasa(FILE* fajl);
void kerdesekMentese(FILE* f);
void kerdesekListazasa();
void kerdesTorlese();
int kerdesKeresese(int id);
void kerdesKiirasaIdAlapjan(int id);
void menu();
void newMenu(FILE* file, char * locationP);
void kerdesekVeglegesitese();
void veglegesKerdoiv();
void kerdesFelvetele();
void kerdesSzerkesztese();
int legnagyobbId();
void validKerdesekValogatasa();
void haromRandomKerdesValogatasa();
int kitoltokSzama();
void felmeresLefuttatasa(char * location);
void kiirEredmeny(int i, Kerdes k, int valaszok[4]);

int main(int argc, char ** argv) {
	if(argc == 2)
	{
        FILE* fajl;
		printf("MLSZ kerdoiv alkalmazasban.\n");
		fajlokBeolvasasa(fajl);
		//menu();
		newMenu(fajl, argv[1]);
		return 0;
    }
    else
    {
        printf("A program iditasakor adja meg parameterkent a varost, ahol a felmerest vegzi!!!!\n");
		return 0;
    }
	/*FILE* fajl;
	printf("MLSZ kerdoiv alkalmazasban.\n");
	fajlokBeolvasasa(fajl);
	//menu();
	newMenu(fajl);
	return 0;*/
}

void fajlokBeolvasasa(FILE* fajl) {
	if ((fajl = fopen("kerdesek.txt", "r")) == NULL) {
		perror("Hiba az kerdesek.txt fajl megnyitasakor!\n");
		exit(1);
	}
	kerdesekBeolvasasa(fajl);
	fclose(fajl);
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
    printf("létrehozva: %s", kerdesek[i].idoDatum);
    printf("\n");
	printf("	1. %s\n",kerdesek[i].valasz1);
	printf("	2. %s\n",kerdesek[i].valasz2);
	
	int igaz1 = strcmp(kerdesek[i].valasz3, "NULL");
	if(igaz1 != 0) {
		printf("	3. %s\n",kerdesek[i].valasz3);
	}
	int igaz2 = strcmp(kerdesek[i].valasz4, "NULL");
	if(igaz2 != 0) {
		printf("	4. %s\n",kerdesek[i].valasz4);
	}
	printf("\n");
  }
    
	
}

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
	
}

int kerdesKeresese(int id) {
	int i = 0;
	for(; i < kerdesekSzama; ++i) {
		if(id == kerdesek[i].kerdesId){
			return i;
		}
	}
}

void newMenu(FILE* file, char * locationP){
	int vege; 
	do {
		printf("Irja be a kivant menu szamat: \n");
		printf("1) Osszes kerdes megtekintese\n");
		printf("2) Kerdoiv veglegesitese\n");
		printf("3) Vegleges kerdoiv megtekintese\n");
		printf("4) Kerdes felvetele\n");
		printf("5) Kerdes szerkesztese\n");
		printf("6) Kerdes torlese\n");
		printf("7) Harom kerdes kuldese a biztosnak\n");
		printf("Irjon be barmit a kilepeshez\n");
		printf("\n\n---------------\n");
		scanf("%d", &vege);
		getchar();
		switch (vege) {
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
			case 7:
				haromRandomKerdesValogatasa();
				felmeresLefuttatasa(locationP);
				break;
			default:
				printf("\n\nNem letezo menupont!\n");
				break;
		}
	 } while (vege != 0);
		kerdesekMentese(file);
}

void kerdesekVeglegesitese(){
	printf("Irja be a kivan menu szamat: \n");
};
void veglegesKerdoiv(){
	int i = 0;
	int validK = countVeglegesKerdesek();
	printf("Ennyi valid kerdes van: %i\n",validK);
	for(; i < kerdesekSzama; ++i) {
			if(kerdesek[i].benneVan == 1) {
						printf("Id: %d;  %s;  %s;Benne van: %d;  \n",
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
	char kerdesTmp[100];
	char valami[50];
	char idoDatumTmp[20];
	int benneVanTmp = 1; /*ha 1, benne van a kérdőívben, ha 0, akkor nem */
	char valasz1Tmp[50];
	char valasz2Tmp[50];
	char valasz3Tmp[50] = "NULL";
	char valasz4Tmp[50] = "NULL";
	
	printf("\nKerdes letrehozasa:\n");
	
	printf("\nAdd meg a kerdest:");
	scanf("%[^\n]s", &kerdesTmp);
	getchar();
	
	
	
	int valaszokSzama = 0;
	printf("\nHany valasztasi lehetoseget szeretne megadni/2,3,4/: \n");
	while(valaszokSzama < 2 || valaszokSzama > 4) {
		scanf("%i",&valaszokSzama);
		getchar();
		switch(valaszokSzama) {
				case 2:
					printf("1. valasz: \n");
					scanf("%[^\n]s",&valasz1Tmp);
					getchar();
					printf("2. valasz: \n");
					scanf("%[^\n]s",&valasz2Tmp);
					getchar();
					break;
				case 3:
					printf("1. valasz: \n");
					scanf("%[^\n]s",&valasz1Tmp);
					getchar();
					printf("2. valasz: \n");
					scanf("%[^\n]s",&valasz2Tmp);
					getchar();
					printf("3. valasz: \n");
					scanf("%[^\n]s",&valasz3Tmp);
					getchar();
					break;
				case 4:
					printf("1. valasz: \n");
					scanf("%[^\n]s",&valasz1Tmp);
					getchar();
					printf("2. valasz: \n");
					scanf("%[^\n]s",&valasz2Tmp);
					getchar();
					printf("3. valasz: \n");
					scanf("%[^\n]s",&valasz3Tmp);
					getchar();
					printf("4. valasz: \n");
					scanf("%[^\n]s",&valasz4Tmp);
					getchar();
					break;
		}
	}
	
	int benneLegyen = -1;
	printf("Benne legyen a vegleges kerdoivben? /1 = igen / 0 = nem\n");
	while(benneLegyen < 0 || benneLegyen > 1) {
		scanf("%i",&benneLegyen);
		getchar();
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

void kkerdesFelvetele(){
	char kerdesTmp[100];
	char valami[50];
	char idoDatumTmp[20];
	int benneVanTmp = 1; /*ha 1, benne van a kérdőívben, ha 0, akkor nem */
	char valasz1Tmp[50];
	char valasz2Tmp[50];
	char valasz3Tmp[50] = "NULL";
	char valasz4Tmp[50] = "NULL";
	int eddigMegadottValaszokSzama = 2;
	int kilep = 0;
	
	printf("\nKerdes letrehozasa:\n");
	
	printf("\nAdd meg a kerdest: ");
	scanf("%[^\n]s", &kerdesTmp);
	getchar();
	
	printf("1. valasz: \n");
	scanf("%[^\n]s",&valasz1Tmp);
	getchar();
					
	printf("2. valasz: \n");
	scanf("%[^\n]s",&valasz2Tmp);
	getchar();
	
	
	do {
		char kerdes3;
		printf("Szeretne meg egy valasz lehetoseget? i:igen/n:nem\n");
		scanf("%c", &kerdes3);
		getchar();
		
		if (kerdes3 != 'i' && kerdes3 != 'I') {
			break;
		}
		
		printf("3. valasz: \n");
		scanf("%[^\n]s", &valasz3Tmp);
		
	}
	while (eddigMegadottValaszokSzama<=4);
	
	
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
	getchar();
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
		getchar();
		switch(almenu) {
				case 1:
					printf("Adja meg az uj kerdest: \n");
					scanf("%[^\n]s",&kerdesTmp);
					getchar();
					strcpy(kerdesek[index].kerdes,kerdesTmp);
					break;
				case 2:
					printf("Adja meg a uj valaszt: \n");
					scanf("%[^\n]s",&valasz1Tmp);
					getchar();
					strcpy(kerdesek[index].valasz1,valasz1Tmp);
					break;
				case 3:
					printf("Adja meg a uj valaszt: \n");
					scanf("%[^\n]s",&valasz2Tmp);
					getchar();
					strcpy(kerdesek[index].valasz2,valasz2Tmp);
					break;
				case 4:
					printf("Adja meg a uj valaszt: \n");
					scanf("%[^\n]s",&valasz3Tmp);
					getchar();
					strcpy(kerdesek[index].valasz3,valasz3Tmp);
					break;
				case 5:
					printf("Adja meg a uj valaszt: \n");
					scanf("%[^\n]s",&valasz4Tmp);
					getchar();
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

int countVeglegesKerdesek(){
	int validKerdesekSzama = 0;
	
	int i = 0;
	for(; i < kerdesekSzama; ++i) {
		if (kerdesek[i].benneVan == 1) {
			++validKerdesekSzama;
		}
	}
	return validKerdesekSzama;
}

void validKerdesekValogatasa(){
	
	int kerdoivValidKerdesekSzama = countVeglegesKerdesek();
	validKerdesek = (Kerdes*) malloc(sizeof(Kerdes) * kerdoivValidKerdesekSzama);
	
	int i = 0;
	int j = 0;
	for(; i < kerdesekSzama; ++i) {
		if (kerdesek[i].benneVan == 1) {
				validKerdesek[j].kerdesId = kerdesek[i].kerdesId;
				strcpy(validKerdesek[j].kerdes, kerdesek[i].kerdes);
				strcpy(validKerdesek[j].idoDatum, kerdesek[i].idoDatum);
				validKerdesek[j].benneVan = kerdesek[i].benneVan;
				strcpy(validKerdesek[j].valasz1 ,kerdesek[i].valasz1);
				strcpy(validKerdesek[j].valasz2 ,kerdesek[i].valasz2);
				strcpy(validKerdesek[j].valasz3 ,kerdesek[i].valasz3);
				strcpy(validKerdesek[j].valasz4 ,kerdesek[i].valasz4);
				j++;
		}
	}
	
	printf("Valid kerdes id-k: \n");
	int y = 0;
	for(; y < kerdoivValidKerdesekSzama; y++) {
		printf("id: %i \n", validKerdesek[y].kerdesId);
	}
}

void haromRandomKerdesValogatasa() {
	kerdoiv = (Kerdes*) malloc(sizeof(Kerdes) * 3);
	int kerdoivValidKerdesekSzama = countVeglegesKerdesek();
	validKerdesekValogatasa();

	
	if (kerdoivValidKerdesekSzama < 3) {
		printf("3 vagy annal tobb valid kerdesnek kell lenni a generalashoz!\n");
		return;
	}
	
	if (kerdoivValidKerdesekSzama == 3) {
		kerdoiv = validKerdesek;
	}
	else {
		int idx = 0;
		
		srand ( time(NULL) );
		for (; idx < 3; idx++) {
			
			int r = rand() % kerdoivValidKerdesekSzama;
			printf("rand: %i\n", r);
			
			kerdoiv[idx].kerdesId = validKerdesek[r].kerdesId;
			strcpy(kerdoiv[idx].kerdes, validKerdesek[r].kerdes);
			strcpy(kerdoiv[idx].idoDatum, validKerdesek[r].idoDatum);
			kerdoiv[idx].benneVan = validKerdesek[r].benneVan;
			strcpy(kerdoiv[idx].valasz1 ,validKerdesek[r].valasz1);
			strcpy(kerdoiv[idx].valasz2 ,validKerdesek[r].valasz2);
			strcpy(kerdoiv[idx].valasz3 ,validKerdesek[r].valasz3);
			strcpy(kerdoiv[idx].valasz4 ,validKerdesek[r].valasz4);
			
			
			//átír
			Kerdes* tmp = (Kerdes*)malloc(sizeof(Kerdes) * (kerdoivValidKerdesekSzama - 1));
			int j = 0;
			for(; j < kerdesekSzama; ++j) {
				if(idx == j) {
					continue;
				} else if(j < idx) {
					tmp[j].kerdesId = validKerdesek[j].kerdesId;
					strcpy(tmp[j].kerdes, validKerdesek[j].kerdes);
					strcpy(tmp[j].idoDatum, validKerdesek[j].idoDatum);
					tmp[j].benneVan = validKerdesek[j].benneVan;
					strcpy(tmp[j].valasz1 ,validKerdesek[j].valasz1);
					strcpy(tmp[j].valasz2 ,validKerdesek[j].valasz2);
					strcpy(tmp[j].valasz3 ,validKerdesek[j].valasz3);
					strcpy(tmp[j].valasz4 ,validKerdesek[j].valasz4);
				} else {
					tmp[j-1].kerdesId = validKerdesek[j].kerdesId;
					strcpy(tmp[j-1].kerdes, validKerdesek[j].kerdes);
					strcpy(tmp[j-1].idoDatum, validKerdesek[j].idoDatum);
					tmp[j-1].benneVan = validKerdesek[j].benneVan;
					strcpy(tmp[j-1].valasz1 ,validKerdesek[j].valasz1);
					strcpy(tmp[j-1].valasz2 ,validKerdesek[j].valasz2);
					strcpy(tmp[j-1].valasz3 ,validKerdesek[j].valasz3);
					strcpy(tmp[j-1].valasz4 ,validKerdesek[j].valasz4);
				}
			}
			validKerdesek = tmp;
			memset(tmp, 0, sizeof tmp);
			
			--kerdoivValidKerdesekSzama;
		}
	}
	
	
}

void ittvagyok() {
	printf("Itt vagyok! Varom a kerdoivet!\n");
}

void megkaptam() {
	printf("Megvan a kerdoiv!\n");
}

void felmeresLefuttatasa(char * location) {
	signal(SIGTERM,ittvagyok);
	signal(SIGUSR1,megkaptam);
	
	Kerdes tmp[3];
	
	Kerdes masolat[3];
	
	Valasz kapottValaszok;
	
	int i = 0;
	for (; i <3; i++) {
		tmp[i].kerdesId = kerdoiv[i].kerdesId;
		strcpy(tmp[i].kerdes, kerdoiv[i].kerdes);
		strcpy(tmp[i].idoDatum, kerdoiv[i].idoDatum);
		tmp[i].benneVan = kerdoiv[i].benneVan;
		strcpy(tmp[i].valasz1 ,kerdoiv[i].valasz1);
		strcpy(tmp[i].valasz2 ,kerdoiv[i].valasz2);
		strcpy(tmp[i].valasz3 ,kerdoiv[i].valasz3);
		strcpy(tmp[i].valasz4 ,kerdoiv[i].valasz4);
		
	}
	
	int p2c[2], n, m;                              /* pipe változó, olvasott bájtok száma */
	int c2p[2];
	
	pid_t pid;
	
	if (pipe(p2c) < 0)                          /* pipe létrehozása */	
		printf("Pipe hiba!\n");
		
	if (pipe(c2p) < 0)                          /* pipe létrehozása */	
		printf("Pipe hiba!\n");

	if ((pid = fork()) < 0)                     /* gyerekfolyamat létrehozása */
		printf("fork hiba");
	else
		if (pid == 0)                             /* gyerek */
		{
			sleep(1);
			kill(getppid(),SIGTERM);
			sleep(3);
			
			close(p2c[1]);                          /* fölösleges vezeték bezárása */
			n = read(p2c[0], &masolat, 3 * sizeof(Kerdes));         /* információ kiolvasása */
			//printf("beolvastam %d bajtot: \n", n);
			fflush(stdout);
			
		  
			/*int x = 0;
			for (; x < 3; x++) {
				printf("%i. kerdes amit kaptam: \n", x);
				printf("%s\n", masolat[x].kerdes);
				printf("%i\n", masolat[x].kerdesId);
				printf("%s\n", masolat[x].idoDatum);
				printf("%i\n", masolat[x].benneVan);
				printf("%s\n", masolat[x].valasz1);
				printf("%s\n", masolat[x].valasz2);
				printf("%s\n", masolat[x].valasz3);
				printf("%s\n", masolat[x].valasz4);

			}*/
		  
			int kitoltok = kitoltokSzama();
			printf("kitoltok szama: %i\n", kitoltok);
		  
			int elso[4] = {0,0,0,0};
			int masodik[4]= {0,0,0,0};
			int harmadik[4]= {0,0,0,0};
			
			int valaszokSzama[3]; 
			valaszokSzama[0] = lehetsegesValaszokSzama(masolat[0]);
			valaszokSzama[1] = lehetsegesValaszokSzama(masolat[1]);
			valaszokSzama[2] = lehetsegesValaszokSzama(masolat[2]);
			
			//printf("elso valaszok szama: %i | %i | %i\n", valaszokSzama[0], valaszokSzama[1], valaszokSzama[2]);
			
			int a = 1;
			srand(time(NULL));
			for (; a <= kitoltok; a++) {
				
				
				int r1 = (rand() % valaszokSzama[0]) ;
				int r2 = (rand() % valaszokSzama[1]) ;
				int r3 = (rand() % valaszokSzama[2]) ;
				//printf("random: %i --- %i --- %i\n",r1,r2,r3);
				int t1 = elso[r1];
				elso[r1] = (t1 + 1);
				int t2 = masodik[r2];
				masodik[r2] = (t2 + 1);
				int t3 = harmadik[r3];
				harmadik[r3] = (t3 + 1);
				
			}
			
			//printf("elso: %i | %i | %i | %i\n", elso[0], elso[1], elso[2], elso[3]);
			//printf("masodik: %i | %i | %i | %i\n", masodik[0], masodik[1], masodik[2], masodik[3]);
			//printf("harmadik: %i | %i | %i | %i\n", harmadik[0], harmadik[1], harmadik[2], harmadik[3]);
			
			Valasz valaszok;
			valaszok.kerdes1[0] = elso[0];
			valaszok.kerdes1[1] = elso[1];
			valaszok.kerdes1[2] = elso[2];
			valaszok.kerdes1[3] = elso[3];
			valaszok.kerdes2[0] = masodik[0];
			valaszok.kerdes2[1] = masodik[1];
			valaszok.kerdes2[2] = masodik[2];
			valaszok.kerdes2[3] = masodik[3];
			valaszok.kerdes3[0] = harmadik[0];
			valaszok.kerdes3[1] = harmadik[1];
			valaszok.kerdes3[2] = harmadik[2];
			valaszok.kerdes3[3] = harmadik[3];
		  
			
			close(c2p[0]);
			write(c2p[1], &valaszok, sizeof(Valasz));
		  
		}
		else                                      /* szülő */
		{
			
			printf("Varakozas a biztosra!\n");
			kill(getppid(),SIGTERM); 
			pause();
			printf("work!\n");
			
			
			close(p2c[0]);                          /* fölösleges vezeték bezárása */
			write(p2c[1], &tmp, 3 * sizeof(Kerdes));      /* információ beírása a pipe-ba */
		  
			
			close(c2p[1]);
			m = read(c2p[0], &kapottValaszok, sizeof(Valasz));
			printf("\n!!!!!!!!!!!FELMERES EREDMENYE!!!!!!!!!!!\n");
			fflush(stdout);
			
			kiirEredmeny(1, tmp[0], kapottValaszok.kerdes1);
			kiirEredmeny(2, tmp[1], kapottValaszok.kerdes2);
			kiirEredmeny(3, tmp[2], kapottValaszok.kerdes3);
			printf("\n!!!!!!!!!!!VEGE A FELMERESNEK!!!!!!!!!!!\n");
		}

	
	

}

int kitoltokSzama(){
	int result = 0;
	
	srand(time(NULL));
	result = (rand() % 10) + 10;
	
	return result;
}



int lehetsegesValaszokSzama(Kerdes k) {
	int counter = 2;
	
	int igaz3 = strcmp(k.valasz3, "NULL");
	if(igaz3 != 0) {
		counter++;
	}
	int igaz4 = strcmp(k.valasz4, "NULL");
	if(igaz4 != 0) {
		counter++;
	}
	
	return counter;
}

void kiirEredmeny(int i, Kerdes k, int valaszok[4]) {
	int valaszokSz = lehetsegesValaszokSzama(k);
	
	
	printf("\n%i. kerdes: %s\n", i, k.kerdes);
	printf("	a) %s | szavazat: %i db\n", k.valasz1, valaszok[0]);
	printf("	b) %s | szavazat: %i db\n", k.valasz2, valaszok[1]);
	if (valaszokSz == 3) {
		printf("	c) %s | szavazat: %i db\n", k.valasz3, valaszok[2]);
	}
	if (valaszokSz == 4) {
		printf("	c) %s | szavazat: %i db\n", k.valasz3, valaszok[2]);
		printf("	d) %s | szavazat: %i db\n", k.valasz4, valaszok[3]);
	}
	printf("---------------------\n");
}


