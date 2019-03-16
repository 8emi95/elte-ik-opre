#include <stdio.h>
#include <stdlib.h>


//adatszerkezet
typedef struct adatszerkezeket{
    char nev[22];
    char mail[40];
    int azon;
    time_t ido;
    int id;
} data;

const int N = 500;

typedef struct Adat{
    int rendez[50];
    int db;
    int dbrend;
    data adatok[200];
} Adatok;

//adatszerkezet vége
///////////////////////////////////////////////
Adatok adat;

void jelentkezes(){
    if(adat.dbrend != 0 && adat.db < 200){
        printf("Kerem a nevet: ");
        scanf("%s",&adat.adatok[adat.db].nev);
        printf("Kerem a mail-t: ");
        scanf("%s",&adat.adatok[adat.db].mail);
        int i;
        int azon;
        do{
            i = 0;
            printf("Kerem a rendezveny azonositojat: ");
            scanf("%d",&azon);
            while(azon != adat.rendez[i] && i < adat.dbrend){
                printf("%d\n",adat.rendez[i]);
                i = i + 1;
            }
            if(i == adat.dbrend){
                printf("Hibas rendezveny azonosito!\n");
            }
        }while(i == adat.dbrend);
        adat.adatok[adat.db].azon = azon;
        time (&adat.adatok[adat.db].ido);
        adat.adatok[adat.db].id = adat.db;
        printf("Vendeg sikeresen felveve! A sorszama: %d\n",adat.db);
        adat.db = adat.db +1;
    }else{
        printf("Nincsen elerheto rendezveny! Vagy megtelt a hely.\n");
    }
}

void listazas(){
    int i = 0;
    //struct tm *pontosido;
    while(i < adat.db){
        //pontosido = localtime( &adat.adatok[i].ido);
        printf("      %d. %s, %s, %d, %d\n",i,adat.adatok[i].nev,adat.adatok[i].mail,adat.adatok[i].azon,ctime(&adat.adatok[i].ido));
        i = i + 1 ;
    }
}

void modositas(){
    if(adat.db != 0){
        printf("Kerem a vendeg sorszamat: ");
        int azon;
        scanf("%d",&azon);
        int i = 0;
        while(adat.adatok[i].id != azon && i < adat.db){
            i = i +1;
        }
        if(i == adat.db){
            printf("Nem letezo sorszam!\n");
        }else{
            printf("Adja meg az adatokat:\n");
            printf("Kerem a nevet: ");
            scanf("%s",&adat.adatok[i].nev);
            printf("Kerem a mail-t: ");
            scanf("%s",&adat.adatok[i].mail);

            int i;
            int azon;
            do{
                i = 0;
                printf("Kerem a rendezveny azonositojat: ");
                scanf("%d",&azon);
                while(azon != adat.rendez[i] && i < adat.dbrend){
                    i = i + 1;
                }
                if(i == adat.dbrend){
                    printf("Hibas rendezveny azonosito!\n");
                }
            }while(i == adat.dbrend);
            adat.adatok[adat.db].azon = azon;
            printf("Vendeg adati sikeresen modsitva\n");
        }
    }else{
        printf("Nincsen elerheto adat!\n");
    }
}

void torles(){
    if(adat.db != 0){
        printf("Kerem a vendeg sorszamat: ");
        int azon;
        scanf("%d",&azon);
        int i = azon;
        if(azon >= adat.db){
            printf("Nem letezo sorszam!\n");
        }else{
            while(i < adat.db-1){
                adat.adatok[i] = adat.adatok[i+1];
                i = i + 1;
            }
            adat.db = adat.db - 1;
            printf("Sikeres torles\n");
        }
    }else{
        printf("Nincsen elerheto adat!\n");
    }
}

