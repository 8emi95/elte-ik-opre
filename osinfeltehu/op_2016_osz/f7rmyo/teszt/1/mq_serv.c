//#include "mes.h"                              /* a k�z�s hdr �llom�ny */
#include "hdr.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define KEY 13                                /* az �zenetsor kulcsa */

#define READ 1                                /* tev�kenys�gk�dok elnevez�sei */
#define WRITE 2

struct                                        /* az �zenet strukt�r�ja */
{
  long mtip;                                  /* �zenet t�pusa */
  int pid;                                    /* k�ld� folyamat azonos�t�ja */
  int cod_op;                                 /* kliens �zenete; tev�kenys�gk�d */
  char mtext[13];                             /* szerver �zenete; tev. elnevez�se */
} MESSAGE;
int msgid;                                    /* �zenetsor azonos�t�ja */

void do_it(MESSAGE *mesp)                     /* tev.k�d -> tev.elnevez�s */
{
  switch (mesp->cod_op)                       /* a kliens �ltal k�ld�tt k�d */
  {
    case READ:                                /* READ = 1 tev. elnevez�se */
      strcpy(mesp->mtext, "READ\n");
      break;
    case WRITE:                               /* WRITE = 2 tev. elnevez�se */
      strcpy(mesp->mtext, "WRITE\n");
      break;
    default:                                  /* ismeretlen tev.k�d */
      strcpy(mesp->mtext, "Ismeretlen\n");
      break;
  }
}

void main(void)
{
  MESSAGE mesp;                               /* �zenet */

  if ((msgid = msgget((key_t) KEY, IPC_CREAT)) < 0)
    err_sys("msgget hiba");                   /* �zenetsor l�trehoz�sa */

  for ever                                    /* v�gtelenciklus */
  {
    if (msgrcv(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), 1L, 0) < 0)
      err_sys("msgrcv hiba");                 /* tev.k�d kiolvas�sa a sorb�l */

    do_it(&mesp);                             /* �talak�t�s */

    mesp.mtip = mesp.pid;                     /* kliens k�dja = �zenet t�pusa */
    mesp.pid = getpid();

    if (msgsnd(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), 0) < 0)
      err_sys("msgsnd hiba");                 /* tev. elnevez�s k�ld�se */
  }
}
