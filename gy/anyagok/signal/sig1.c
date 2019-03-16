
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//char tmpfilename [L_tmpnam];

void terminate (int param)
{
  printf ("Terminating program...\n");
  //remove (tmpfilename);
  exit(1);
}
      
int main ()
{
  void (*prev_fn)(int);
  printf("*********..\n");
  prev_fn = signal (SIGTERM,terminate);
  printf("++++++%d\n",prev_fn);
  printf("------%d\n",SIGABRT);
  //if (prev_fn==SIG_IGN) signal (SIGTERM,SIG_IGN);
  //tmpnam (tmpfilename);
  /* code here */
  return 0;
}