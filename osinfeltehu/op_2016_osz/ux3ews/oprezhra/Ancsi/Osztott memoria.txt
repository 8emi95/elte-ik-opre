      
Osztott memória (shared memory)
Alkalmazása esetén ugyanazt a memóriarészt használja az összes összeköttetésben levő folyamat...
 

  Definíció

  Műveletek az osztott memóriával

  Létrehozás (shmget)

  Az osztott memória adatainak lekérdezése, módosítása és törlése (shmctl)

  Memóriarész hozzárendelése (shmat)

  Memóriarész hozzárendelésének megszüntetése (shmdt)

  Példa

 

 

  Szemaforok (semaphores)

  Definíció

Az osztott vagy közös memória segítségével megoldható, hogy két vagy több folyamat ugyanazt a memóriarészt használja. Az osztott memóriazónák általi kommunikáció elvei:

- Egy folyamat létrehoz egy közös memóriazónát. A folyamat azonosítója bekerül a memóriazónához rendelt struktúrába.

- A létrehozó folyamat hozzárendel az osztott memóriához egy numerikus kulcsot, amelyet minden ezt a memóriarészt használni kívánó folyamatnak ismernie kell. Ezt a memóriazónát az shmid változó azonosítja.

- A létrehozó folyamat leszögezi a többi folyamat hozzáférési jogait az illető zónához. Azért, hogy egy folyamat (beleértve a létrehozó folyamatot is) írni és olvasni tudjon a közös memóriarészből, hozzá kell rendelnie egy virtuális címterületet.

Ez a kommunikáció a leggyorsabb, hiszen az adatokat nem kell mozgatni a kliens és a szerver között.

A folyamatoknak a közös memóriarészhez való hozzáférését nem a rendszer felügyeli, a konfliktusok kezelése a felhasználó folyamatok feladata. Ha a szerver adatokat helyez el a közös memóriarészben, akkor a kliensnek várakoznia kell egészen a művelet befejezéséig, s csak akkor férhet hozzá az illető adatokhoz. A hozzáférések összehangolására gyakran használunk szemaforokat.

A rendszer minden egyes közös memóriarész esetén a következő adatokat tárolja:

struct shmid_ds
{
  struct ipc_perm shm_perm;  /* definiálja a jogokat és a tulajdonost */
  struct anon_map *shm_amp;  /* pointer a rendszerben */
  int shm_segz;              /* szegmens mérete bájtokban */
  pid_t shm_cpid;            /* létrehozó folyamat pid-je */
  pid_t shm_lpid;            /* utolsó shmop() pid-je */
  ulong shm_nattach;         /* eddig kapcsolódót folyamatok száma */
  ulong shm_cnattch;         /* csak az shminfo használja */
  time_t shm_atime;          /* utolsó beírás ideje */
  time_t shm_dtime;          /* utolsó kiolvasás ideje */
  time_t shm_ctime;          /* utolsó struktúramódosítás ideje */
}

Egy folyamat csak akkor férhet hozzá a közös memóriarészhez ha:

- a folyamat a superuser-é,

- a felhasználó ID-je (uid) megegyezik az shm_perm.cuid-vel vagy az shm_perm.uid-vel és az shm_perm.mode tartalmazza a kívánt jogokat,

- a felhasználó csoportazonosítója (gid) megegyezik az shm_perm.cgid vagy shm_perm.gid értékek egyikével és az shm_perm.mode tartalmazza a kívánt jogokat,

- a felhasználó beleesik a "többi felhsználó" kategóriába és az shm_perm.mode tartalmazza a megfelelő jogokat.

  Műveletek az osztott memóriával

  Létrehozás (shmget)

Az shmget rendszerfüggvény engedélyezi a közös memória azonosítójának lekérdezését felhasználva egy bizonyos kulcsot. Alakja:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int shmget(key_t key, int size, int flg);

A függvény visszatérési értéke a key kulcshoz rendelt memóriazóna azonosítója, illetve -1 hiba esetén. A size változóban a közös memória méretét kell megadni, míg az flg-ben a létrehozási tevékenységet és a hozzáférési jogokat. A kulcsot a közös memóriát használó összes folyamatnak kell ismernie.

Egy új memóriaterület létrehozása esetén az flg mezőt a következő formában kell megadni:

IPC_CREAT  |  IPC_EXCL  |  hozzáférési_jogok

Ha az IPC_CREAT opció nincs beállítva, és már létezik egy előzőleg létrehozott memóriazóna, a függvény ennek az azonosítóját téríti vissza.

Példaként hozzunk létre egy 2003 kulccsal rendelkező 200 bájt méretű memóriarészt.

#define KEY 2003

int shmid;
shmid = shmget((key_t) KEY, 200, IPC_CREAT | 0666);

Ha egy már létező osztott memóriarész azonosítóját (ID-ját) szeretnénk meghatározni, akkor a size és az flg mezőbe 0-t írunk – ez esetben a függvény nem fog létrehozni új sort. Például az előzőleg létrehozott memóriazóna esetén a következőképpen járunk el:

