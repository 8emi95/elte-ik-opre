      
Névvel ellátott csővezeték (FIFO)
Azaz olyan állomány, amelyhez bármelyik folyamat hozzáférhet amelynek van rá megfelelő joga...
 

  Definíció

  Műveletek FIFO állományokkal

  Létrehozás (mknod, mkfifo)

  Megnyitás (open)

  Olvasás és írás (read, write)

  Bezárás (close)

  Törlés (unlink)

  Folyamatok közti kommunikáció FIFO állományok segítségével

  Példák

  1.példa (apa-fiú kapcsolat)

  2. példa (kliens-szerver rendszer)

 

 

  Csővezeték (pipe)

  Definíció

A névvel ellátott csővezeték (FIFO állomány) a közönséges fájl és a pipe kombinációja. A FIFO állománynak van egy szimbolikus neve, és egy könyvtára, ahová létrehozzuk. Mindemellett megőrzi a pipe fájlok összes jellemzőit.

A FIFO állományok előnye a pipe-okkal szemben az, hogy a rajtuk keresztül kommunikáló folyamatok nem kell továbbítsák a fájlleírót. Éppen ezért a FIFO állományokat nem csak az a folyamat (vagy annak leszármazottja) használhatja, amely azt létrehozta, hanem más, külső folyamatok is. Tehát bármely folyamat megnyithat írásra, illetve olvasásra egy FIFO fájlot, csak a szimbolikus nevet kell ismernie.

Egy FIFO állomány megtekinthető az ls -l Unix parancs segítségével. A hozzáférési jogok első mezője ez esetben egy p betű.

  Műveletek FIFO állományokkal

  Létrehozás (mknod, mkfifo)

A FIFO állományok létrehozására két parancs is létezik: az mknod és az mkfifo. A második függvény tulajdonképpen az elsőt hívja, s a különbség csak a paraméterek számában van. Alakjuk:

#include <sys/types.h>
#include <sys/stat.h>

int mknod(char *pathname, int mode, 0);
int mkfifo(const char *pathname, mode_t mode);

Sikeres létrehozás esetén mindkét függvény 0-t térít vissza, különben -1 értéket. A pathname argumentum a FIFO állomány elérési útvonala. A mode egész típusú változó az S_FIFO állomány típusát és a hozzáférési jogokat (r, w, x) jelöli a tulajdonos, a csoport és a többi felhasználó számára. Ezt a paramétert a következő alakban kell megadni:

S_IFIFO  |  jogok

Például ahhoz, hogy egy 'read' és 'write' jogokkal rendelkező FIFO állományt létrehozzunk a tulajdonosnak, a csoportjának és a többiek számára, használhatjuk a következő függvényt:

int makepipe(char *path)
{
  return (mknod(path, S_IFIFO | 0666, 0));
}

  Megnyitás (open)

Egy FIFO állomány megnyitása írásra vagy olvasásra a hagyományos fájlokéhoz hasonlóan történik az open függvény használatával, azaz:

#include <sys/types.h>
#include <sys/stat.h>

int open(const char *pathname, int flags);

A függvény visszatérített értéke az új fájlleíró, vagy -1 hiba esetén. A pathname argumentum a FIFO állomány elérési útvonala, a flags változóban pedig a hozzáférési jogokat kell megadni, amely éppen a következő konstansok egyike:

- O_RDONLY, csak olvasható,
- O_WRONLY, csak írható,
- O_RDWR, olvasható és írható.

Például:

... 
int fd;
fd = open("FIFO_1", O_WRONLY);
...

  Olvasás és írás (read, write)

A FIFO állományok olvasása pontosan úgy történik, mint a hagyományos fájloké. A read és a write függvények szintaxisa megtalálható itt.

  Bezárás (close)

A FIFO állományok bezárása is hasonló a hagyományos fájlokéhoz, illetve a pipe-oknál látottakhoz. A close parancs szintaxisának megtekintéséhez kattints ide.

