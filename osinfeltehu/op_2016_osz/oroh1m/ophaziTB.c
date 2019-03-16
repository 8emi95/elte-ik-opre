#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct vendeg{
    char nev[20];
    char email[20];
    char azonosito[10];
    char t[64];
}vendeg;

typedef struct lista{
    struct vendeg vendegek;
    int sorrend;
    struct lista *kov;
}lista;

lista *beteszhatra(lista *eleje,vendeg vendegek,int i){
    lista *nou;
    lista *mozgo;
    nou=(lista*)malloc(sizeof(lista));
    mozgo=eleje;
    nou->vendegek=vendegek;
    nou->sorrend=i;
    if (eleje == NULL)
        return nou;
   else {
        while (mozgo->kov != NULL){
        mozgo=mozgo->kov;
    }
     mozgo->kov=nou;
    nou->kov=NULL;
    return eleje;
   }

}

void bejar( lista *eleje){
    if (eleje == NULL)
        printf("Lista este goala");
    else
    {
        do {
            printf("%d %s %s %s %s",eleje->sorrend,eleje->vendegek.nev,eleje->vendegek.email,eleje->vendegek.azonosito,eleje->vendegek.t);
            eleje=eleje->kov;
        }while(eleje != NULL);
    }
}

void beolvas(char * nev,char * email,char* azon)
{
    printf("Udv!");
    printf("Nev: ");
    scanf("%s",nev);
    printf("E-mail: ");
    scanf("%s",email);
    printf("Azonosito: ");
    scanf("%s", azon);
}

lista *torles(lista *eleje,int torlendo){
    lista *mozgo;
    mozgo=eleje;
    while (mozgo->kov->sorrend != torlendo ){
        mozgo=mozgo->kov;
    }
    mozgo->kov=mozgo->kov->kov;
    return eleje;
}

void modosit(lista *eleje,int modositando)
{   int c;
    char *valtozo;
    valtozo = (char*)malloc(sizeof(char));
    lista* mozgo;
    mozgo =eleje;
    while(mozgo->sorrend != modositando)
        {
            mozgo=mozgo->kov;
        }
    printf("1-es nevmodositas\n");
    printf("2-es email modositas\n");
    printf("3-as azonosito modositas\n");
    printf("Barmely mas kilep\n");
    printf("valasz opciot:");
    scanf("%d",&c);
    switch(c){
        case 1:
            printf("uj nev:");
            scanf("%s",valtozo);
            strcpy(mozgo->vendegek.nev,valtozo);
            break;
        case 2:
            printf("uj email:");
            scanf("%s",valtozo);
            strcpy(mozgo->vendegek.email,valtozo);
            break;
        case 3:
            printf("uj azonosito: ");
            scanf("%s",valtozo);
            strcpy(mozgo->vendegek.azonosito,valtozo);
            break;
        default:
            exit(0);
            break;
    }

}

void fajlbairas(lista* eleje){
       char* filename;
    filename = (char*)malloc(sizeof(char));
    FILE *fp;
    lista *mozgo;
    mozgo = eleje;
     while(mozgo!=NULL){
     strcpy(filename,mozgo->vendegek.azonosito);
    strcat(filename,".txt");
    fp = fopen(filename,"a+");
    fprintf(fp,"%d %s %s %s %s \n",mozgo->sorrend,mozgo->vendegek.nev,mozgo->vendegek.email,mozgo->vendegek.azonosito,mozgo->vendegek.t);
    fclose(fp);
    mozgo=mozgo->kov;
     }
}

void rendezvenytorles(char* torlendo){
    strcat(torlendo,".txt");
    remove(torlendo);

}

int main(){
    time_t mytime;
    mytime=time(NULL);
    struct tm *t;
    vendeg vendegek[10];
    lista *eleje;
    eleje=(lista*)malloc(sizeof(lista));
    eleje=NULL;
    //FILE *fp;
    int i=0;
    int opcio;
    int opcio1;
    int erkezesisz=0;
    char* rendazon;
    char ido[64];
    char *nev;
    char *email;
    char *azon;
    azon = (char*)malloc(sizeof(char));
    nev = (char*)malloc(sizeof(char));
    email = (char*)malloc(sizeof(char));
    rendazon = (char*)malloc(sizeof(char));
    // t =localtime(&mytime);
    //strftime(ido,sizeof(ido),"%c",t);
    do{
        printf("1.Jelentkezes\n");
        printf("2.Jelentkezettek listazasa\n");
        printf("3.Vendeg adatainak modositasa\n");
        printf("4.Vendeg torlese\n");
        printf("5.Vendeg lista mentese file-ba\n");
        printf("6.Rendezveny teljes torlese\n");
        printf("Kerem adja meg az opciot: ");
        scanf("%d",&opcio);
        switch(opcio){
            case 1:
                beolvas(nev,email,azon);
                t =localtime(&mytime);
                strftime(ido,sizeof(ido),"%c",t);
                strcpy(vendegek[i].nev,nev);
                strcpy(vendegek[i].email,email);
                strcpy(vendegek[i].azonosito,azon);
                strcpy(vendegek[i].t,ido);
                eleje=beteszhatra(eleje,vendegek[i],i+1);
                i++;
                break;
            case 2:
                bejar(eleje);
                break;
            case 3:
                printf("A modositando vendeg erkezesi szama: ");
                scanf("%d",&erkezesisz);
                modosit(eleje,erkezesisz);
                break;
            case 4:
                printf("A torloni kivant vendeg erkezesi szama: ");
                scanf("%d",&erkezesisz);
                eleje=torles(eleje,erkezesisz);
                break;
            case 5:
                printf("A fajlban levo adatokat nem lehet modositani!\n");
                printf("Biztosan akarja menteni az adatokat?");
                printf("1.Igen\n 2.Nem");
                printf("Valaszon");
                scanf("%d",&opcio1);
                switch(opcio1){
                    case 1:
                        fajlbairas(eleje);
                        break;
                    case 2:
                        exit(0);
                        break;

                }
                break;
            case 6:
                printf("Adja meg a torlendo rendezveny azonositojat: ");
                scanf("%s",rendazon);
                rendezvenytorles(rendazon);
                break;
            default:
                exit(0);
                break;
    }
    }while(opcio!=0);
    return 0;
}