      
Üzenetsorok (message queues)
Folyamatok közti kommunikáció üzenetek (kis adatcsomagok) küldözgetésével...
 

  Definíció

  Műveletek üzenetsorokkal

  Létrehozás (msgget)

  Az üzenetsor adatainak lekérdezése, módosítása és törlése (msgctl)

  Üzenet küldése (msgsnd)

  Üzenet fogadása (msgrcv)

  Példa

 

 

  Névvel ellátott csővezeték (FIFO)

  Definíció

A folyamatok közti kommunikáció egyik leghatékonyabb módja az üzenetsorok használata. Az üzenetsorokat a rendszer magja felügyeli. Egy ilyen sorban az adatok (vagyis az üzenetek) cirkulárisan közlekednek, a folyamatok közti szinkronizáció pedig a előállító/fogyasztó elv alapján valósul meg. Magyarul: ha az üzenetsor megtelt, az előállító leáll, ameddig egy fogyasztó ki nem olvas egy üzenetet, illetve ha az üzenetsor kiürült, a fogyasztónak kell várnia az első (neki címzett) üzenet érkezéséig.

Tehát az üzenetsor egy, a rendszer által tárol láncolt lista, amelynek jól meghatározott azonosítója van. Valamely folyamat egy üzenetsort egy kulcs segítségével azonosít. (Vigyázat! Nem tévesztendő össze az azonosító és a kulcs. A kulcsot mi választjuk, az azonosítót pedig a rendszer minden üzenetsorhoz automatikusan rendeli hozzá.)

Az a folyamat, amely üzenetet szeretne küldeni, előbb az adott kulcs alapján az msgget függvénnyel lekéri az üzenetsor azonosítóját, majd az msgsnd függvény segítségével elküldi az üzenetet. Az üzenet a sor végére kerül.

Az a folyamat, amely üzenetet szeretne fogadni, ugyanúgy lekérdezi a kulcs és az msgget függvény segítségével az üzenetsor azonosítóját, majd az msgrcv függvénnyel kiolvassa az üzenetet. Az üzenetsorból való olvasás nem feltétlenül a FIFO módszer szerint történik, ugyanis az mtype mező értékétől függően az üzeneteket tetszőleges sorrendben is kiolvashatjuk.

Egy üzenet szerkezete az msg.h állományban a következőképpen van definiálva:

struct msgbuf
{
  long mtype;
  char mtext[1];
};

Az mtype mező egy hosszú egész számot tartalmaz, amely az üzenet típusát jelképezi. Ezután következik a tulajdonképpeni üzenet (mtext), amely jelen esetben egyetlen karakterből áll. Amennyiben ennél hosszabb üzenetet szeretnénk egyszerre küldeni, mi is definiálhatunk hasonló szerkezetű struktúrákat. Például:

struct msg
{
  long tip;
  char uzenet[256];
};

Amint látjuk, az üzenetek szerkezete nem rögzített, de kötelezően tartalmazniuk kell a típust és magát az üzenetet. Ez utóbbi hossza mindig az alkalmazástól függ.

Minden üzenetsorhoz a rendszer hozzárendel egy msqid_ds típusú struktúrát, amelynek szerkezete:

struct msqid_ds
{
  struct ipc_perm msg_perm;  /* definiálja a jogokat és a tulajdonost */
  struct msg *msg_first;     /* mutató az első üzenetre */
  struct msg *msg_last;      /* mutató az utolsó üzenetre */
  ulong msg_cbytes;          /* sor bájtjainak száma */
  ulong msg_qnum;            /* üzenetek száma a sorban */
  ulong msg_qbytes;          /* maximális bájtok száma sorban */
  pid_t msg_lspid;           /* utolsó msgsnd() pid-je */
  pid_t msg_lrpid;           /* utolsó msgrcv() pid-je */
  time_t msg_stime;          /* utolsó msgsnd() ideje */
  time_t msg_rtime;          /* utolsó msgrcv() ideje */
  time_t msg_ctime;          /* utolsó struktúramódosítás ideje */
};

ahol az ipc_perm struktúra a következőképpen definiált:

struct ipc_perm
{
  uid_t uid;                 /* effektív felhasználó ID-je */
  gid_t gid;                 /* effektív csoport ID-je */
  uid_t cuid;                /* effektív létrehozó felhasználó ID-je */
  gid_t cgid;                /* effektív létrehozó csoport ID-je */
  mode_t mode;               /* hozzáférési jogok */
  ulong seq;                 /* numǎr de secvenţǎ utilizare slot */
  key_t key;                 /* kulcs */
};

