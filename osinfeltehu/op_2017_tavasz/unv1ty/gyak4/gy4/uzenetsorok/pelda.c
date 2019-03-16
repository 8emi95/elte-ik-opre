/*Hozzunk létre egy kliens-szerver rendszert! A kliensfolyamat a parancssorból beolvas egy tevékenységkódot (pl. kliens 2),
majd ezt egy üzenetsoron keresztül elküldi a szervernek. A visszakapott választ a képernyőre írja.
A szerverfolyamat kiolvassa az üzenetsorból a tevékenységkódot, elvégzi a tevékenységet, majd jelentést küld a kliensnek.
Az egyszerűség kedvéért a közös adatokat (így az üzenet struktúráját is) egy külön fejléc állományban tároljuk,
amelyet mind a kliens, mind a szerver el tud majd érni. */

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
----------------------

/*A kliens első lépésben a parancssorból beolvas egy tevékenységkódot. Egyetlen üzenetsort használunk,
amelyen keresztül minden kliens elküldheti a kéréseit a szerver felé, és amelyen a válasz is érkezik.
Azért, hogy egy kliens megkapja a neki címzett választ, csak azon üzeneteket fogja kiolvasni a sorból, 
amelyek típusa éppen megegyezik a saját pid-jével.Éppen ezért a kéréssel egyidőben minden kliensnek el kell küldenie az azonosítóját is. */

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
------------------------------------------
/*A szerverfolyamat kap a klienstől egy 1-es típusú üzenetet, majd egy olyan választ készít, 
 * amelynek típusa megegyezik a kérést küldő kliens pid-jével. Az alábbi példában a szerver által felkínált
 *  szolgáltatások a következők: READ és WRITE. Ezek végrehajtását a szerver csak szimulálja. A választ az mtext
 *  mező fogja tartalmazni, amely megmutatja, hogy a szervernek sikerült-e azonosítania a tevékenységet vagy sem. */

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
