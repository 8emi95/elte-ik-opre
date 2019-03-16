#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ever (;;)  //19.sorban

static void sig_usr1(int);         /* generat cu kill -USR1 <pid> */
static void sig_intr(int);         /* generat la Ctrl-C si rearmat */
static void sig_quit(int);         /* generat cu Ctrl-\ si resetat */
static void sig_alarm(int);        /* generat dupa scurgerea timpului t din alarm(t) */

int main(void)
{
  if (signal(SIGALRM, sig_alarm) == SIG_ERR)
    printf("hiba: signal(SIGALRM, ...)");
  if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
    printf("hiba: signal(SIGUSR1, ...)");
  if (signal(SIGINT, sig_intr) == SIG_ERR)
    printf("hiba: signal(SIGINT, ...)");
  if (signal(SIGQUIT, sig_quit) == SIG_ERR)
    printf("hiba: signal(SIGQUIT, ...)");
  for ever pause();
}

static void sig_alarm(int sig)
{
  printf("SIGALRM jelet vettem...\n");
  return;
}

static void sig_quit(int sig)
{
  printf("SIGQUIT jelet vettem...\n");
  if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    printf("nem lehet visszaallitani ezt a jelet...");
  return;
}

static void sig_intr(int sig)
{
  printf("SIGINT jelet vettem...\n");
  if (signal(SIGINT, sig_intr) == SIG_ERR)
    printf("nem lehet ujra betolteni...");
  return;
}

static void sig_usr1(int sig)
{
  printf("SIGUSR1 jelet vettem...\n");
  alarm(1);
  printf("a riasztas 1 masodperc mulva elindul!\n");
  return;
}
