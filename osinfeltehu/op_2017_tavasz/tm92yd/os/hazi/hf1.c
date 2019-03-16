#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
1. Írjuk ki egy program bemeneti paraméterei hosszainak az ősszegét!

2. Írjuk ki, ha egy programnak legalább két paramétere van, és ezek mind megegyeznek!

3. Ellenőrizzuk, hogy egy programnak pontosan két bemenő paramétere van-e, majd írjuk ki ezek konkatenáltját!

4. Javítsuk ki az összes ismétlés hibát open_fopen.c mintaprogramban!

5. Hozzunk létre egy fájlt, és írjuk bele a nevünket (ugyanabból a programból)!

6. Definiáljunk egy tetszőleges structot ket int adattaggal, adjunk az adattagoknak értéket, írjuk ki fájlba (bináris módban), olvassuk vissza (szinten bináris módban), majd ellenőrizzuk, hogy azt olvastuk ki, amit beírtunk!

6.b A számokat program paraméterben kérjuk be! (Szükség lehet char* -> int konverzióra, használhatunk atoi-t.)

7. Készítsünk programot, mely egy char és int adattagokból álló struktúra alkotta n (konstans, páros) elemű tömböt rendre feltölt 'a'-tól kezdve egymás utáni kódú karakterekkel, valamint 1-től n-ig számokkal, majd kiírja a tartalmát egy fájlba bináris módban. Ezután visszaolvassa az értékeket a tömbbe kettesével úgy, hogy az első kettőt az (n-2), (n-1); a második kettőt az (n-4), (n-3); és így tovább, míg az utolsó kettőt a 0, 1 indexű elemekbe olvassa. Végül írjuk ki a tömb tartalmát a képernyőre.

8. Olvassunk ki soronként egy fájlt, és írjuk ki a képernyőre rendezve a tartalmát!
*/

//1
int length(char *pointer) {
    int len = 0;
    while(*pointer != 0) {
        len++;
        pointer++;
    }
    return len;
}

//2
int equal(char *str1, char *str2) {
    while(*str1 || *str2) {
        if(*str1 != *str2) {
            return 0;
            str1++;
            str2++;
        }
    }
    return 1;
}

//6
struct IntStruct {
    int a;
    int b;
};

int main(int argc, char ** argv) {
    //1
    int i;
    int s = 0;
    for(i=1; i<argc; ++i) {
        s += length(argv[i]);
    }

    printf("Bemeneti paraméterek összege: %i \n", s);

    //2
    int eq = 0;
    int j;
    int equals[argc-1];
    for(i=0; i<sizeof(equals)/sizeof(int); ++i) {
        equals[i] = 1;
    }

    char *string;
    if(argc >= 2) {
        for(i=1; i<argc-1; ++i) {
            for(j=1; j<argc-1; ++j) {
                if(i != j) {
                    eq = equal(argv[i], argv[i+1]);
                }
            }
            if(eq > 0) {
                equals[i-1] = eq;
                string = argv[i];
            }
        }
    }

    for(i=0; i<sizeof(equals)/sizeof(int); ++i) {
        eq *= equals[i];
    }

    if(eq != 0) {
        printf("Bemeneti paraméterek azonosak: %s \n", string);
    }
    
    //3
    if(argc-1 == 2) {
        char src[50], dest[50];

        strcpy(src,  argv[1]);
        strcpy(dest, argv[2]);

        strcat(dest, src);
        printf("2 bemeneti parameter konkatenálva: %s \n", dest);
    }

    //5
    FILE * f;
    f=fopen("name.txt","a+");
    fputs("Varga Mate Istvan", f);

    //6
    FILE * fb;
    fb=fopen("struct.txt","ab+");
    
    IntStruct st;
    st.a = 0;
    st.b = 1;
    printf("Struct %d %d \n", st);
    fwrite(&st, sizeof(struct IntStruct), 1, fb);

    fread(0, sizeof(struct IntStruct), 1, fb);

    return 0;
}