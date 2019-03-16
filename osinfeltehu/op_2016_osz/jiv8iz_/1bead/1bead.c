#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 512
#define fajlnev "output.txt"

void menu();
void jelentkezes();
void listazas();
void modositas();
void torles();
void ujrend();
void rendtorles();


int main(int argc, char* argv[]) {
    menu();
    return 0;
}

void menu(){
	char menupont[MAX];
    sscanf("-1","%s",menupont);
	while(strcmp(menupont,"7")!=0){
		printf("\n1 - Uj jelentkezo\n");
		printf("2 - Jelentkezo adatainak modositas\n");
		printf("3 - Jelentkezo adatainak torles\n");
		printf("4 - Jelentkezok listazasa\n");
		printf("5 - Uj rendezveny bejegyzese\n");
		printf("6 - Rendezveny adatainak torlese\n");
		printf("7 - Kilepes \n");
		printf("\nValasszon a menupontok kozul: ");
		scanf("%s",menupont);

		if(strcmp(menupont,"1") == 0){
			jelentkezes();
		}
		else if(strcmp(menupont,"2") == 0){
			modositas();
		}
		else if(strcmp(menupont,"3") == 0){
			torles();
		}
		else if(strcmp(menupont,"4") == 0){
			listazas();
		}
		else if(strcmp(menupont,"5") == 0){
			ujrend();
		}
		else if(strcmp(menupont,"6") == 0){
			rendtorles();
		}
	}
}

void jelentkezes(){
	printf("Uj vendeg regisztracioja:\n\n");

	char nev[MAX],mail[MAX];
	char rendazon[MAX];

    printf("Adja meg a jelentkezo nevet: ");
    scanf("%s",nev);
    printf("Adja meg a jelentkezo email cimet: ");
    scanf("%s",mail);
    printf("Adja meg a rendezveny azonositojat: ");
    scanf("%s",rendazon);
	
    FILE* fajl = fopen(fajlnev,"a+"); //olvasásra és módosítáásra nyitja meg
    fprintf(fajl,"%s\t%s\t%s\t%s\t%s\n",nev,mail,rendazon,__DATE__,__TIME__);
    fclose(fajl);

	fajl = fopen(fajlnev,"r");
	int sorszam = 0;
	char sor[MAX];
	while(fgets(sor,sizeof(sor),fajl)){
		sorszam++;
	}
	fclose(fajl);
    printf("%i", sorszam);
    printf(". helyen jelentkezett!\n");

}

void torles(){
    printf("Jelentkezo adatainak torlese!\n\n");

    char neve[64];

    printf("Adja meg a torlendo jelentkezo nevet: ");
    scanf("%s",neve);

    FILE* fajl = fopen(fajlnev,"r");
    FILE* uj = fopen("uj.txt","w");
    char sor[MAX];

    char nev[64],mail[64],rendazon[16];
    while(fgets(sor,sizeof(sor),fajl)){
        sscanf(sor,"%s %s %s",nev,mail,rendazon);
        if (strcmp(neve,nev)==0) {
            printf("A jeletkezo adatai torolve.");
        }else {
            fprintf(uj,"%s",sor);
        }
    }
    fclose(fajl);
    fclose(uj);

    fajl = fopen(fajlnev,"w");
    uj = fopen("uj.txt","r");
    while(fgets(sor,sizeof(sor),uj)){
        fprintf(fajl,"%s",sor);
    }
    fclose(uj);
    fclose(fajl);
    remove("uj.txt");
}

void modositas(){
    printf("Jelentkezo adatanak modositasa!\n\n");
    char modnev[64];

    printf("Adja meg a modosiani kivant jelentkezo nevet: ");
    scanf("%s",modnev);

    char ujnev[64], ujmail[64], ujazon[16];

    printf("Adja meg a jelentkezo uj adatait: \n");
    printf("Neve: ");
    scanf("%s",ujnev);
    printf("Email cime: ");
    scanf("%s",ujmail);
    printf("Rendezveny azonositoja: ");
    scanf("%s",ujazon);

    FILE* fajl  = fopen(fajlnev,"r");
    FILE* uj = fopen("uj.txt","w");
    char sor[MAX];
    char nev[64], mail[64], azon[16], honap[16], nap[16], ido[64];
	
	while(fgets(sor,sizeof(sor),fajl)){
        sscanf(sor,"%s %s %s %s %s %s",nev,mail,azon,honap,nap,ido);
        if(strcmp(nev,modnev)==0) {
            fprintf(uj,"%s\t%s\t%s\t%s\t%s\n",ujnev,ujmail,ujazon,honap,nap,ido);
        }else{
            fprintf(uj,"%s",sor);
        }
    }

    fclose(fajl);
    fclose(uj);

    fajl = fopen(fajlnev,"w");
    uj = fopen("uj.txt","r");

    while(fgets(sor,sizeof(sor),uj)){
        fprintf(fajl,"%s",sor);
    }
    fclose(uj);
    fclose(fajl);
    remove("uj.txt");
}

