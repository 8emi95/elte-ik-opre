      
Csővezeték (pipe)
Folyamatok közti kommunikáció név nélküli állományok segítségével...
 

  Definíció

  Műveletek pipe állományokkal

  Létrehozás (pipe)

  Bezárás (close)

  Olvasás és írás (read, write)

  Példa

  A fájlleíró megduplázása (dup, dup2)

  Kétirányú kommunikáció

  Egyszerűen pipe (popen, pclose, fgets, fputs)

 

 

  Jelzések (signals)

  Definíció

A kommunikációs csatornák működését már láthattuk a Unix shell parancsok végrehajtásakor. Például, ha egy rendezett listát szeretnénk megjeleníteni a hálózatba belépett felhasználókról a következőképpen járunk el:

$ who | sort | pr

A fenti példában három folyamatot két csatornával kötöttünk össze. Az adatok áramlása balról jobbra történik.

Unix alatt azonban pipe-okat programból is létrehozhatunk, amely nagy flexibilitást eredményez, és megoldja a folyamatok közötti körkörös kommunikációt is.

A csővezeték (pipe) a rendszer által létrehozott (név nélküli) speciális állomány. Maximális hossza 10 blokk (azaz 5120 byte). 

Feladata: két folyamat közötti kommunikáció megvalósítása úgy, hogy az egyik folyamat ír a standard kimenetre, a másik pedig olvas a standard bemenetről.

A pipe FIFO szerkezetű (elsőnek be, elsőnek ki). Két mutatója van: egyik az író folyamaté, a másik az olvasóé. Így az írás/olvasás cirkulárisan történik. Ha a pipe megtelik az írónak kell várnia, ha kiürül az olvasónak. A kiolvasott adatok törlődnek a listából.

A FIFO algoritmus szabályainak betartása érdekében, egy pipe állományt csak az őt létrehozó folyamat és annak leszármazottai használhatnak. Általában a pipe-ot egy olyan folyamat hozza létre, amely később meghív egy fork függvényt, és ezáltal a pipe mind a szülő, mind a gyerek számára láthatóvá válik. A fork hívás után az állománytábla bemenetei közösek lesznek.

Tehát levonhatjuk a következtetést: pipe fájlokat csak apa-fiú kapcsolatban levő vagy közös őssel rendelkező folyamatok használhatnak.

A pipe fájlok szintjén a következő műveleteket kell (lehet) elvégezni:

1. az állomány létrehozása a pipe függvény segítségével,

2. olvasás/írás a read/write függvények segítségével,

3. az O_NDELAY mutató beállítása az fcntl függvénnyel, amely a következőket eredményezi: ha a read vagy write függvények nem fejeződnek be, az a folyamat, amely ezeket meghívta nem fog leállni, viszont a read/write függvények befejeződnek és 0 eredményt szolgáltatnak,

4. az állomány bezárása a close függvény segítségével,

5. az állomány (pontosabban az i-node bemenet) törlése; ezt a rendszer automatikusan végzi, amint az i-node-ra történő hivatkozások száma 0 lesz.

  Műveletek pipe állományokkal

  Létrehozás (pipe)

Egy csővezeték létrehozása a pipe függvény segítségével történik, amelynek alakja:

#include <unistd.h>
int pipe(int pfd[2]);

A függvény 0-t térít vissza, ha a létrehozás sikerült, és -1-et, ha nem.

A pfd egy két elemű táblázat, ahol a pfd[0]-ból olvasunk, és a pfd[1]-be írunk.

A pipe függvényhívás tehát egy olyan kommunikációs csatornát hoz létre, amelynek az egyik végén beírjuk az adatokat, a másik végén pedig kiolvassuk. A pfd[0]-ba való írás során (write) az adatok a pipe fájlba kerülnek, míg a pfd[1]-ből olvasva (read) törlődnek onnan.

Hiba esetén az errno változó a hiba kódját fogja tartalmazni.

  Bezárás (close)

