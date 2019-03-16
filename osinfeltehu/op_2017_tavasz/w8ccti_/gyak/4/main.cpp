#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t pids[10];

  printf("sajt: ");

  for(int i = 0; i < 10; i++) {
    pids[i] = fork();

    if(pids[i] == 0) {
      printf("child %d\n", i);
      return 0;
    }
  }

  int status;

  for(int i = 0; i < 10; i++) {
    waitpid(pids[i], &status, 0);
  }
}
