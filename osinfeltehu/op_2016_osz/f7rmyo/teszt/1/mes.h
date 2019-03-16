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