A csővezeték (azaz a pipe fájl) bezárására szolgál a close függvény. Szintaxisa:

#include <unistd.h>
int close(int pfd);

A függvény 0-t térít vissza, ha a bezárás sikerült, és -1-et különben.

Látható, hogy a pfd argumentum egy egész szám, tehát csak az állomány egyik végét zárja be. A nem szükséges pipe végeket ajánlatos minél előbb bezárni! Például, ha egy programban a szülő információkat küld a gyereknek pipe-on keresztűl, akkor nem szükséges a pfd[0] a szülőben (mivel ő nem olvas), sem a pfd[1] a gyerekben (mivel ő nem ír).

  Olvasás és írás (read, write)

Mivel a csővezeték is egyfajta fájl, a pipe olvasása és írása az állományokéhoz hasonlóan történik.

#include <unistd.h>

ssize_t read(int pfd, void *buf, size_t count);
ssize_t write(int pfd, const void *buf, size_t count);

vagy

#include <stdio.h>

int fscanf(FILE *stream, const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);

A második változatot főként standard fájlok esetén használjuk. Pipe fájlok kezelésére a read és write függvényeket ajánljuk. Itt paraméterként meg kell adnunk a pipe fájl egyik végének azonosítóját (pfd), a buf paraméterben a beírandó vagy kiolvasandó információt, míg a count változóba ennek méretét.

A függvények visszatérített értéke a pipe-ból sikeresen kiolvasott (beírt) bájtok száma.

  Példa

Készítsünk egy apa és egy fiú folyamatot, majd küldjünk el egy szöveget az apától a fiúnak pipe vezeték segítségével. A gyerek írja ki, hogy hány bájtot olvasott, és jelenítse meg az üzenet szövegét.

#include "hdr.h"

int main(void)
{ 
  int pfd[2], n;                              /* pipe változó, olvasott bájtok száma */
  pid_t pid;

  char buf[MAXLINE];                          /* ebbe fogja kiolvasni a gyerek az inf. */
  char test[] = "pipe teszt\n";               /* ezt küldi a gyerek */

  if (pipe(pfd) < 0)                          /* pipe létrehozása */
    err_sys("pipe hiba");

  if ((pid = fork()) < 0)                     /* gyerekfolyamat létrehozása */
    err_sys("fork hiba");
  else
    if (pid == 0)                             /* gyerek */
    {
      close(pfd[1]);                          /* fölösleges vezeték bezárása */
      n = read(pfd[0], buf, MAXLINE);         /* információ kiolvasása */
      printf("beolvastam %d bajtot: \n", n);
      fflush(stdout);
      write(1, buf, n);
    }
    else                                      /* szülő */
    {
      close(pfd[0]);                          /* fölösleges vezeték bezárása */
      write(pfd[1], test, sizeof(test));      /* információ beírása a pipe-ba */
    }
  exit(0);
}

  A fájlleíró megduplázása (dup, dup2)

Sok program az adatokat a standard bemenetről olvassa, amelynek fájlleírója 0, vagy az adatokat a standard kimenetre írja, amelyhez az 1 fájlleíró tartozik. Tehát ahhoz, hogy két programot a parancssorban össze tudjunk kapcsolni (cd1 | cd2), a pipe fájlhoz ezen két fájlleírót (tehát a standard kimenetet és bemenetet) kell kapcsolnunk. A két fájlleíró egyszerű megnyitása a pipe hívás előtt egyáltalán nem garantálja, hogy pontosan ezen fájlleíró értékeket fogják tartalmazni. Ezért, a kívánt fájlleírók megnyitása után szükséges a pipe fájlleírók megduplázása a dup és a dup2 függvények segítségével.

Ez azt jelenti, hogyha a dup meghívása előtt már megnyitottuk a 0 fájlleírót, ez a hívás biztosan 0-val fog visszatérni. A függvények szintaxisa:

#include <unistd.h>

int dup(int pfd);
int dup2(int pfd, int npfd);