Megjegyzés: Ha egy FIFO állományt bezárunk, akkor az őt olvasó folyamat az illető állományt befejezettnek látja.

  Törlés (unlink)

A pipe fájlokkal ellentétben a FIFO állományok törlése az unlink függvény segítségével történik. Az állományt akkor nyilváníthatjuk töröltnek, ha a rá vonatkozó hivatkozások száma zéró.

#include <unistd.h>
int unlink(const char *pathname);

A függvény tehát törli a pathname elérési útvonalon (lehet csak állománynév is) keresztül megadott FIFO állományt. A visszatérített érték 0, ha a törlés sikerült, és -1 különben.

  Folyamatok közti kommunikáció FIFO állományok segítségével

A folyamatok közti kommunikáció a FIFO állományok segítségével a következő lépések szerint történik:

a. egy folyamat a szimbolikus név alapján létrehozza a FIFO állományt az mknod vagy mkfifo függvények segítségével,

b. egy folyamat, amely információt szeretne közölni egy másikkal megnyitja a FIFO állományt az open függvénnyel, és a write segítségével beírja az adatokat,

c. egy folyamat, amely az adatokat szeretné kiolvasni, megnyitja a FIFO állományt olvasásra az open függvénnyel, majd a read segítségével kiolvassa a kívánt információkat,

d. egy folyamat a szimbolikus név alapján törli a FIFO állományt az unlink függvénnyel.

  Példák

  1. példa (apa-fiú kapcsolat)

A folyamatok közti kommunikáció kétirányú is lehet. Ekkor két FIFO állományt kell létrehoznunk. A következő példa e kétirányú információcserét valósítja meg egy apa-fiú kapcsolatban.

#include "hdr.h"

int main(void)
{
  int gyerek;
  ...
  mknod("PIPE_FIFO_1", 0666 | S_IFIFO, 0);          /* FIFO fájlok létrehozása */
  mknod("PIPE_FIFO_2", 0666 | S_IFIFO, 0);
  ...
  if ((gyerek = fork()) < 0)                        /* gyerekfolyamat létrehozása */
    err_sys("fork hiba");
  if (gyerek)
    apa();                                          /* P1 folyamat */
  else
    fiu();                                          /* P2 folyamat */
  exit(0);
}

void apa(void)                                      /* P1 folyamat */
{
  int fd1, fd2;
  ...
  if ((fd1 = open("PIPE_FIFO_1", O_WRONLY)) < 0)    /* FIFO fájl megnyitása írásra */
    err_sys("hiba a PIPE_FIFO_1 nyitasa soran");
  if ((fd2 = open("PIPE_FIFO_2", O_RONLY)) < 0)     /* másik FIFO nyitása olvasásra */
    err_sys("hiba a PIPE_FIFO_2 nyitasa soran");
  ...
  write(fd1, ...);                                  /* írás a PIPE_FIFO_1-be */
  ...
  read(fd2, ...);                                   /* olvasás a PIPE_FIFO_2-ből */
  ...
  close(fd1);                                       /* FIFO fájlok bezárása */
  close(fd2);
  exit(0);
}

void fiu(void)                                      /* P2 folyamat */
{
  int fd1, fd2;
  ...
  fd1 = open("PIPE_FIFO_1", O_RDONLY);              /* FIFO fájl megnyitása olvasásra */
  fd2 = open("PIPE_FIFO_2", O_WRONLY);              /* FIFO fájl megnyitása írásra */
  ...
  read(fd1, ...);                                   /* olvasás a PIPE_FIFO_1-ből */
  ...
  write(fd2, ...);                                  /* írás a PIPE_FIFO_2-be */
  ...
  close(fd1);                                       /* FIFO fájlok bezárása */
  close(fd2);
  exit(0);
}

  2. példa (kliens-szerver rendszer)

A következőkben egy olyan FIFO állományt hozunk létre, amely nem csupán apa-fiú kapcsolatoknál működik, hanem tetszőleges két folyamat között. A feladat egy olyan kliens-szerver rendszer (két különálló állományból áll!) létrehozása, amelyben a kliens küld egy számot a szervernek, mire a szerver válaszként visszaküldi a szám négyzetét.