void ujrendezveny(){
    if(adat.dbrend < 50){
        printf("Kerem a rendezveny azonositojat: ");
        int azon;
        scanf("%d",&azon);
        int i = 0;
        while(adat.rendez[i] != azon && i < adat.dbrend){
            i = i + 1;
        }
        if(i != adat.dbrend){
            printf("Mar letezo azonosito!\n");
        }else{
            adat.rendez[adat.dbrend] = azon;
            adat.dbrend = adat.dbrend + 1;
            printf("Rendezveny sikeresen felveve\n");
        }
    }else{
        printf("Nincs szabad hely!\n");
    }
}

void rendezvenytorles(){
    if(adat.dbrend != 0){
        int azon;
        int i;
        do{
            i = 0;
            printf("Kerem a rendezveny azonositojat: ");
            scanf("%d",&azon);
            while(azon != adat.rendez[i] && i < adat.dbrend){
                i = i + 1;
            }
            if(i == adat.dbrend){
                printf("Hibas rendezveny azonosito!\n");
            }
        }while(i == adat.dbrend);
        while(torles2(azon) == 0){;}
        while(i < adat.dbrend -1){
            adat.rendez[i] = adat.rendez[i+1];
            i = i + 1;
        }
        adat.dbrend = adat.dbrend -1;
    }else{
        printf("Nincsen elerheto adat!\n");
    }
}

int torles2(int azon){
    int i = 0;
    while(adat.adatok[i].azon != azon && i < adat.db){
        i = i +1;
    }
    if(i == adat.db){
        return(1);
    }else{
        while(i < adat.db -1){
            adat.adatok[i] = adat.adatok[i+1];
            i = i + 1;
        }
        adat.db = adat.db - 1;
        return(0);
    }
}

void kilep(){
    FILE *file;
    file = fopen("be.txt","w");
    fprintf(file,"%d\n",adat.dbrend);
    int i = 0;
    while(i < adat.dbrend){
        fprintf(file,"%d ",adat.rendez[i]);
        i = i + 1;
    }
    fprintf(file,"\n%d",adat.db);
    i = 0;
    while(i < adat.db){
        fprintf(file,"\n%s\n%s\n%d\n%d\n%d",adat.adatok[i].nev,adat.adatok[i].mail,adat.adatok[i].azon,adat.adatok[i].id,adat.adatok[i].ido);
        i = i + 1;
    }
    fclose(file);
}

int main()
{
    //init rész

    adat.db = 0;
    adat.dbrend = 0;
    //adatok beöltése
    FILE *file = 0;
    file = fopen("be.txt","r");
    if(file != 0){
        fscanf(file,"%d",&adat.dbrend);
        int i = 0;
        while(i < adat.dbrend){
            fscanf(file,"%d",&adat.rendez[i]);
            i = i +1;
        }
        fscanf(file,"%d",&adat.db);
        i = 0;
        while(i < adat.db){
            fscanf(file,"%s",&adat.adatok[i].nev);
            fscanf(file,"%s",&adat.adatok[i].mail);
            fscanf(file,"%d",&adat.adatok[i].azon);
            fscanf(file,"%d",&adat.adatok[i].id);
            fscanf(file,"%d",&adat.adatok[i].ido);
            i = i + 1;
        }
    }
    fclose(file);

    //menu
    int id = 0;
    while(id != 7){
        printf("1. Vendeg jelentkezes\n");
        printf("2. Jelentkezettek listazasa\n");
        printf("3. Vendeg adatainak modositasa\n");
        printf("4. Vendeg torlese\n");
        printf("5. Uj rendezveny inditasa\n");
        printf("6. Rendezveny adatainak torlese\n");
        printf("7. Kilepes\n");
        scanf("%d",&id);
        switch(id){
        case 1: jelentkezes();
            break;
        case 2:listazas();
            break;
        case 3:modositas();
            break;
        case 4:torles();
            break;
        case 5:ujrendezveny();
            break;
        case 6:rendezvenytorles();
            break;
        case 7: kilep();
            break;
        default: printf("hibas id!\n");
        }
    }


    return 0;
}
