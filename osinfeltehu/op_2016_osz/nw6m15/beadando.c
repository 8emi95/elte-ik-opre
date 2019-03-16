/*
1. beadand� feladat. Bead�si hat�rido: 2016-11-07.
K�sz�tsen C programot, amely rendszerh�v�sok alkalmaz�s�val megoldja a k�vetkezo feladatot:

Egy ki�ll�t�sokat, rendezv�nyeket szervezo t�rsas�g a "King of Stands", elektronikus l�togat�i nyilv�ntart�st k�sz�tett.
A l�togat�k a nyilv�ntart�sba jelentkez�skor megadj�k nev�ket, mail c�m�ket, a rendezv�ny azonos�t� sz�m�t!

Az elektronikus rendszer a jelentkez�s nyugt�z�sak�nt visszaadja, hogy h�nyadik vend�gk�nt t�rt�nt meg a regisztr�ci�!

Az alkalmaz�s adjon lehetos�get vend�g jelentkez�shez, a jelentkezettek list�z�s�hoz, egy vend�g adatainak m�dos�t�s�hoz, t�rl�s�hez.

Legyen lehetos�g�nk egy �j rendezv�ny ind�t�s�hoz, egy rendezv�ny �sszes adat�nak t�rl�s�hez!

A jelentkezokn�l a kor�bbi adatok mellett r�gz�ts�k automatikusan a jelentkez�si idot(rendszerido) is(ezt nem kell bek�rni)!

K�sz�ts�nk C programot, amellyel a le�rt tev�kenys�g elv�gezheto. Az adatokat f�jlban t�roljuk.


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
        printf("M�dos�t�shoz k�rem adja meg a nevet amit m�dos�tani akar: ");
        scanf("%s", name);

        if(hanyadik!=0){
            vendeg * next = first;
            int bool = 1;
            while (bool ==1 && megvan==0){
                bool=(*next).hasnext;
                //printf("%s ", (*next).nev);

                if((strcmp((*next).nev, name) == 0)){
                        char name[30];
                        printf("Kerem az �j/modositott nevet: (max 30 karakter): ");
                        scanf("%s", name);

                        char mail[50];
                        printf("Kerem az �j/modositott mail cimet (max 50 karakter): ");
                        scanf("%s", mail);

                        char azon[50];
                        printf("Mi az �j/modositott eloadas azonositoja? ");
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
        printf("T�rleshez k�rem adja meg a nevet amit t�r�lni akar: ");
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
                    printf("T�r�lve!");

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
        printf("1, vend�g jelentkez�s\n");
        printf("2, Jelentkez�sek list�z�sa\n");
        printf("3, vend�g adatm�dos�t�s\n");
        printf("4, jelentkez�s t�rl�s\n");
        printf("5, Kil�p�s a programb�l\n");
        num_ok = scanf( "%d", &menuvalasztasout );

        if (num_ok ==1){
            int menuvalasztas =menuvalasztasout;
            if(menuvalasztas>5){
                printf("1-5-ig v�lassz\n");

                menu();
            }else if(menuvalasztas<1){
                printf("1-5-ig v�lassz\n");

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
            printf("Sz�mokat kell megadni!");
            return 6;
        }

        return 0;
    }