A függvény az új fájlleírót téríti vissza, ha a művelet sikerült, illetve -1-et hiba esetén.

A dup sajátossága, hogy a nem használatosak közül mindig a legkisebb számú fájlleírót téríti vissza (azaz hozzárendeli a pfd változóhoz). Ezzel ellentétben a dup2 az npfd változó által megadott új fájlleírót rendeli a pfd-hez.

A pipe-okhoz rendelt fájlleírók megduplázását megértése érdekében nézzük a következő példát, amely a parancssorban megadott fájlt a képernyőre írja. A program az állomány tartalmát oldalról-oldalra jeleníti meg. Azért, hogy az egész állomány tartalmát ne kelljen egy időszakos állományba írni, s azután a system függvénnyel kiíratni, e program kimenetelét össze kell kapcsolnunk a lapok formázását végző programmal. Ennek érdekében előbb létrehozunk egy pipe állományt, majd a fork függvény segítségével egy gyereket. Ezután a gyerekfolyamat standard kimenetét hozzárendeljük az olvasó fájlleíró vezetékéhez, végül pedig meghívjuk az exec függvényt, hogy az oldalformázásokat elvégezze.

#include <sys/wait.h>
#include "hdr.h"
#define DEF_PAGER "/usr/bin/more"

void main(int argc, char *argv[])
{
  int pfd[2], n;
  pid_t pid;
  char buf[MAXLINE], *pager, *arg;
  FILE *fp;

  if (argc != 2)
    err_quit("hasznalat: a.out <path>");

  if (pipe(pfd) < 0)
    err_sys("pipe hiba");

  if ((fp = fopen(argv[1], "r")) == NULL)
    err_sys("fopen hiba %s", argv[1]); 

  switch (pid = fork())
  {
    case -1:
      err_sys("fork hiba"); 
    case 0:                                     /* gyerek olvas */
      close(pfd[1]);
      if (pfd[0] != 0) 
      {
        if (dup2(pfd[0], 0) != 0)
          err_sys("dup2 hiba"); 
        close(pfd[0]);
      }
      if ((pager = getenv("PAGER")) == NULL)
        pager = DEF_PAGER;
      if ((arg = strrchr(pager, '/')) != NULL)
        arg++;
      else
        arg = pager;
      if (execl(pager, arg, NULL) < 0) 
        err_sys("execl hiba: %s", pager);
  default:                                      /* szülő ír */
    close(pfd[0]);
    while (fgets(buf, MAXLINE, fp) != NULL) 
    {
      n = strlen(buf);
      if (write(pfd[1], buf, n) != n)
        err_sys("write hiba"); 
    }
    if (ferror(fp))
      err_sys("ferror hiba"); 
    close(pfd[1]);
    if (waitpid(pid, NULL, 0) < 0)
      err_sys("waitpid hiba"); 
    exit(0);                                   /* azért nincs több kommentár, hogy  */
  }                                            /* mindenki alaposan gondolja át ;-) */
}

  Kétirányú kommunikáció

Az előbbi példákban a kommunikáció csak egyirányú volt, azaz mindig a szülőtől haladt az információ a gyerekhez. Egy apa-fiú kapcsolatban akkor akkor beszélhetünk kétirányú kommunikációról, ha a gyerek is tud adatot küldeni a szülő felé. Ezt, egyszerűen két pipe fájllal fogjuk megvalósítani.

Nevezzük a két pipe-ot például pfd1 és pfd2-nek. Oldjuk meg a következő feladatot: a szülő elküld a gyereknek egy n egész számot, mire a gyerek 0-t küld vissza, ha a szám páros, és 1-et, ha páratlan.

A pipe mechanizmust a következő ábra szemlélteti:



A program pedig a közetkező:

#include "hdr.h"