void listazas(){
    char menupont[MAX];
    printf("A jelentkezok listazasa!\n\n");
    printf("1. - Minden jelentkezo listazasa.\n");
    printf("2. - Adott rendezvenyre jelentkezok listazasa.\n");
    printf("Menupont: ");
    scanf("%s",menupont);

    FILE* fajl = fopen(fajlnev,"r");
    char sor[MAX];

    if(strcmp(menupont,"1") == 0) {
        while(fgets(sor,sizeof(sor),fajl)){
            printf("%s",sor);
        }
    }

    if(strcmp(menupont,"2")==0){
        char rend[MAX],nev[64],mail[64],rendazon[16];
        printf("Rendezveny azonositoja: ");
        scanf("%s",rend);
        while(fgets(sor,sizeof(sor),fajl)){
            sscanf(sor,"%s%s%s",nev,mail,rendazon);
            if(strcmp(rendazon,rend) == 0){
                printf("%s",sor);
            }
        }
    }

    fclose(fajl);

}

void ujrend(){
    char ujrendnev[MAX], ujrendazon[MAX];
    printf("Adja meg az uj rendezveny nevet: ");
    scanf("%s",ujrendnev);
    printf("Adja meg az uj rendezveny azonositojat: ");
    scanf("%s",ujrendazon);

    FILE* rendezvenyek =fopen("rendezvenyek.txt","a+");
    fprintf(rendezvenyek,"%s\t%s\n",ujrendnev, ujrendazon);

    fclose(rendezvenyek);
}

void rendtorles(){
    char azon[MAX];

    printf("Adja meg melyik rendezvenyhez tartozo adatokat szeretne torolni!\n");
    printf("Azonositoja: ");
    scanf("%s",azon);

    FILE* fajl = fopen(fajlnev,"r");
    FILE* uj = fopen("uj.txt","w");

    char sor[MAX];

    char nev[64],mail[64],rendazon[16];
    while(fgets(sor,sizeof(sor),fajl)){
        sscanf(sor,"%s %s %s",nev,mail,rendazon);
        if (strcmp(rendazon,azon)==0) {
            printf("A rendezveny adatai torolve a jelentkezesek listajabol.");
        }else {
            fprintf(uj,"%s",sor);
        }
    }

    fclose(fajl);
    fclose(uj);

    fajl = fopen(fajlnev,"w");
    uj = fopen("uj.txt","r");

    while(fgets(sor,sizeof(sor),uj)){
        fprintf(fajl,"%s",sor);
    }

    fclose(uj);
    fclose(fajl);
    remove("uj.txt");


    FILE* rendezvenyek = fopen("rendezvenyek.txt","r");
    FILE* uj2 = fopen("uj2.txt","w");

    char rendnev[64],rendazonmasik[64];
    while(fgets(sor,sizeof(sor),rendezvenyek)){
        sscanf(sor,"%s %s",rendnev,rendazonmasik);
        if (strcmp(rendazonmasik,azon)==0) {
            printf("A rendezveny adatai torolve a rendezvenyek listajabol.");
        }else {
            fprintf(uj2,"%s",sor);
        }
    }

    fclose(rendezvenyek);
    fclose(uj2);

    rendezvenyek = fopen("rendezvenyek.txt","w");
    uj2 = fopen("uj2.txt","r");


    while(fgets(sor,sizeof(sor),uj2)){
        fprintf(rendezvenyek,"%s",sor);
    }

    fclose(uj2);
    fclose(rendezvenyek);
    remove("uj2.txt");
}
