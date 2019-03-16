#include <stdio.h>
#include <signal.h>

int main() {
  printf("SIGRTMIN: %i, SIGRTMAX: %i\n", SIGRTMIN, SIGRTMAX);

  return 0;
}
