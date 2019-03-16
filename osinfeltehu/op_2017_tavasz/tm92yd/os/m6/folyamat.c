      
Folyamatokkal végzett műveletek
Folyamatok létrehozása, végrehajtása, befejezése...
 

  Bevezető

  Folyamat létrehozása (fork)

  Folyamat befejezése (exit)

  Várakozás egy folyamatra (wait, waitpid)

  Program meghívása, futtatás (exec)

  Folyamat attribútumainak lekérdezése (getpid, getppid, getuid, getgid, geteuid, getegid, setuid, setgid)

  Folyamatcsoportok (getpgrp, setpgid)

  Parancs végrehajtása (system)

  Folyamat prioritásának változtatása (nice)

 

 

  ANSI C

  Bevezető

A Unixban folyamatkezelés alatt a következőket értjük:

- folyamat létrehozása,
- programok meghívása,
- folyamat várakoztatása addig, amíg a gyerekfolyamat befejeződik,
- folyamat befejezése.

Amint már említettük, minden folyamatnak egyedi azonosítója (PID-je) van: egy pozitív egész szám.

Egy Unix rendszerben léteznek speciális folyamatok, amelyek a rendszer indulásakor töltődnek be:

- A 0 azonosítójú folyamat mindig az ütemező, neve swapper. Ez a folyamat része a rendszer magjának és rendszerfolyamatnak számít.

- Az 1 azonosítójú folyamat az init (/etc/init, újabb verziókban /sbin/init), amelyet a rendszer magja hív meg a rendszer betöltése után. Ez a folyamat folyamatosan beolvassa a rendszertől függő inicializációs fájlokat és stabilizálja a rendszert. Az init (a swapper-rel ellentétben) felhasználói folyamat, de futtatáskor mégis superuser jogokkal rendelkezik. Az init folyamat nem fejeződik be soha!

- A Unix egyes, virtuális memóriát is használó implementációiban a 2 azonosítóval rendelkező folyamat a pagedaemon. Ez a rendszerfolyamat a lapozásért felel.

  Folyamat létrehozása (fork)

Egy folyamatot a fork rendszerfüggvénnyel hozhatunk létre. Azt a folyamatot, amely a fork-ot hívta szülőnek, az újonnan létrehozott folyamatot pedig gyereknek (fiúnak) nevezzük.

A fork függvény szintaxisa:

#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

A visszaadott pid_t típusú érték:

0 – a gyerekben,
a gyerek PID-je – a szülőben,
-1 – hiba esetén.

Hiba esetén az errno változó a hiba kódját fogja tartalmazni.

Egy folyamat több gyereket is létrehozhat. Mivel nincs egyetlen olyan függvény sem, amellyel meg lehetne egy gyerek PID-jét változtatni, a fork függvény a szülőnek visszaadja a (létrehozott) gyerek folyamatazonosítóját. A gyerekfolyamatban ez az érték 0, hiszen egy gyereknek csak egy szülője lehet. 

Egy gyerek a szülője azonosítóját a getppid függvénnyel kérdezheti le. A saját PID lekérdezésére a getpid függvény szolgál.

#include <sys/types.h>
#include <unistd.h>

pid_t getppid(void);
pid_t getpid(void);

Figyelem! A gyerekfolyamat és a szülőfolyamat kódját ugyanabban az állományban kell megírni! A fork hívás után a szülő- és a gyerekfolyamat két különböző folyamatként, egyidőben fog futni! A végrehajtást mindkettő a fork utáni első utasítással fogja folytatni.

Tehát: a két folyamat (szülő és gyerek) ugyanazon kódszegmenst használják, az adat- és veremszegmensük viszont különböző!

Példa:

#include <stdio.h>
#include <sys/types.h>

int main(void)
{
  pid_t pid;
  pid = fork();
  printf("%d folyamat, pid = %d\n", getpid(), pid);
}

A fenti program végrehajtásának eredménye:

$ a.out
226 folyamat, pid = 207
207 folyamat, pid = 0

Mivel a fork hívás után két azonos kódszegmensű folyamat jött létre, a printf függvényt mind a szülő, mind pedig a gyerek végrehajtja. A szülő egyrészt kiírja a saját azonosítóját (226), másrészt a gyerek PID-jét (207). A gyerek szintén előbb a saját PID-jét (207), majd a 0 értéket jeleníti meg (hiszen a pid változó értéke a gyerekben 0).

