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