int main(void)
{
  int n, k, pfd1[2], pfd2[2];
  printf("kerem a szamot:\n");            /* az n bekérése */
  scanf("%d", &n);
  pipe(pfd1);                             /* a két pipe fájl létrehozása */
  pipe(pfd2);
  if (fork())                             /* itt be lehet tenni még hibaellenőrzést is */
  {                                       /* szülő */
    close(pfd1[0]);                       /* fölösleges pipe-ok bezárása */
    close(pfd2[1]);
    write(pfd1[1], &n, sizeof(int));      /* az n elküldése a pfd1 pipe-on keresztül */
    close(pfd1[1]);
    read(pfd2[0], &k, sizeof(int));       /* az eredmény kiolvasása a pfd2-n keresztül */
    if (k == 0)
      printf("a szam paros\n");           /* az eredmény kiírása */
    else
      printf("a szam paratlan\n");
    exit(0);
  }
  else                                    /* gyerek */
  {
    close(pfd1[1]);
    close(pfd2[0]);
    read(pfd1[0], &n, sizeof(int));       /* az n kiolvasása a pfd1 pipe-ból */
    close(pfd1[0]);
    k = n%2;                              /* k=1, ha az n páratlan, k=0, ha n páros */
    write(pfd2[1], &k, sizeof(int));      /* az eredmény elküldése a pfd2 pipe-on */
    close(pfd2[1]);
    exit(0);
  }
}

  Egyszerűen pipe (popen, pclose)

A popen és a pclose függvények bizonyos esetekben megkönnyítik a pipe-ok használatát. A popen a következőket teszi:

- létrehoz egy pipe-ot (pipe),
- létrehoz egy gyerekfolyamatot (fork),
- a kommunikáció irányától függően bezárja a nem szükséges csatornákat (close),
- a gyerekfolyamatban végrehajtja a megadott parancsot (exec),
- megvárja, ameddig a parancs befejeződik (wait).

A két függvény alakja:

#include <stdio.h>

FILE *popen(const char *cmd, const char *type);
int pclose(FILE *fp);

A popen egy állományra mutató pointert ad vissza, ha a művelet sikeres volt, és NULL értéket egyébként. A pclose függvény visszaadja a cmd parancs végrehajtásának befejeződési állapotát, vagy a -1 értéket hiba esetén.

A popen függvény tehát létrehoz egy gyerekfolyamatot (fork), majd az exec függvény segítségével végrehajtja a cmd parancsot. Ha a type argumentum értéke "r", az állományhoz rendelt mutató a cmd parancs kimenete lesz (tehát a fájlból olvashatunk). Ha az argumentum értéke "w", a mutató a cmd parancs bemenetéhez lesz hozzárendelve (így írni tudunk a fájlba).

A pclose lezárja az állománymutatókat, és megvárja a gyerekfolyamat által végrehajtott parancs befejezését, majd visszatéríti a parancs kilépési kódját.

Feladat: Listázzuk egy állomány tartalmát a more parancs segítségével!

#include "hdr.h"

