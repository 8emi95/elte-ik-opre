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
  signal(SIGFPE,kezelo);	//figyeli a lebegõpontos tulcsordulást, ha hiba van akkor kezelo végrehajtódik, hibafigyelést állít be
  sigsetjmp(a,SIGFPE);	//ha hiba következett be akkor ugorjunk a kezelõre, elmenti az aktuális állapotot
  //printf("%d"a,"******");
  printf("1/x = %d\n",1/x);
  return 0;
}
                                                  