shmid = shmget((key_t) KEY, 0, 0);

Létrehozáskor a társított adatstruktúra (shm_perm) mezői a következő információkkal töltődnek fel:

- shm_perm.cuid, shm_perm.uid – az shmget függvényt meghívó folyamathoz hozzárendelt felhasználó ID-ja,
- shm_perm.cgid, shm_perm.uid – az shmget függvényt meghívó folyamathoz hozzárendelt felhasználócsoport ID-ja,
- shm_perm.mode – az shmget függvény hívásakor megadott flg argumentum, amely a hozzáférési jogokat tartalmazza,
- shm_qnum, shm_lspid, shm_lrpid, shm_stime, shm_rtime – értéke 0,
- shm_ctime – az aktuális időt tartalmazza,
- shm_segz – az shmget függvény hívásakor megadott size argumentum.

Előfordulhat, hogy egy osztott memóriazónának nincs hozzárendelt kulcsa. Ebben az esetben a key paraméternek az IPC_PRIVATE értéket adjuk.

  Az osztott memória adatainak lekérdezése, módosítása és törlése (shmctl)

Az shmctl függvény az osztott memóriarész információinak lekérdezésére, módosítására és törlésére használható. Szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int shmctl(int shmid, int cmd, struct shmid_ds *buf);

A függvény visszatérési értéke 0, ha a művelet sikeres volt, ellenkező esetben -1. Az shmid paraméter az shmget függvény által meghatározott osztott memóriarész azonosítója.

A cmd argumentum a kívánt műveletet határozza meg és a következő értékeket veheti fel:

- IPC_STAT – az osztott memóriához rendelt struktúra tartalma a buf változóba kerül,
- IPC_SET – az osztott memóriához rendelt struktúrát frissíti a buf által megadott struktúrával,
- IPC_RMID – az osztott memóriarész elméletileg törlődik; a tulajdonképpeni törlésre csak akkor kerül sor, amikor az utolsó folyamat is, amely ezt a zónát használja, megszakítja a kapcsolatát ezzel a memóriarésszel; függetlenül attól, hogy ez a rész éppen használat alatt van-e vagy sem az ID törlődik, s ezáltal ez a memóriarész többet nem osztható ki egyetlen folyamat számára sem; ebben az esetbe a buf argumentumnak a NULL értéket kell adni,
- SHM_LOCK – megtiltja a hozzáférést a közös memóriarészhez,
- SHM_UNLOCK – engedélyezi a hozzáférést a közös memóriarészhez.

Példa:

#define KEY 2003

shmid = shmget((key_t) KEY, 0, 0);
shmctl(shmid, IPC_RMID, NULL);

  Memóriarész hozzárendelése (shmat)

Az shmat függvény feladata egy folyamat címterületéhez hozzárendelni egy osztott memóriazónát. A hozzárendelés után a folyamat írhat, illetve olvashat erről a memóriarészről. A függvény szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void *shmat(int shmid, void *addr, int flg);

A függvény visszatérített értéke egy pointer a közös memóriazónára, ha a művelet sikeres volt, ellenkező esetben -1. Az shmid paraméter az shmget által meghatározott osztott memória azonosítója.

A addr pointer típusú változó a közös memóriarész hozzáférési címe a hívó folyamat adatszegmensében. Ezért ha:

- ha az addr ≠ NULL, a következő esetek fordulnak elő:

a. ha az SHM_RND opció be van állítva, a hozzárendelés az addr címhez történik,

b. ha az SHM_RND nincs beállítva, a hozzárendelés az (addr - (addr mod SHMLBA)) címhez történik,

- ha az addr = NULL, a memóriarész a rendszer által kiválasztott első szabad címhez történik (ajánlott).

Az flg paraméter meghatározza a hozzárendelt memória megadási módját (SHM_RND) és a közös részhez való hozzáférést, tehát hogy írásvédett (SHM_RDONLY) vagy sem.

A következő példa bemutatja, hogyan lehet írni egy közös memóriazónára:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define KEY 2003

void main(void)
{
  int shmid;
  char *p;
  ...
  shmid = shmget((key_t) KEY, 0, 0);
  p = shmat(shmid, NULL, 0);
  strcpy(p, "proba");
  ...
  exit(0);
}

  Memóriarész hozzárendelésének megszüntetése (shmdt)

Az shmdt függvény feladata a hívó folyamat címterületéhez hozzárendelt osztott memóriazóna felszabadása. Megjegyezzük, hogy a memóriarészhez hozzárendelt struktúra és az ID nem törlődik a rendszerből, míg egy folyamat (általában a szerver) az shmctl függvényhívással (IPC_RMID) azt végérvényesen nem törli. Alakja:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int shmdt(void *addr);

A következő programrészlet bemutatja, hogyan lehet kiolvasni adatokat egy olyan közös memóriazónából, ahová előzőleg egy másik folyamat írt. A végén a memóriaterületet felszabadítjuk és töröljük.

