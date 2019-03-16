      
Szemaforok (semaphores)
Olyan jelek, amelyek megmutatják, hogy egy folyamat végrehajthat-e egy programrészt vagy sem...
 

  Definíció

  Műveletek szemaforokkal

  Létrehozás (semget)

  Szemafor adatainak lekérdezése, módosítása és törlése (semctl)

  Szemafor értékének növelése és csökkentése (semop)

  Példák

 

 

  Üzenetsorok (message queues)

  Definíció

A processzor maximális kihasználása érdekében az operációs rendszerek jelentős része – így a Unix is – engedélyezi a folyamatoknak a párhuzamos (egyidőben történő) futást. Ez a megoldás igen hatékony, de mi történik akkor, ha két folyamat egyidőben próbálja elérni ugyanazt az erőforrást? Ilyen "érzékeny" erőforrások például a nyomtató vagy a memória, amelyet egyszerre csak egy folyamat használhat, különben a műveletnek előre nem látható következményei lehetnek.

Kritikus szakasznak nevezzünk tehát egy olyan módosítást a rendszeren, amelyet nem szabad megszakítani. A kritikus szakasz erőforráshoz kötött (például a nyomtatóhoz).

A szemaforok lehetővé teszik a felhasználók számára a folyamatok szinkronizálását. Általában a szemafor egy egész változó, amelyhez hozzárendelünk egy e0(v) kezdeti értéket, egy e(v) aktuális értéket és egy e0(v) várakozási sort.

Az alábbiakban bemutatjuk a legismertebb szinkronizációs P és V eljárásokat.

P(v)
{
  e(v) = e(v) - 1;
  if (e(v) < 0)
  {
    folyamat_allapota = WAIT;
    f(v) sorba <- folyamat;
  }
}

V(v)
{
  e(v) = e(v) + 1;
  if (e(v) <= 0)
  {
    folyamat <- f(v) sorból;
    kivalasztott_folyamat_allapota = READY;
  }
}

A szemafor aktuális állapotát a következő képlet adja:

e(v) = e0(v) + n·V(v) – n·P(v)

ahol:

n·V(v)  –  a v szemaforon végrehajtott V eljárások száma,
n·P(v)  –  a v szemaforon végrehajtott P eljárások száma.

Tehát egy kritikus szakasz megvalósítása a következőképpen történik: a szemafor kezdeti értéke 1 lesz, a kritikus szakaszt pedig a P és V eljárások határolják körül.

P(v)
kritikus szakasz
V(v)
folyamat többi része

Megjegyezzük, hogy a P és V eljárás és áltatában a szinkronizáló eljárások feloszthatatlanok (atomi műveletek).

A UNIX System V verzióban a szemafor fogalmát általánosították. Ezért egy szemafor esetében egyidöben akár több műveletet is megadhatunk (a P és V eljárások más-más időben hívódnak meg a folyamaton belül), és az e(v) értékének növelése vagy csökkentése nem feltétlenül 1-el kell történjen. Minden a rendszer által végrehajtott művelet feloszthatatlan. Tehát ha a rendszer nem tudja elvégezni a folyamat által kért összes műveletet, nem végzi el egyiket sem, és a folyamat várakozási állapotba kerül egészen az összes művelet végrehajtásáig.

Egy folyamat létrehozhat egy egész szemaforköteget. A kötegnek van egy bemenete a szemafortáblában, amely a szemaforköteg fejlécét tartalmazza. A folyamathoz rendelt táblázat pontosan annyi elemből áll, amennyi szemafor tartozik a köteghez. Minden elem megőrzi az illető szemaforhoz rendelt értéket.

A szemaforok kezelése nagyon hasonlít az osztott memória és az üzenetsorok kezeléséhez. Ezért egy folyamatnak csak akkor van joga hozzáférni egy szemaforhoz, ha ismeri a hozzárendelt kulcsot. Belsőleg a szemaforköteget egy egész számmal azonosítjuk, ezért a folyamat bármelyik szemaforhoz hozzáférhet 

