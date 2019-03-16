#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void kezelo(int signum)
{
  puts("signal kezel");
}
int main()
{
   signal(SIGINT,kezelo);
   puts("SIGINT kezel");
   sleep(5);
   signal(SIGINT,SIG_IGN);
   puts("SIGINT ignore");
   sleep(5);
   signal (SIGINT, SIG_DFL);
   puts("SIGINT default");
   sleep(5);
   return 0;
}
                                                    