Egy folyamat csak akkor férhet hozzá egy üzenetsorhoz ha:

- a folyamat a superuser-é,

- a felhasználó ID-je (uid) megegyezik az msg_perm.cuid-vel vagy az msg_perm.uid-vel és az msg_perm.mode tartalmazza a kívánt jogokat,

- a felhasználó csoportazonosítója (gid) megegyezik az msg_perm.cgid vagy msg_perm.gid értékek egyikével és az msg_perm.mode tartalmazza a kívánt jogokat,

- a felhasználó beleesik a "többi felhsználó" kategóriába és az msg_perm.mode tartalmazza a megfelelő jogokat.

  Műveletek üzenetsorokkal

  Létrehozás (msgget)

Az msgget rendszerfüggvény engedélyezi a folyamatnak, hogy létrehozzon egy üzenetsort felhasználva egy bizonyos kulcsot. Alakja:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgget(key_t key, int flg);

A függvény visszatérési értéke az újonnan létrehozott vagy egy régi üzenetsor azonosítója, ha a művelet sikerült, különben -1. A key változóban az üzenetsorhoz rendelt kulcsot kell megadni, míg az flg-ben a létrehozási tevékenységet és a hozzáférési jogokat. A kulcsot az üzenetsorhoz kapcsolódó összes folyamatnak ismernie kell.

Egy új üzenetsor létrehozása esetén az flg mezőt a következő formában kell megadni:

IPC_CREAT  |  IPC_EXCL  |  hozzáférési_jogok

Példaként hozzunk létre egy 2003 kulccsal rendelkező üzenetsort, és adjunk hozzá írási és olvasási jogot minden felhasználónak.

#define KEY 2003

int msgid;
msgid = msgget((key_t) KEY, IPC_CREAT | 0666);

Ha az üzenetsor már létezik, de meg szeretnénk határozni az azonosítóját (ID-ját), akkor az flg mezőbe 0-t írunk – ez esetben a függvény nem fog létrehozni új sort.

msgid = msgget((key_t) KEY, 0);

Létrehozáskor a társított adatstruktúra (msg_perm) mezői a következő információkkal töltődnek fel:

- msg_perm.cuid, msg_perm.uid – az msgget függvényt meghívó folyamathoz hozzárendelt felhasználó ID-ja,
- msg_perm.cgid, msg_perm.uid – az msgget függvényt meghívó folyamathoz hozzárendelt felhasználócsoport ID-ja,
- msg_perm.mode – az msgget függvény hívásakor megadott flg argumentum, amely a hozzáférési jogokat tartalmazza,
- msg_qnum, msg_lspid, msg_lrpid, msg_stime, msg_rtime – értéke 0,
- msg_ctime – az aktuális időt tartalmazza,
- msg_qbytes – azt a legnagyobb megengedett értéket tartalmazza, amely a rendszer létrehozásakor volt rögzítve.

Az msgget függvény hívásakor visszaadott azonosítót az üzenetsorral dolgozó függvények használják.

  Az üzenetsor adatainak lekérdezése, módosítása és törlése (msgctl)

Az msgctl függvény az üzenetsorok szintjén az információk lekérdezésére, módosítására és törlésére használható. Szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgctl(int msgid, int cmd, struct msqid_ds *buf);

A függvény visszatérési értéke 0, ha a művelet sikeres volt, ellenkező esetben -1. Az msgid paraméter az msgget függvény által meghatározott üzenetsor azonosítója.

A cmd argumentum a kívánt műveletet határozza meg és a következő értékeket veheti fel:

- IPC_STAT – az üzenetsorhoz rendelt struktúra tartalma a buf változóba kerül,
- IPC_SET – az üzenetsorhoz rendelt struktúrát frissíti a buf által megadott struktúrával,
- IPC_RMID – az üzenetsorhoz rendelt struktúrát törli; a művelet azonnali és minden folyamat, amely ezt a sort használja az errno=EIDRM üzenetet kapja; ez esetbe a buf argumentumnak a NULL értéket kell adni.

Példa:

#define KEY 2003

msgid = msgget((key_t) KEY, 0);
msgctl(msgid, IPC_RMID, NULL);

  Üzenet küldése (msgsnd)

Az msgsnd függvény egy már létrehozott üzenetsorba ír egy adott üzenetet. Szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgsnd(int msgid, const void *addr, size_t nbytes, int flg);