Megjegyzések:

- a szerver létrehoz egy szerverfifot, amelyre az összes kliens csatlakozni fog,

- minden kliensnek külön FIFO-ja van, amelyet minden kliens magának készít el; ezért amikor a kliens a szervernek elküldi a kérést, valahogyan jeleznie kell, hogy milyen nevű FIFO-n keresztül szeretné a választ megkapni; a legegyszerűbb, ha a kliens FIFO-jának nevében szerepel a kliens folyamatazonosítója is, így a név egyértelmű,

- a kliens előbb megnyitja a saját FIFO-ját olvasásra, s csak azután küldi el az üzenetet a szerver felé,

- a szerver FIFO-ja sosem záródik be,

- a kliens FIFO-jának a szerver oldalát a szerver a válaszadás után bezárja; ha újabb kérés érkezik, újból megnyitja,

- ha a kliens befejezte működését be kell zárnia a saját FIFO-ját.

Mivel a FIFO-n küldött adatok típusa megegyezik a szerverben és a kliensben, a könnyebb kezelhetőség érdekében ajánlatos egy közös adatszerkezetet létrehozni, és ezt egy külön hdr állományban tárolni. Esetünkben ez a következő lesz:

struktura.h

typedef struct elem
{
  int szam;
  int pid;
} azon;

A kliens-szerver rendszert megvalósító folyamatok a következők lesznek:

szerver.c

#include <sys/types.h>
#include <sys/stat.h>
#include "struktura.h"                    /* a fent megadott fejléc */

int main(void)
{
  int fd, fd1;                            /* szerver- és kliensfifo */
  char s[15];                             /* kliensfifo neve; pl. fifo_143 */
  azon t;                                 /* küldendő "csomag" */

  mkfifo("szerverfifo", S_IFIFO|0666);    /* a szerver létrehozza a saját fifo-ját */
  fd = open("szerverfifo", O_RDONLY);     /* megnyitja olvasásra; jöhetnek a számok */

  do                                      /* addig megy, míg 0-t nem küld egy kliens */
  {
    read(fd, &t, sizeof(t));              /* szám kiolvasása */
    t.szam = t.szam * t.szam;
    sprintf(s, "fifo_%d", t.pid);         /* a pid segítségével meghat. a kliensfifo nevét */
    fd1 = open(s, O_WRONLY);              /* kliensfifo megnyitása írásra */
    write(fd1, &t, sizeof(t));
    close(fd1);                           /* adatok elküldve, kliensfifo vége */
  } while (!t.szam);
  close(fd);                              /* szerverfifo vége */
  unlink("szerverfifo");                  /* törli a szerverfifot, hiszen ő hozta létre */
  exit(0);
}


kliens.c

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "struktura.h"                    /* a fenti fejlécállomány */

int main(int argc, char *argv[])          /* a számot a parancssorban adjuk meg */
{
  int fd, fd1;                            /* kliens- és szerverfifo */
  char s[15];
  azon t;

  if (argc != 2)                          /* nincs megadva argumentum, hiba */
  {
    printf("hasznalat: kliens <szam>\n");
    exit(1);
  }

  sprintf(s, "fifo_%d", getpid());        /* meghat. a fifonevet a pid segítségével */
  mkfifo(s, S_IFIFO|0666);                /* létrehoz egy kliensfifot */
  fd = open("szerverfifo", O_WRONLY);
  t.pid = getpid();                       /* a küldendő adatok */
  t.szam = atoi(argv[1]);                 /* string átalakítása számmá */
  write(fd, &t, sizeof(t));               /* küldi a szervernek */
  fd1 = open(s, O_RDONLY);
  read(fd1, &t, sizeof(t));               /* a válasz */
  close(fd1);
  unlink(fd1);                            /* kliensfifo törlése */
  printf("a negyzete: %d\n", t.szam);
  exit(0);
}

  Üzenetsorok (message queues)

 

Copyright (C) Buzogány László, 2002



About