//#include "mes.h"                              /* a közös hdr állomány */
#include "hdr.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define KEY 13                                /* az üzenetsor kulcsa */

#define READ 1                                /* tevékenységkódok elnevezései */
#define WRITE 2

struct                                        /* az üzenet struktúrája */
{
  long mtip;                                  /* üzenet típusa */
  int pid;                                    /* küldõ folyamat azonosítója */
  int cod_op;                                 /* kliens üzenete; tevékenységkód */
  char mtext[13];                             /* szerver üzenete; tev. elnevezése */
} MESSAGE;

int msgid;                                    /* üzenetsor azonosítója */

void main(int argc, char **argv)
{
  pid_t pid;                                  /* folyamatazonosító */
  
  MESSAGE mesp;                              
  /* üzenet */

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
    err_sys("msgrcv hiba");                   /* tev.név fogadása a szervertõl */

  mesp.mtext[12] = '\0';                      /* eredmény elõkészítése, kiíratás */

  printf("a %d szerver a %d kliensnek %s kerest kuldott\n", mesp.pid, pid, mesp.mtext);
}