Tehát minden szemaforkötegnek van egy azonosítója (amely egy pozitív egész szám) és egy semid_ds típusú adatszerkezete.

struct semid_ds
{
  struct ipc_perm sem_perm;  /* definiálja a jogokat és a tulajdonost */
  struct sem *sem_base;      /* pointer az első szemaforra a kötegből */
  int sem_nsens;             /* a kötegben található szemaforok száma */
  time_t sem_otime;          /* utolsó semop() művelet ideje */
  time_t sem_ctime;          /* utolsó struktúramódosítás ideje */
};

Egy folyamat csak akkor férhet hozzá egy szemaforköteghez ha:

- a folyamat a superuser-é,

- a felhasználó ID-je (uid) megegyezik az sem_perm.cuid-vel vagy az sem_perm.uid-vel és az sem_perm.mode tartalmazza a kívánt jogokat,

- a felhasználó csoportazonosítója (gid) megegyezik az sem_perm.cgid vagy sem_perm.gid értékek egyikével és az sem_perm.mode tartalmazza a kívánt jogokat,

- a felhasználó beleesik a "többi felhsználó" kategóriába és az sem_perm.mode tartalmazza a megfelelő jogokat.

Egy szemaforhoz rendelt adatszerkezet a következő:

struct sem
{
  ushort semval;    /* a szemafor értéke (semval>=0) */
  pid_t sempid;     /* utolsó, műveletet végző folyamat ID-ja */
  ushort semncnt;   /* azon foly. száma, amelyek a semval növekedését várják */
  ushort semzcnt;   /* azon foly. száma, amelyek a semval=0-t várják */
};

  Műveletek szemaforokkal

  Létrehozás (semget)

A semget rendszerfüggvény lehetővé teszi egy szemaforköteg ID-jának a meghatározását. Ha a szemaforköteg előzőleg nem létezett, a függvény létrehozza azt. Függetlenül attó, hogy a köteg létezett-e vagy sem a folyamatnak ismernie kell a szemaforhoz rendelt kulcsot. A függvény alakja:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semget(key_t key, int nrsem, int flg);

A függvény visszatérési értéke az újonnan létrehozott vagy egy régi szemafor azonosítója, illetve -1 hiba esetén. A key változóban a szemaforhoz rendelt kulcsot kell megadni, míg az flg-ben a létrehozási tevékenységet és a hozzáférési jogokat. A kulcsot a szemafort használó összes folyamatnak ismernie kell. Az nrsem argumentum a kötegben található szemaforok számát jelenti. Ha egy új köteget hozunk létre meg kell adnunk a változó értékét, ha viszont egy már létező kötegre hivatkozunk az nrsem értéke 0 lesz.

Egy új szemaforköteg létrehozása esetén az flg mezőt a következő formában kell megadni:

IPC_CREAT  |  IPC_EXCL  |  hozzáférési_jogok

Ha az IPC_EXCL nincs beállítva, akkor ha már létezik egy előzőleg létrehozott köteg a megadott kulcsra, a program nem jelez hibát, hanem visszaadja a létező köteg ID-ját.

Példa egy szemaforköteg létrehozására:

#define KEY 2003

int semid;
semid = semget((key_t) KEY, 5, IPC_CREAT | 0666);

Ha csak a szemaforköteg azonosítójára vagyunk kíváncsiak, a semget hívásakor adjuk meg a kulcsot, a többi paraméternek pedig 0 értéket.

semid = semget((key_t) KEY, 0, 0);

Létrehozáskor a társított adatstruktúra (sem_perm) mezői a következő információkkal töltődnek fel:

- sem_perm.cuid, sem_perm.uid – a semget függvényt meghívó folyamathoz hozzárendelt felhasználó ID-ja,
- sem_perm.cgid, sem_perm.uid – a semget függvényt meghívó folyamathoz hozzárendelt felhasználócsoport ID-ja,
- sem_perm.mode – a semget függvény hívásakor megadott flg argumentum, amely a hozzáférési jogokat tartalmazza,
- sem_nsems – a semget függvény hívásakor megadott szemaforok száma,
- sem_ctime – az aktuális időt tartalmazza,
- sem_otime – értéke 0.