Azért, hogy a két kódrészt (gyerek-szülő) jól elkülönítsük, a fork-ot a következőképpen szoktuk használni:

pid = fork();
if (pid == 0)
{
  /* gyerek folyamat */
}
else
{
  /* szülő folyamat */
}

Az alábbi példában a hibát is kezeljük:

switch (fork())
{
  case -1:
    perror("fork");
    exit(1);
  case 0:
    /* gyerek folyamat */
  default:
    /* szülő folyamat */
}

Észrevehetjük, hogy a szülő- és a gyerekfolyamatok különböznek, külön azonosítóval rendelkeznek, mégis nagyon sok közös attribútumuk van. A fork előtt deklarált változókat mind a szülő, mind a gyerek láthatja, de értékük a későbbi módosítások során csak az aktuális folyamatban változik.

#include <sys/types.h>

int gvar = 4;

int main(void)
{
  pid_t pid;
  int var = 7;
  printf("a fork elott\n");
  if ((pid = fork()) == -1)
    err_sys("fork hiba");
  else
    if (pid == 0)
    {
      gvar++;
      var += 2;
    }
    else
      sleep(2);
  printf("folyamat(pid)=%d, gvar=%d, var=%d\n", getpid(), gvar, var);
  exit(0);
}

A fent definiált két folyamat közül az egyiket a rendszer hamarabb fogja végrehajtani de, hogy melyiket, azt nem tudhatjuk előre (még a sleep(2) utasítás sem garantálhatja, hogy a gyerek folyamat fog hamarabb lefutni). Ezért futtatáskor akár két különböző eredményt is kaphatunk:

$ a.out
a fork elott
folyamat(pid)=184, gvar=5, var=9
folyamat(pid)=183, gvar=4, var=7

$ a.out
a fork elott
folyamat(pid)=187, gvar=4, var=7
folyamat(pid)=188, gvar=5, var=9

Látható, hogy a két futtatás eredményeként különböző folyamatazonosítókat osztott ki a rendszer.

  Folyamat befejezése (exit)

Egy folyamat szabályszerű, önmaga által történő, azonnali leállítása az exit, illetve az _exit függvényekkel történik. (Az előbbi ANSI C utasítás, míg a második csak a POSIX-ben található meg, és tartalmazza a Unix sajátosságokat.)

#include <stdlib.h>
void exit(int exit_code);

#include <unistd.h>
void _exit(int exit_code);

Az exit_code a kilépési kód, amelyet a hívó program használ ellenőrzésre. Egy folyamat befejezésekor az összes gyerekfolyamat átöröklődik az init (1) folyamathoz (tehát új szülőt kap). A rendszer ezáltal biztosítja, hogy minden folyamatnak legyen szülője.

Ha egy program nem tartalmazza az exit utasítást, a rendszer automatikusan végrehajtja az exit-et a main függvény befejezése után.

Rendellenes működés esetén a folyamat befejezésére használjuk az abort függvényt. Szintaxisa:

#include <stdlib.h>
void abort(void);

Amennyiben egy gyerekfolyamat hamarabb befejeződik, mint a szülő a rendszer bizonyos információkat még megőriz vele kapcsolatban (PID, befejeződési állapot, elhasznált processzoridő). Ezen információk a wait és a waitpid függvények segítségével érhetők el. Ezek a függvények felfüggesztik a folyamat működését, ameddig a gyerekfolyamat be nem fejeződik.

A Unix felfogása szerint, azt a folyamatot, amely befejeződött, de a szülő nem adott ki wait parancsot, zombie folyamatnak nevezzük. Ebben az állapotban a folyamatnak nincs semmilyen lefoglalt memóriaterülete, csak egy bemenete a folyamattáblában. A rendszer felhasználhatja a lefoglalt memóriazónákat, illetve bezárhatja az általa megnyitott fájlokat. A zombie folyamatokat a ps parancs segítségével követhetjük nyomon.

Példa:

#include "hdr.h"

int main(void)
{
  pid_t pid;
  if ((pid = fork()) == -1)
    err_sys("fork hiba");
  else
    if (pid == 0)
      exit(0);
  sleep(3);
  system("ps");
  exit(0);
}

A fenti program által kiírt eredmény:

