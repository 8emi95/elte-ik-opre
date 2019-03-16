#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber) {
  printf("sajt %d\n", signumber);
}

int main () {
  signal(SIGINT, handler);
  while(1) { sleep(10); }
}

