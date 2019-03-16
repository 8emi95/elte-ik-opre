/*
1. beadandó feladat. Beadási határido: 2016-11-07.
Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következo feladatot:

Egy kiállításokat, rendezvényeket szervezo társaság a "King of Stands", elektronikus látogatói nyilvántartást készített.
A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát!

Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció!

Az alkalmazás adjon lehetoséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez.

Legyen lehetoségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez!

A jelentkezoknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési idot(rendszerido) is(ezt nem kell bekérni)!

Készítsünk C programot, amellyel a leírt tevékenység elvégezheto. Az adatokat fájlban tároljuk.


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h> //ctime

	int i=0;
	int hanyadik=0;
	struct stat st;


    typedef struct {
            char nev[30];
            char mail[50];
            char azonosito[50];
            time_t t;
            int hasnext;
            struct vendeg * next;
    }vendeg;

    vendeg* first;
    vendeg* last;


    void jelentkezes ();

    void lista ();

    void modosit();

    void torles();

    int menu();


int main(int argc,char** argv){
	printf("King of Stands\n");


    stat(argv[0],&st);
    time_t t=st.st_mtime;
    printf("SYSTIME: %s\n",ctime(&t)); //convert time_t to string

	int a = menu();



	return a;
}

    void jelentkezes (){

        char name[30];
        printf("Kerem a nevet: (max 30 karakter): ");
        scanf("%s", name);

        char mail[50];
        printf("Kerem a mail cimet (max 50 karakter): ");
        scanf("%s", mail);

        char azon[50];
        printf("Mi az eloadas azonositoja? ");
        scanf("%s", azon);

        vendeg vendeg1;
        strcpy( vendeg1.azonosito ,azon);
        strcpy( vendeg1.mail, mail);
        strcpy( vendeg1.nev, name);

        vendeg1.t=st.st_mtime;
        vendeg1.hasnext=0;

        if (hanyadik == 0){
            first = &vendeg1;
            last = &vendeg1;
            hanyadik++;
        }else {
            (*first).hasnext=1;

            (*last).next = &vendeg1;
            (*last).hasnext =1;
           // printf("%s ", (*last).nev);
            last = &vendeg1;
            hanyadik++;
        }


        menu();
    }
    void lista (){

        if(hanyadik!=0){
            vendeg * next = first;
           // vendeg vendegfirst = *first;

            int bool = 1;
            while (bool ==1 ){
                bool=(*next).hasnext;

                //printf("%d\n",bool);

                printf("%s ", (*next).nev);
                printf("%s ", (*next).mail);
                printf("%s ", (*next).azonosito);
                time_t tt=(*next).t;
                printf("regtine: %s\n",ctime(&tt));
                printf("\n");

                if (bool ==1){
                    next = (*next).next;
                }
            }
        }
        menu();
    }

    void modosit(){

        int megvan = 0;
        char name[30];
        printf("Módosításhoz kérem adja meg a nevet amit módosítani akar: ");
        scanf("%s", name);

        if(hanyadik!=0){
            vendeg * next = first;
            int bool = 1;
            while (bool ==1 && megvan==0){
                bool=(*next).hasnext;
                //printf("%s ", (*next).nev);

                if((strcmp((*next).nev, name) == 0)){
                        char name[30];
                        printf("Kerem az új/modositott nevet: (max 30 karakter): ");
                        scanf("%s", name);

                        char mail[50];
                        printf("Kerem az új/modositott mail cimet (max 50 karakter): ");
                        scanf("%s", mail);

                        char azon[50];
                        printf("Mi az új/modositott eloadas azonositoja? ");
                        scanf("%s", azon);
                        strcpy( (*next).azonosito ,azon);
                        strcpy( (*next).mail, mail);
                        strcpy( (*next).nev, name);
                        megvan=1;

                }

                if (bool ==1 && megvan==0){
                    next = (*next).next;
                }
            }
        }

        menu();
    }
    void torles(){

        int megvan = 0;
        char name[30];
        printf("Törleshez kérem adja meg a nevet amit törölni akar: ");
        scanf("%s", name);

        if(hanyadik!=0){
            vendeg * next = first;
            vendeg * post = first;
            int bool = 1;
            while (bool ==1 && megvan==0){
                bool=(*next).hasnext;

                //printf("%s ", (*next).nev);

                if((strcmp((*next).nev, name) == 0)){
                    (*post).next=(*next).next;
                    printf("Törölve!");

                    megvan=1;
                }

                if (bool ==1 && megvan==0){
                    post = next;
                    next = (*next).next;
                }
            }
        }


        menu();
    }
    int menu(){
        int menuvalasztasout;
        int num_ok;

        printf("MENU:\n");
        printf("1, vendég jelentkezés\n");
        printf("2, Jelentkezések listázása\n");
        printf("3, vendég adatmódosítás\n");
        printf("4, jelentkezés törlés\n");
        printf("5, Kilépés a programból\n");
        num_ok = scanf( "%d", &menuvalasztasout );

        if (num_ok ==1){
            int menuvalasztas =menuvalasztasout;
            if(menuvalasztas>5){
                printf("1-5-ig válassz\n");

                menu();
            }else if(menuvalasztas<1){
                printf("1-5-ig válassz\n");

                menu();
            }else if (menuvalasztas ==1){
                jelentkezes();
            }else if (menuvalasztas ==2){
                lista ();
            }else if (menuvalasztas ==3){
                modosit();
            }else if (menuvalasztas ==4){
                torles();
            }else if (menuvalasztas ==5){
                printf("Viszlat!");
                return 5;
            }

        }else {
            printf("Számokat kell megadni!");
            return 6;
        }

        return 0;
    }
