!!! A fork függvény szintaxisa:

#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

A visszaadott pid_t típusú érték:

0 – a gyerekben,
a gyerek PID-je – a szülőben,
-1 – hiba esetén.
Egy gyerek a szülője azonosítóját a getppid függvénnyel kérdezheti le. A saját PID lekérdezésére a getpid függvény szolgál.

Példa:

#include <stdio.h>
#include <sys/types.h>

int main(void)
{
  pid_t pid;
  pid = fork();
  printf("%d folyamat, pid = %d\n", getpid(), pid);
}

A fenti program végrehajtásának eredménye:

$ a.out
226 folyamat, pid = 207
207 folyamat, pid = 0

hogy a két kódrészt (gyerek-szülő) jól elkülönítsük, a fork-ot a következőképpen szoktuk használni:

pid = fork();
if (pid == 0)
{
  /* gyerek folyamat */
}
else
{
  /* szülő folyamat */
}

Az alábbi példában a hibát is kezeljük:

switch (fork())
{
  case -1:
    perror("fork");
    exit(1);
  case 0:
    /* gyerek folyamat */
  default:
    /* szülő folyamat */
}
------------------------------------------------------------
!!! Folyamat befejezése (exit)

#include <stdlib.h>
void exit(int exit_code);

#include <unistd.h>			//ANSI-C
void _exit(int exit_code); //POSIX
-------------------------------------------------------------
!!! Várakozás egy folyamatra (wait, waitpid):

#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *status);	// felfüggeszti a hívó folyamatot, amíg a gyerek befejeződik
pid_t waitpid(pid_t pid, int *status, int opt); //opt = 0 általában

waitpid nem mindig az első fiú befejezéséig vár, hanem a pid változóban megadott azonosítójú gyerek befejezéséig.
---------------------------------------------------------------
Folyamat prioritásának változtatása (nice)

Egy folyamat prioritásának megváltoztatására szolgál a nice függvény. Alakja:

#include <unistd.h>
int nice(int inc);

A függvény hozzáadja az inc értéket a hívó folyamat nice értékéhez. (Egy folyamatnak minél nagyobb a nice értéke,
annál kisebb a prioritása.) Csak a superuser adhat meg negatív értékeket, s növelheti ezáltal egy folyamat futási sebességét.
A visszatérített érték 0, ha a művelet sikeres volt, és -1 különben.
Egy folyamat prioritását a rendszer a következő képlettel határozza meg:

folyamat_prioritás = alap_prioritás + elhasznált_processzoridő / konstans + nice_érték

A nice érték minden esetben egy -20 és 19 közötti egész szám.


