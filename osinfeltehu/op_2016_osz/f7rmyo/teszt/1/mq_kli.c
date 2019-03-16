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

void main(int argc, char **argv)
{
  pid_t pid;                                  /* folyamatazonos�t� */
  
  MESSAGE mesp;                              
  /* �zenet */

  if (argc != 2)                              /* hib�s argumentumok */
    err_quit("hasznalat: c <cop_op>");

  if ((msgid = msgget((key_t) KEY, 0666)) < 0)
    err_sys("msgget hiba");                   /* �zenetsor ID-j�nek lek�rdez�se */

  mesp.mtip = 1;                              /* a k�r�s t�pusa mindig 1 */
  mesp.cod_op = atoi(argv[1]);                /* tev�kenys�g k�dja parancssorb�l */
  pid = mesp.pid = getpid();
  printf("a %d kliens elkuldte a kerest\n", mesp.pid);

  if (msgsnd(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), 0) < 0)
    err_quit("msgsnd hiba");                  /* tev.k�d elk�ld�se a szervernek */

  if (msgrcv(msgid, (struct msgbuf *)&mesp, sizeof(mesp)-sizeof(long), pid, 0) < 0)
    err_sys("msgrcv hiba");                   /* tev.n�v fogad�sa a szervert�l */

  mesp.mtext[12] = '\0';                      /* eredm�ny el�k�sz�t�se, ki�rat�s */

  printf("a %d szerver a %d kliensnek %s kerest kuldott\n", mesp.pid, pid, mesp.mtext);
}