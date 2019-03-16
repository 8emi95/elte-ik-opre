#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  pid_t fid; // a fork eredmenye
  int status;
  fid=fork(); // child -1 lesz ha sikertelen a fork hivas
  if (fid>0)  //szulo
  {
    printf("Szulo vagyok, varakozas indul..., az azonositom: %i\n",getpid());
    waitpid(fid,&status,WUNTRACED|WCONTINUED|WNOHANG); //WNOHANG volt, igy nem var
    printf("Megvartam a gyereket.\n");
  }
  else  // gyerek ag
  {
    printf("A gyerek vagyok, azonositoja: %i\n",getpid());
    sleep(5);
    printf("A gyereknek vege! A szulom azonositoja: %i\n",getppid());
  }
  printf("Vege%i\n",getpid()); // ezt szulo es gyerek is vegrehajtja
  return 0;
}
