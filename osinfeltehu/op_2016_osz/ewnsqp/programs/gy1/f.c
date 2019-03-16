#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status = 0;
 // fork letrehoz egy gyerek folyamatot, az eredeti futo programban letrehozott valtozokat bemasolja, amjd a fork utan reszt lefuttatja
 pid_t  myPid = getpid();
 pid_t  child=fork(); //forks make a copy of variables
 fork();
 // gyereknek nincs pid-je, pontosabban = 0-val 
 // ha nem siekrult letrehozni a gyereket, akkor -1
 if (child == -1) { perror(".."); exit(1); }
 if (child > 0)
 {
  // bevarja a gyerek valaszat, sync (childPid: pid_t, &status: int, waitForIt:int)
  // utolso param: hogy bevarja-e tenyleg, vagy menjen tovabb a folyamat
  waitpid(child, &status, 0);	
  printf("szulo %i\n", getppid());
 }
 else
 {
  printf("gyerek %i\n", getppid());
 }
 
 //printf("Hello %i\n", getpid());
 return 0;
}

// ha a promptot visszakapjuk, miután még van gyerek, akkor baj van, hisz a foprogram elobb fejezodott be, mint a child
// fflush(stdout) -> OLYAN INSTANT IRJA KI A CUCCOT, HOGY BESZARSZ