#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<setjmp.h>

int x;
sigjmp_buf a;

void kezelo(int signum)
{
    switch(signum)
    {
    case SIGFPE:
    x=1;
    siglongjmp(a,1); 
    break;
    default: puts("egy");
    }
}
int main()
{
  x=0;    
  signal(SIGFPE,kezelo);	//figyeli a lebeg�pontos tulcsordul�st, ha hiba van akkor kezelo v�grehajt�dik, hibafigyel�st �ll�t be
  sigsetjmp(a,SIGFPE);	//ha hiba k�vetkezett be akkor ugorjunk a kezel�re, elmenti az aktu�lis �llapotot
  //printf("%d"a,"******");
  printf("1/x = %d\n",1/x);
  return 0;
}
                                                  