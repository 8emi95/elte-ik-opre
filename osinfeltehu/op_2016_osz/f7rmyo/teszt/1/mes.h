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