#include <stdio.h>
#include <sys/type.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY 2003

void main(void)
{
  int shmid;
  char *p;
  ...
  shmid = shmget((key_t) KEY, 0, 0);
  p = shmat(shmid, NULL, 0);
  printf("a kozos memoria tartalma: %s\n", p);
  shmdt(p);
  shmctl(shmid, IPC_RMID, NULL);
  exit(0);
}

  Példa

Készítsünk olyan programot, amely létrehozza, olvassa, írja és törli az osztott memóriát! A műveletet a parancssoron keresztül fogjuk megadni. Amennyiben egy művelet kiadásakor a közös memória nem létezik a program automatikusan hozza létre azt!

A folyamat tehát a következő műveleteket tudja elvégezni:

- írás a memóriazónába: shmtool w "text"
- a memóriazónán található szöveg kiolvasása: shmtool r
- a hozzáférési jogok módosítása (mode): shmtool m (mode)
- memóriarész törlése: shmtool r

A forráskód tartalmazza a főprogramot és a műveleteket elvégző segédeljárásokat. (Az ftok utasítással a folyamat számára egyedi kulcsot hozunk létre.)

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SEGSIZE 100                                 /* a tárolandó szöveg max. mérete */

void writeshm(int shmid, char *segptr, char *text);
void readshm(int shmid, char *segptr);
void removeshm(int shmid);                          /* függvények deklarálása */
void changemode(int shmid, char *mode);
void usage(void);

int main(int argc, char *argv[])                    /* parancssorból a paraméterek */
{
  key_t key;                                        /* kulcs */
  int shmid;                                        /* osztott memória ID-ja */
  char *segptr;                                     /* osztott memória címe */

  if (argc == 1)
    usage();                                        /* hiányos paraméterlista */

  key = ftok(".", 'S');                             /* egyedi kulcs létrehozása */

                                                    /* megnyitás, szükség esetén létrehozás */

  if ((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0666)) == -1)
  {
    printf("az osztott memoriazona letezik - megnyitom\n");

    if ((shmid = shmget(key, SEGSIZE, 0)) == -1)
    {
      perror("shmget hiba");
      exit(1);
    }
  }
  else
    printf("letrehozok egy uj osztott memoriazonat\n");

                                                    /* memóriacím hozzárendelése */

  if ((segptr = shmat(shmid, 0, 0)) == (void *) -1)
  {
    perror("shmat hiba");
    exit(1);
  }

  switch (tolower(argv[1][0]))                      /* a param. alapján elvégzi a műv. */
  {
    case 'w':                                       /* megadott szöveg írása */
      writeshm(shmid, segptr, argv[2]);
      break;
    case 'r':                                       /* osztott memóriazóna kiolvasása */
      readshm(shmid, segptr);
      break;
    case 'd':                                       /* törlés */
      removeshm(shmid);
      break;
    case 'm':                                       /* jogok módosítása */
      changemode(shmid, argv[2]);
      break;
    default:                                        /* hibás opció */
      usage();
  }
}

void writeshm(int shmid, char *segptr, char *text)
{
  strcpy(segptr, text);                             /* szöveg beírása a memóriába */
  printf("kesz...\n");
}

void readshm(int shmid, char *segptr)
{
  printf("segptr: %s\n", segptr);                   /* mem. tartalmának kiolvasása */
}

void removeshm(int shmid)
{
  shmctl(shmid, IPC_RMID, 0);                       /* osztott memória törlése */
  printf("torolve\n");
}

void changemode(int shmid, char *mode)
{
  struct shmid_ds myshmds;
  shmctl(shmid, IPC_STAT, &myshmds);               /* aktuális jogok lekérdezése */
  printf("a regi jogok: %o\n", myshmds.shm_perm.mode);
  sscanf(mode, "%o", &myshmds.shm_perm.mode);
  shmctl(shmid, IPC_SET, &myshmds);                /* új jogok beállítása */
  printf("az uj jogok: %o\n", myshmds.shm_perm.mode);
}

void usage(void)                                   /* használati útmutató */
{
  printf("shmtool - osztott memoria menedzselo rendszer\n\n");
  printf("HASZNALAT: shmtool (w)rite <szoveg>\n");
  printf("                   (r)ead\n");
  printf("                   (d)elete\n");
  printf("                   (m)ode change <oktalis_mod>\n");
  exit(1);
}

A fenti példa tesztelésekor a következő eredményre jutottunk:

$ shmtool w teszt
letrehozok egy uj osztott memoriazonat
kesz...

$ shmtool r
az osztott memoriazona letezik - megnyitom
segptr: teszt

$ shmtool w szasz
letrehozok egy uj osztott memoriazonat
kesz...

$ shmtool r
az osztott memoriazona letezik - megnyitom
segptr: szasz

$ shmtool d
az osztott memoriazona letezik - megnyitom
torolve

$ shmtool m 660
letrehozok egy uj osztott memoriazonat
a regi jogok: 666
az uj jogok: 660

$

 

Copyright (C) Buzogány László, 2002



About