A függvény visszatérési értéke 0, ha a művelet sikeres volt, ellenkező esetben -1. Az msgid az msgget által meghatározott üzenetsor azonosítója. Az addr pointer típusú argumentum az üzenetre mutat. Ez lehet void vagy msgbuf típusú. Az előbbi esetben az üzenet szerkezete tetszőleges lehet. Az nbytes megadja az üzenet tartalmának a hosszát (nem az egész üzenetét!). Az flg paraméterben megadhatjuk, hogy a rendszer hogyan viselkedjen, ha az üzenetet nem lehet beírni a sorba. Például, ha a sor megtelt és az IPC_NOWAIT opciót választottuk, akkor a folyamat nem áll le, hanem visszatér a függvényből, és az errno az EAGAIN hibaüzenetet fogja tartalmazni.

A következő példa egy üzenetnek a sorba való beírását mutatja be:

#define KEY 2003
struct msgbuf uzenet;
char *uzen = "probauzenet";

msgid = msgget((key_t) KEY, 0);
uzenet.mtype = 100;
strcpy(uzenet.mtext, uzen);
msgsnd(msgid, &uzenet, strlen(uzen), IPC_NOWAIT);

  Üzenet fogadása (msgrcv)

Az msgrcv függvény feladata kiolvasni egy üzenetet az üzenetsorból. A paraméterek segítségével megadhatjuk, hogy milyen típusú üzeneteket fogadjon. Alakja:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgrcv(int msgid, void *addr, size_t nbytes, long msgtype, int flg);

A függvény sikeres olvasás esetén visszaadja a kiolvasott bájtok számát, különben a -1 értéket. Az msgid az msgget által meghatározott üzenetsor azonosítója. Az addr pointer a kapott üzenetre mutat.

Az nbytes a lekérdezett üzenet maximális hossza bájtokban. Az üzenet valós mérete különbözhet az nbytes-ban megadottól. Ha az üzenet hossza kisebb a megadott maximális méretnél, akkor minden OK. Ha viszont átlépi az nbytes határt, akkor két eset lehetséges:

a. Ha az MSG_NOERROR be van állítva, az üzenet megcsonkul, de nem kapunk hibaüzenetet.

b. Ha az MSG_NOERROR nincs beállítva, a függvény nem olvassa ki az üzenetet, a függvény hibát ad, és az errno változó értéke E2BIG lesz.

Az msgtype paraméter határozza meg a kiolvasandó üzenet típusát. 3 eset lehetséges:

a. msgtype = 0  –  a sor első üzenetét olvassa ki,

b. msgtype > 0  –  a sor első msgtype típusú üzenetét olvassa ki,

c. msgtype < 0  –  a sor legkisebb, de az abs(msgtype)-nál nagyobb vagy azzal egyenlő értékű üzenetét olvassa ki.

Az flg argumentummal megadhatjuk, hogy a rendszer hogyan viselkedjen, ha a függvény meghívásának feltételei nem teljesülnek. Az előbb bemutattuk az MSG_NOERROR használatát. Következzen az IPC_NOWAIT-é. Szintén két eset lehetséges:

a. Ha az IPC_NOWAIT nincs beállítva, a folyamat nem várja meg az óhajtott üzenetnek a sorban való elhelyezését. A függvény hibát ad, és az errno értéke ENOMSG lesz.

b. Ha az IPC_NOWAIT nincs beállítva, a folyamat addig várakozik, ameddig kerül egy megfelelő üzenet, ameddig a sor megsemmisül (errno=EIDRM), vagy egy jelzés érkezik (errno=EINTR).

A következő programrészlet példa egy üzenet kiolvasására:

#define KEY 2003
struct msgbuf uzenet;

msgid = msgget((key_t) KEY, 0);
msgrcv(msgid, &uzenet, 25, long(100), IPC_NOWAIT | MSG_NOERROR);

Ha az üzenetsorba bekerülő adatok típusai megegyeznek, akkor az üzenetsorok tulajdonképpen FIFO állományként működnek.

  Példa

Hozzunk létre egy kliens-szerver rendszert! A kliensfolyamat a parancssorból beolvas egy tevékenységkódot (pl. kliens 2), majd ezt egy üzenetsoron keresztül elküldi a szervernek. A visszakapott választ a képernyőre írja. A szerverfolyamat kiolvassa az üzenetsorból a tevékenységkódot, elvégzi a tevékenységet, majd jelentést küld a kliensnek.

Az egyszerűség kedvéért a közös adatokat (így az üzenet struktúráját is) egy külön fejléc állományban tároljuk, amelyet mind a kliens, mind a szerver el tud majd érni.

mes.h

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define KEY 13                                /* az üzenetsor kulcsa */

#define READ 1                                /* tevékenységkódok elnevezései */
#define WRITE 2