PID TTY  STAT TIME COMMAND
54  v01  S    0:00 -bash
90  v01  S    0:00 a.out
91  v01  Z    0:00 (a.out) <zombie>
92  v01  R    0:00 ps

Egy folyamat, amelynek szülője az init, sosem kerülhet zombie állapotba, mivel az init mindig meghív egy wait függvényt, ellenőrizve ezzel a befejezett folyamat állapotát.

  Várakozás egy folyamatra (wait, waitpid)

Ha egy folyamat befejeződik, a rendszer egy SIGCLD üzenettel értesíti a szülőfolyamatot. Ezek után a szülő figyelmen kívül hagyja az illető folyamatot.

Előfordulhat, hogy a szülőnek meg kell várnia a gyerekfolyamat lefutását, s csak azután tud valamilyen feladatot megoldani. Erre (azaz például várakozásra) használjuk a wait és a waitpid függvényeket. Egy folyamat, amely meghívja a wait vagy a waitpid függvényeket:

- várakozhat (ha minden gyereke fut),
- érzékelheti, hogyha egy gyerek befejeződött,
- visszatéríthet hibát (ha nincs gyereke).

Szintaxisuk:

#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int opt);

A status egy mutató egy olyan táblázatra, amely egy gyerekfolyamat befejeződési állapotát tartalmazza. Ez a 16 bites információ a következőket tartalmazza:

- ha a gyerekfolyamat exit-tel ért véget, az állapot (status) változó a következőket tartalmazza:

       

ahol az exit_code az exit függvény által megadott érték.

- ha a gyerekfolyamat egy jelzés hatására ért véget, a status változó:

       

ahol az x értéke 1, ha a jel üres memóriaterületet eredményezett, és 0 különben. A jel_id annak a jelnek az azonosítója, amely a gyerekfolyamat befejezését eredményezte.

- ha a gyerekfolyamatot leállították, a status változó:

       

ahol a jel_id annak a jelnek az azonosítója, amely a folyamatot leállította

Különbségek a wait és a waitpid között:

- a wait felfüggeszti a hívó folyamatot, amíg a gyerek befejeződik, ezzel szemben a waitpid egy külön opciót kínál fel (opt), melynek használatával a felfüggesztés elkerülhető,

- a waitpid nem mindig az első fiú befejezéséig vár, hanem a pid változóban megadott azonosítójú gyerek befejezéséig,

- a waitpid az opt argumentum segítségével engedélyezi a programok vezérlését.

A wait függvény visszatérési értéke azon gyerekfolyamat azonosítója, amely éppen befejeződött. Mivel tehát a függvény a folyamat PID-jét téríti vissza, mindig pontosan tudjuk, mely gyerek fejeződött be éppen. Egy bizonyos gyerek befejeztét megvárhatjuk például így:

while (wait(allapot) != pid);

ahol a pid azon gyerekfolyamat azonosítója, amelyre várakozunk.

A waitpid függvényhívásnál megadható pid változó lehet:

- pid = -1  –  bármely gyerekre várakozhat; ekvivalens a wait-tel,
- pid > 0  –  a pid azonosítójú folyamatra várakozik,
- pid = 0  –  bármely olyan folyamatra várakozik, amelynek a csoportazonosítója megegyezik a hívó programéval,
- pid < -1  –  bármely olyan folyamatra várakozik, amelynek a csoportazonosítója megegyezik a hívó programéval abszolút értékben.

A waitpid -1 értéket térít vissza, ha nem létezik a pid-ben megadott azonosítójú folyamat, vagy nem gyereke a hívó folyamatnak.

  Program meghívása, futtatás (exec)

Az exec parancs az aktív folyamat kódját egy másikkal helyettesíti (azaz átadja a vezérlést egy másik programnak). A program ezáltal egy teljesen új kódszegmenst, és egy ennek megfelelő adatszegmenst kap. Az új programnak egy futtatható állománynak kell lennie.

Az exec utasítás különböző végződésekkel rendelkezhet. A különbségeket a következő táblázat szemlélteti:

 
 Függvények
Paraméter
Keresési útvonal
Környezet
 
 
execl
lista
./
marad
 
 
execv
tömb
./
marad
 
 
execlp
lista
PATH
marad
 
 
execvp
tömb
PATH
marad
 
 
execle
lista
./
változik
 
 
execve
tömb
./
változik
 
 

A fenti végződések jelentése a következő:

l – a paraméterátadás listán keresztül történik,
v – a paraméterátadás tömbön keresztül történik,
e – változik a környezeti változó,
p – figyelembe veszi a PATH változót.

A függvények szintaxisa:

#include <unistd.h>

int execl(const char *path,       /* elérési út */
          const char *arg0,       /* programnév */
          const char *arg1,       /* paraméterek */ 
          ...
          const char *argn,
          NULL);                  /* a paraméterek befejeztét jelző NULL */

int execv(const char *path, char *argv[]);

int execlp(const char *filename,  /* a futtatható állomány neve */
           const char *arg0,
           const char *arg1
           ...
           const char *argn,
           NULL);

int execvp(const char *filename, char *argv[]);

int execle(const char *path,
           const char *arg0,
           const char *arg1,
           ...
           const char *argn,
           NULL,
           char *envp[]);         /* környezeti változók */

int execve(const char *path, char *argv[], char *envp[]);

Az egyes változók jelentései:

- path: mutató egy karaktersorhoz, amely a futtatható állomány keresési útvonalát jelöli,
- arg0: mutató a futtatható állomány nevéhez,
- arg1, arg2, ..., argn: mutatók, amelyek a programnak átadott paramétereket jelölik,
- argv: mutató a paramétervektorhoz (a 0-dik paraméter az állomány neve),
- filename: mutató a futtatható állomány nevéhez; ha a név nem kezdődik a gyökérrel (és nincs megadva a teljes útvonal), akkor az állományt a PATH változó által definiált könyvtárakban keresi a rendszer,
- envp: mutató az új környezeti változókhoz, amelyek a vektorban egyenként változó=érték alakban jelennek meg.

Amikor a paramétereket listaként adjuk meg, az utolsó paramétert követően beírunk egy NULL paramétert is. Ez jelzi a felsorolás végét. 

Példák:

execl("/bin/time", "time", "ps", NULL);

...
char *argv[3];
argv[0] = "time";
argv[1] = "ps";
argv[2] = (char *)0;
execv("/bin/time", argv);

execlp("prog", "prog", "p1", "p2", NULL);

...
char *argv[4];
argv[0] = "prog";
argv[1] = "p1";
argv[2] = "p2";
argv[3] = (char *)0;
execvp("prog", argv);

...
char *envp[2];
envp[0] = "term=vt100";
envp[1] = (char *)0;
execle("prog", "prog", "p1", "p2", 0, envp);

Feladat: írjunk programot, amely billentyűzetről bekér Unix parancsokat és végrehajtja őket, pontosan úgy, ahogy a shell!

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
  int MAXLINE = 1024;
  char buf[MAXLINE];
  pid_t pid;
  int status;

  printf("> ");
  while (fgets(buf, MAXLINE,stdin) != NULL) 
  {
    buf[strlen(buf)-1] = 0;
    if ((pid = fork()) < 0)
    {
      printf("fork hiba\n");
      exit(1);
    }
    else
      if (pid == 0)
      {
        execlp(buf,buf,NULL);
        printf("nem lehet vegrehajtani: %s\n", buf);
        exit(127);
      }
    if ((pid = waitpid(pid, &status, 0)) < 0)
    {
      printf("waitpid hiba\n");
      exit(1);
    }
    printf("> ");
  }
  printf("befejeztem\n");
  exit(0);
}

A végrehajtandó parancsra várva a program megjeleníti a promptot (>). A programból a Cltr-\ megszakítási jelzéssel tudunk kilépni.

A program futtatásának eredménye lehet például a következő:

$ a.out
> date
Wed Dec 25 09:56:37 EST 2002
> pwd
/home/lacka
> who
lacka   vc/1   Dec 25 07:16
> Ctrl-\
$

  Folyamat attribútumainak lekérdezése

A Unix minden folyamathoz legkevesebb 6 azonosítót rendel. Ezeket céljuk szerint a következőképpen lehet csoportosítani:

a. amelyek a valódi felhasználót azonosítják (azt a személyt aki a felhasználónevével belépett):
- a valódi felhasználó ID-je,
- a valódi csoport ID-je,

b. amelyek meghatározzák a hozzáférési jogát egy állományhoz:
- effektív felhasználó ID-je,
- effektív csoport ID-je,
- a csoport kiegészítő azonosítói (egyes verziókban),