int main(int argc, char *argv[])                /* argumentumok átadása */
{
  if (argc != 2)                                /* argumentumok számának ellenőrzése */
    err_quit("hasznalat: a.out <fajlnev>");

  char buf[MAXLINE];
  int err;

  FILE *megadott;                               /* az arg.-ban megadott, bemeneti fájl */
  FILE *fp;                                     /* ez lesz a more parancs bemenete */

  megadott = fopen(argv[1], "r");               /* a megadott fájl megnyitása olvasásra */
  fp = popen("more", "w");                      /* a more parancs futtatása gyerekként */

  while (fgets(buf, MAXLINE, megadott)          /* a fájl átküldése a more bemenetére */
    fputs(buf, fp);

  err = pclose(pf);                             /* állománymutatók bezárása */
  fclose(megadott);
  if (err == -1)                                /* hiba kezelése */
    err_sys("more hiba");
}

Amint a fenti példában is láthatjuk külön függvények vannak a popen által megnyitott fájlok írására és olvasására.

Ha a popen-t olvasásra ("r") nyitottuk meg, akkor az fgets függvényt kell használnunk. Alakja:

#include <stdio.h>
char *fgets(char *s, int size, FILE *stream);

A függvény size bájtot próbál kiolvasni a stream fájlból. Az eredményt (sikeresen kiolvasott információt) az s stringben kapjuk vissza. A függvény visszatérített értéke szintén s, ha az olvasás sikeres volt, különben NULL.

Ha a popen-t írásra ("w") nyitottuk meg, akkor az fputs függvényt használjuk:

#include <stdio.h>
int fputs(const char *s, FILE *stream);

A függvény a stream fájlba írja az s változóban megadott stringet. A visszatérített érték egy nemnegatív szám, ha a művelet sikeres volt, különben EOF.

Feladat: A következőkben olyan C programot írunk, amely két folyamatot hoz létre: egy szervert és egy klienst. A kliensfolyamat egy kérést intéz a szerverhez, mire a szerver a popen parancs segítségével megkeresi a választ, majd egy pipe fájlon visszaküldi az eredményt a klienshez. Ebben az esetben a kliens küldjön a szervernek egy host nevet, a szerver pedig ellenőrizze, hogy az illető host létezik-e vagy sem, és küldjön vissza a szervernek egy megfelelő üzenetet.

#include "hdr.h"

int main(int argc, char *argv[])
{
  int pipe1[2], pipe2[2];                     /* két pipe a kétirányú kommunikációhoz */
  int pid;
  char all[3];

  if (argc != 2)                              /* ha a parancssorban nincs paraméter */
    err_quit("hasznalat: hc <hostnev>\n");

  pipe(pipe1); // client --> server           /* pipe-ok létrehozása */
  pipe(pipe2); // server --> client

  switch (pid = fork())                       /* gyerekfolyamat létrehozása */
  {
    case -1 :                                 /* hiba a létrehozáskor */
      err_quit("fork hiba");
    case 0 :                                  /* gyerek - szerver */
    {
      FILE *ps;                               /* a popen ebből fog olvasni */
      char shellcmd[200];                     /* kiadandó shell parancs */
      char c;                                 /* az eredmény karakterenként itt lesz */
      int i;

      close(pipe1[1]);                        /* pipe1 --> - olvasás a pipe1-ből */
      close(pipe2[0]);                        /* --> pipe2 - írás a pipe2-be */

      strcpy(shellcmd, "ping -c 3 ");         /* a shell parancs eleje */
      i = strlen(shellcmd)-1;

      do 
      {
        read(pipe1[0], &c, sizeof(char));     /* olvassa a host nevet ... */
        if (i < 149) i++;
        shellcmd[i] = c;                      /* ... hozzátoldja a shell parancshoz */
      } while (c != 0);

      ps = popen(shellcmd, "r");              /* shell parancs vegrehajtasa popen-nel */
      do
      {
        c = fgetc(ps);                        /* eredmény kiolvasása a popen-ből */
        write(pipe2[1], &c, sizeof(char));    /* eredmény küldése a kliensnek */
      } while (c != EOF);

      pclose(ps);                             /* shell parancs vége */

      exit(0);                                /* folyamat vége */

    }
    default :                                 /* szülő - kliens */
    {
      char c;

      close(pipe1[0]);                        /* --> pipe1 - írás a pipe1-be */
      close(pipe2[1]);                        /* pipe2 --> - olvasás a pipe2-ből */

      write(pipe1[1], argv[1], strlen(argv[1])+1);
      wait();                                 /* a host név elküldve; várakozás */

      do
      {
        read(pipe2[0], &c, sizeof(char));     /* válasz kiolvasása a pipe2-ből */
        if (c != EOF)
          printf("%c", c);                    /* válasz kiírása */
      } while (c != EOF);

      waitpid(pid, all, 0);

      exit(0);                                /* kliens vége */
    }
  }
}

  Névvel ellátott csővezeték (FIFO)

 

Copyright (C) Buzogány László, 2002



About