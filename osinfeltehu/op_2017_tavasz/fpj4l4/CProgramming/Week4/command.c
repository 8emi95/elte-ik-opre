#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main() {
  char command[100];
  char exitor[] = "exit sys";
  
  while (strcmp(command,exitor) != 0) {
    printf("Give a command \n");
    scanf("%s", command);  
  }

  return 0;
}