Előfordulhat, hogy egy szemaforköteghez nincs hozzárendelt kulcs. Ebben az esetben a köteg létrehozásakor a semget függvény key paramétereként az IPC_PRIVATE szimbolikus konstanst kell megadni.

  Szemafor adatainak lekérdezése, módosítása és törlése (semctl)

A semctl függvény a szemaforkötegek szintjén az információk lekérdezésére, módosítására és törlésére használható. Szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semctl(int semid, int nrsem, int cmd, union semun arg);

A függvény visszatérített értéke az összes GET parancs kimeneti értéke, kivételt képez a GETALL parancs. Minden más esetben a függvény értéke 0.

A semid paraméter a semget függvény által meghatározott szemaforköteg azonosítója, míg az nrsem annak a szemafornak a száma, amelyen végre szeretnénk hajtani a cmd műveletet.

A cmd argumentum tehát a kívánt műveletet határozza meg és a következő értékeket veheti fel:

- GETVAL – a függvény visszatéríti a semval értékét az nrsem által meghatározott szemaforban,
- SETVAL – az nrsem által meghatározott szemafor semval paraméterének értéke arg.val lesz,
- GETPID – a függvény visszatéríti a sempid értékét az nrsem által meghatározott szemaforban,
- GETNCNT – a függvény visszatéríti a semncnt értékét az nrsem által meghatározott szemaforban,
- GETZCNT – a függvény visszatéríti a semzcnt értékét az nrsem által meghatározott szemaforban,
- GETALL – minden szemafor semval értékét elhelyezi az arg.array tömbben,
- SETALL – minden szemafor semval értékét feltölti az arg.array tömbben megadott értékekkel,
- IPC_STAT – a semid_ds struktúra elemeit lementi az arg.buf tömbbe,
- IPC_SET – a sem_perm.uid, a sem_perm.gid és a sem_perm.mode mezők értékeit frissíti az arg.buf tömbben megadott értékekkel,
- IPC_RMID – a szemaforköteg törlése; a törlés azonnali, tehát minden ezt a szemafor használó folyamat egy EIDRM üzenetet kap, és hibával leáll.

Az arg variáns típusú változó, amely a cmd parancs által felhasznált argumentumokat tartalmazza:

union semun
{
  int val;                /* a SETVAL-hoz */
  struct semid_ds *buf;   /* az IPC_STAT-hoz és az IPC_SET-hez */
  ushort *array;          /* a GETALL-hoz és a SETALL-hoz */
}

  Szemafor értékének növelése és csökkentése (semop)

A semop függvény feladata egy szemaforköteg egy elemének növelése és csökkentése. Szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semop(int semid, struct sembuf op[], size_t nrop);

A függvény visszatérési értéke 0, ha a művelet sikeres volt, ellenkező esetben -1. A semid a semget által meghatározott szemaforköteg azonosítója.

Az op egy mutató nrop darab sembuf típusú struktúrához, ahol:

struct sembuf
{
  ushort sem_num;  /* a kötegben levő szemaforok száma */
  short sem_op;    /* végrehajtandó művelet */
  short sem_flg;   /* végrehajtási körülmények */
}

Tehát minden szemafor esetén a végrehajtandó műveletet a sem_op mező jelzi. Ha:

- sem_op < 0

a. Ha semvan >= |sem_op|, akkor semval = semval - |sem_op|. Ez biztosítja, hogy a szemafor által visszatérített érték >= 0.

b. Ha (semval < |sem_op|) & (sem_flg & IPC_NOWAIT) = true, akkor a függvény egy hibakódot térít vissza.

c. Ha semval < |sem_op| és az IPC_NOWAIT nincs megadva, akkor a folyamat leáll, amíg a következők közül valamely feltétel nem teljesül:

- semval >= |sem_op| (egy másik folyamat felszabadít pár erőforrást),
- a szemafor törlődik a rendszerből; a függvény ebben az esetben -1 értéket térít vissza és errno=ERMID,
- ha egy folyamat egy jel lekezelése után visszatér, a semncnt értéke a szemaforban csökken, és a függvény -1-et ad vissza (errno=EINTR).

- sem_op > 0

Ekkor semval = semval + sem_op.

- sem_op = 0

a. Ha a semval = 0, akkor a függvény azonnal befejeződik.

b. Ha a semval ≠ 0 és az IPC_NOWAIT be van állítva, akkor a függvény -1-et ad vissza és az errno=EAGAIN.

c. Ha a semval ≠ 0 és az IPC_NOWAIT nincs beállítva, akkor a folyamat leáll, ameddig a semval 0 nem lesz, vagy a szemaforköteg megsemmisül, vagy a folyamat egy jelet kap.

  Példák

A következőkben megnézzük, hogyan lehet implementálni a P és V eljárásokat:

pv.c

#include "hdr.h"

static void semcall(int semid, int op)
{
  struct sembuf pbuf;

  pbuf.sem_num = 0;
  pbuf.sem_op = op;
  pbuf.sem_flg = 0;

  if (semop(semid, &pbuf, 1) < 0)
    err_sys("semop hiba");
}

void P(int semid)
{
  semcall(semid, -1);
}

void V(int semid)
{
  semcall(semid, 1);
}

Azért, hogy a P és a V eljárások működését jobban megértsük tekintsük a következő példát.

Írjunk folyamatot, amely három gyereket hoz létre. Mindenik gyerek hozzá szeretne férni egy közös erőforráshoz. A kritikus szakasz 10 másodpercig tart. (A valóságban a kritikus szakasznak sokkal rövidebbnek kell lennie.) A szemafor biztosítja minden folyamat egyéni hozzáférését a kritikus szakaszban. A tesztprogram a következő:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "pv.c"                    /* a fent definiált P és V eljárások */

#define SEMPERM 0600               /* hozzáférési jogok */

void rut_sem(int semid);
int initsem(key_t semkey);

void main(void)
{
  key_t semkey = 0x200;            /* kulcs */
  int semid, i;
  semid = initsem(semkey);         /* szemafor létrehozása */

  for (i=0; i<3; i++)              /* 3 darab gyerekfolyamat létrehozása */
     if (fork() == 0)
      rut_sem(semid);              /* erőforrási kérelmek a gyerekfolyamatoktól */
}

void rut_sem(int semid)            /* szemaforok megvalósítása */
{
  pid_t pid;
  pid = getpid();                  /* gyerekfolyamat PID-je */

  P(semid);                        /* belépés a kritikus szakaszba */
  printf("a %d folyamat kritikus szakaszban van\n", pid);

  sleep(random()%5);               /* kritikus szakasz; várakozás */

  printf("a %d folyamat elhagyja a kritikus szakaszt\n", pid);
  V(semid);                        /* kilépés a kritikus szakaszból */

  exit(0);                         /* gyerekfolyamat vége */
}

int initsem(key_t semkey)
{
  int semid;                       /* szemafor létrehozása */

  semid = semget(semkey, 1, SEMPERM | IPC_CREAT);

  if (semctl(semid, 0, SETVAL, 1) < 0)
    err_sys("semctl hiba");        /* szemaforok száma = 1 */

  return semid;                    /* szemafor ID-jának visszatérítése */
}

A következő tesztesetek is bizonyítják, hogy egy bizonyos időpillanatban csak egyetlen folyamat lehet a kritikus szakaszban.

$ a.out
a 790 folyamat kritikus szakaszban van
a 790 folyamat elhagyja a kritikus szakaszt
a 791 folyamat kritikus szakaszban van
a 791 folyamat elhagyja a kritikus szakaszt
a 792 folyamat kritikus szakaszban van
a 792 folyamat elhagyja a kritikus szakaszt

  Osztott memória (shared memory)

 

Copyright (C) Buzogány László, 2002



About