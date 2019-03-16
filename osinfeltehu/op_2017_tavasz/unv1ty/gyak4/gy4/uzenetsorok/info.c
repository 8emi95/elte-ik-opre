Egy üzenet szerkezete az msg.h állományban a következőképpen van definiálva:

struct msgbuf
{
  long mtype;
  char mtext[1];
};

 Amennyiben ennél hosszabb üzenetet szeretnénk küldeni vdefiniálhatunk hasonló szerkezetű struktúrákat

struct msg
{
  long tip;
  char uzenet[256];
};

üzenetek szerkezete nem rögzített, de kötelezően tartalmazniuk kell a típust és magát az üzenetet. Ez utóbbi hossza mindig az alkalmazástól függ.
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

ipc_perm struktúra a következőképpen definiált:

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
-----------------------------
MŰVELETEK:

!!! Létrehozás (msgget):

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgget(key_t key, int flg);

Egy új üzenetsor létrehozása esetén az flg mezőt a következő formában kell megadni:

IPC_CREAT  |  IPC_EXCL  |  hozzáférési_jogok

Példaként hozzunk létre egy 2003 kulccsal rendelkező üzenetsort, és adjunk hozzá írási és olvasási jogot minden felhasználónak.

#define KEY 2003

int msgid;
msgid = msgget((key_t) KEY, IPC_CREAT | 0666);

Ha az üzenetsor már létezik, de meg szeretnénk határozni az azonosítóját (ID-ját), akkor az flg mezőbe 0-t írunk – ez esetben a függvény nem fog létrehozni új sort.

msgid = msgget((key_t) KEY, 0);
---------------------------------------

!!! Az üzenetsor adatainak lekérdezése, módosítása és törlése (msgctl):

Az msgctl függvény az üzenetsorok szintjén az információk lekérdezésére, módosítására és törlésére használható. Szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgctl(int msgid, int cmd, struct msqid_ds *buf);

A függvény visszatérési értéke 0, ha a művelet sikeres volt, ellenkező esetben -1. Az msgid paraméter az 
msgget függvény által meghatározott üzenetsor azonosítója.

A cmd argumentum a kívánt műveletet határozza meg és a következő értékeket veheti fel:

- IPC_STAT – az üzenetsorhoz rendelt struktúra tartalma a buf változóba kerül,
- IPC_SET – az üzenetsorhoz rendelt struktúrát frissíti a buf által megadott struktúrával,
- IPC_RMID – az üzenetsorhoz rendelt struktúrát törli; a művelet azonnali és minden folyamat, 
			amely ezt a sort használja az errno=EIDRM üzenetet kapja; ez esetbe a buf argumentumnak a NULL értéket kell adni.

Példa:

#define KEY 2003

msgid = msgget((key_t) KEY, 0);
msgctl(msgid, IPC_RMID, NULL);
---------------------------------------------------------
!!! ÜZENET KÜLDÉS (msgsnd):

Az msgsnd függvény egy már létrehozott üzenetsorba ír egy adott üzenetet. Szintaxisa:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgsnd(int msgid, const void *addr, size_t nbytes, int flg);

A következő példa egy üzenetnek a sorba való beírását mutatja be:

#define KEY 2003
struct msgbuf uzenet;
char *uzen = "probauzenet";

msgid = msgget((key_t) KEY, 0);
uzenet.mtype = 100;
strcpy(uzenet.mtext, uzen);
msgsnd(msgid, &uzenet, strlen(uzen), IPC_NOWAIT);  //NOWAIT helyett 0 (ignored) is lehet
-----------------------------------------------------------
!!! ÜZENET FOGADÁS(msgrcv):

Az msgrcv függvény feladata kiolvasni egy üzenetet az üzenetsorból. A paraméterek segítségével megadhatjuk, 
hogy milyen típusú üzeneteket fogadjon. Alakja:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgrcv(int msgid, void *addr, size_t nbytes, long msgtype, int flg);

sikeres olvasás esetén visszaadja a kiolvasott bájtok számát, különben -1.
Az msgid az msgget által meghatározott üzenetsor azonosítója. Az addr pointer a kapott üzenetre mutat.
Az nbytes a lekérdezett üzenet maximális hossza bájtokban.

Az msgtype paraméter határozza meg a kiolvasandó üzenet típusát. 3 eset lehetséges:
	a. msgtype = 0  –  a sor első üzenetét olvassa ki,
	b. msgtype > 0  –  a sor első msgtype típusú üzenetét olvassa ki,
	c. msgtype < 0  –  a sor legkisebb, de az abs(msgtype)-nál nagyobb vagy azzal egyenlő értékű üzenetét olvassa ki.

példa egy üzenet kiolvasására:

#define KEY 2003
struct msgbuf uzenet;

msgid = msgget((key_t) KEY, 0);
msgrcv(msgid, &uzenet, 25, long(100), IPC_NOWAIT | MSG_NOERROR);
-----------------------------------------------------------------------------