c. amelyeket elmentett az exec függvény:
- elmentett felhasználó ID,
- elmentett csoport ID.

Amint már láttuk egy folyamat azonosítójának lekérdezésére a getpid függvény szolgál, míg a szülő azonosítóját a getppid függvény szolgáltatja.

Egy folyamat valódi felhasználójának azonosítóját a getuid, a valódi csoport azonosítóját pedig a getgid függvénnyel lehet lekérdezni. Az azonosítók effektív változatait a geteuid és a getegid függvények adják. Ezek szintaxisa:

#include <sys/types.h>
#include <unistd.h>

uid_t getuid(void);
gid_t getgid(void);
uid_t geteuid(void);
gid_t getegid(void);

A valódi felhasználó és csoport azonosítójának módosítására is van lehetőségünk a setuid és a setgid függvények által. Itt meg kell adnunk az új azonosítókat.

#include <sys/types.h>
#include <unistd.h>

int setuid(uid_t uid);
int setgid(gid_t gid);

Mind a két függvény 0-t térít vissza, ha a módosítás sikerült, és -1-et hiba esetén.

  Folyamatcsoportok

Unixban minden folyamat egy csoport része. Egy folyamatcsoport több folyamatból álló gyűjtemény, amely egyedi azonosítóval rendelkezik (ennek típusa megegyezik a folyamatazonosítók típusával).

A getpgrp függvény visszatéríti annak a csoportnak az azonosítóját, amelyhez a hívást végző folyamat tartozik. Ennek szintaxisa a következő:

#include <sys/types.h>
#include <unistd.h>

pid_t getpgrp(void);

Minden folyamatcsoportnak van egy ún. csoportvezetője. Ezt könnyen meghatározhatjuk, mivel ennek a folyamatnak az azonosítója éppen megegyezik a csoport ID-jével.

Egy folyamat csatlakozhat egy már létező csoporthoz, vagy létrehozhat egy új csoportot a setpgid függvény használatával. Ennek alakja:

#include <sys/types.h>
#include <unistd.h>

int setpgid(pid_t pid, pid_t pgid);

A függvény 0-t ad vissza, ha a csoport létrehozása sikerült, és -1-et, ha nem. Továbbá a pid azonosítójú folyamatot hozzárendeli az újonnan létrehozott pgid azonosítójú csoporthoz.

Egy folyamat a csoportja számára adhatja a saját ID-ját, vagy valamelyik gyereke azonosítóját. Ellenben nem változtathat meg egy csoport ID-t valamely gyereke számára, ha a gyerek már meghívott egy exec függvényt.

Ha a pid értéke 0, a csoport a hívó program ID-ját fogja használni, ha viszont a pgid 0, akkor a pid által meghatározott azonosítót.

  Parancs végrehajtása (system)

A system függvény segítségével egy C programban is végrehajthatunk bármilyen Unix parancsot.

#include <stdlib.h>
int system(const char *cmdstr);

A parancsot a cmdstr karaktersorban kell megadnunk, a visszaadott érték pedig -1, ha a végrehajtás nem sikerült, illetve a visszatérési állapot minden más esetben.

Figyelem! A system igen hasonlít az exec-hez, de ő előbb egy fork hívással létrehoz egy új gyerekfolyamatot, amelyben az exec segítségével végrehajtja az adott parancsot. Éppen ezért a system nem adja át a vezérlést, hanem a gyerek befejeztével a soron következő utasítással folytatja az aktuális folyamat végrehajtását.

Példa:

...
system("ps");
...

  Folyamat prioritásának változtatása (nice)

Egy folyamat prioritásának megváltoztatására szolgál a nice függvény. Alakja:

#include <unistd.h>
int nice(int inc);

A függvény hozzáadja az inc értéket a hívó folyamat nice értékéhez. (Egy folyamatnak minél nagyobb a nice értéke, annál kisebb a prioritása.) Csak a superuser adhat meg negatív értékeket, s növelheti ezáltal egy folyamat futási sebességét.

A visszatérített érték 0, ha a művelet sikeres volt, és -1 különben.

Egy folyamat prioritását a rendszer a következő képlettel határozza meg:

folyamat_prioritás = alap_prioritás + elhasznált_processzoridő / konstans + nice_érték

A nice érték minden esetben egy -20 és 19 közötti egész szám.

  Jelzések (signals)

 

Copyright (C) Buzogány László, 2002



About