struct                                        /* az üzenet struktúrája */
{
  long mtip;                                  /* üzenet típusa */
  int pid;                                    /* küldő folyamat azonosítója */
  int cod_op;                                 /* kliens üzenete; tevékenységkód */
  char mtext[13];                             /* szerver üzenete; tev. elnevezése */
} MESSAGE;

A kliens első lépésben a parancssorból beolvas egy tevékenységkódot. Egyetlen üzenetsort használunk, amelyen keresztül minden kliens elküldheti a kéréseit a szerver felé, és amelyen a válasz is érkezik. Azért, hogy egy kliens megkapja a neki címzett választ, csak azon üzeneteket fogja kiolvasni a sorból, amelyek típusa éppen megegyezik a saját pid-jével. Éppen ezért a kéréssel egyidőben minden kliensnek el kell küldenie az azonosítóját is.

kliens.c

#include "mes.h"                              /* a közös hdr állomány */
#include "hdr.h"

int msgid;                                    /* üzenetsor azonosítója */

void main(int argc, char **argv)
{
  pid_t pid;                                  /* folyamatazonosító */
  MESSAGE mesp;                               /* üzenet */

  if (argc != 2)                              /* hibás argumentumok */
    err_quit("hasznalat: c <cop_op>");

  if ((msgid = msgget((key_t) KEY, 0666)) < 0)
    err_sys("msgget hiba");                   /* üzenetsor ID-jánek lekérdezése */

  mesp.mtip = 1;                              /* a kérés típusa mindig 1 */
  mesp.cod_op = atoi(argv[1]);                /* tevékenység kódja parancssorból */
  pid = mesp.pid = getpid();
  printf("a %d kliens elkuldte a kerest\n", mesp.pid);

  if (msgsnd(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), 0) < 0)
    err_quit("msgsnd hiba");                  /* tev.kód elküldése a szervernek */

  if (msgrcv(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), pid, 0) < 0)
    err_sys("msgrcv hiba");                   /* tev.név fogadása a szervertől */

  mesp.mtext[12] = '\0';                      /* eredmény előkészítése, kiíratás */

  printf("a %d szerver a %d kliensnek %s kerest kuldott\n", mesp.pid, pid, mesp.mtext);
}

A szerverfolyamat kap a klienstől egy 1-es típusú üzenetet, majd egy olyan választ készít, amelynek típusa megegyezik a kérést küldő kliens pid-jével. Az alábbi példában a szerver által felkínált szolgáltatások a következők: READ és WRITE. Ezek végrehajtását a szerver csak szimulálja. A választ az mtext mező fogja tartalmazni, amely megmutatja, hogy a szervernek sikerült-e azonosítania a tevékenységet vagy sem.

szerver.c

#include "mes.h"                              /* a közös hdr állomány */
#include "hdr.h"

int msgid;                                    /* üzenetsor azonosítója */

void do_it(MESSAGE *mesp)                     /* tev.kód -> tev.elnevezés */
{
  switch (mesp->cod_op)                       /* a kliens által küldött kód */
  {
    case READ:                                /* READ = 1 tev. elnevezése */
      strcpy(mesp->mtext, "READ\n");
      break;
    case WRITE:                               /* WRITE = 2 tev. elnevezése */
      strcpy(mesp->mtext, "WRITE\n");
      break;
    default:                                  /* ismeretlen tev.kód */
      strcpy(mesp->mtext, "Ismeretlen\n");
      break;
  }
}

void main(void)
{
  MESSAGE mesp;                               /* üzenet */

  if ((msgid = msgget((key_t) KEY, IPC_CREAT)) < 0)
    err_sys("msgget hiba");                   /* üzenetsor létrehozása */

  for ever                                    /* végtelenciklus */
  {
    if (msgrcv(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), 1L, 0) < 0)
      err_sys("msgrcv hiba");                 /* tev.kód kiolvasása a sorból */

    do_it(&mesp);                             /* átalakítás */

    mesp.mtip = mesp.pid;                     /* kliens kódja = üzenet típusa */
    mesp.pid = getpid();

    if (msgsnd(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), 0) < 0)
      err_sys("msgsnd hiba");                 /* tev. elnevezés küldése */
  }
}

A fenti példa egy tesztesetre a következőképpen működik:

$ szerver &
$ kliens 1 & kliens 4

a 995 kliens elkuldte a kerest
a 768 szerver a 995 kliensnek READ kerest kuldott

a 999 kliens elkuldte a kerest
a 768 szerver a 999 kliensnek ismeterlen kerest kuldott

  Szemaforok (semaphores)

 

Copyright (C) Buzogány László, 2002



About