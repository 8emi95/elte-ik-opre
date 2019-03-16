#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int nChildren;

void myFork(int n);

int main(int argc, char *argv[]) {

  // just a check on the number of arguments supplied
  if (argc < 2) {
    printf("Usage: forktest <number_of_children>\n");
    printf("Example: forktest 5 - spawns 5 children processes\n");
    return -1;
  }

  nChildren = atoi(argv[1]);
  // starting the recursion...
  myFork(nChildren-1);
  return 0;
}

// the recursive function
void myFork(int n) {
  pid_t pid;

  pid = fork();

  // the child does nothing but printing a message on screen
  if (pid == 0) {
    printf("I am a child: %d PID: %d\n", nChildren-n, getpid());
    return;
  }

  // if pid != 0, we're in the parent
  // let's print a message showing that the parent pid is always the same...
  printf("It's always me (PID %d) spawning a new child (PID %d)\n", getpid(), pid);
  // ...and wait for the child to terminate.
  wait(pid);

  // let's call ourself again, decreasing the counter, until it reaches 0.
  if (n > 0) {
    myFork(n-1);
  }
}