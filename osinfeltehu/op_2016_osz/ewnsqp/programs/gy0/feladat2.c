#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* spaceDel(char* str) {
  while (*str == ' ') {
      str++;
        }
          return str;
          }
          
          int main(int argc, char** argv) {
              char* value = spaceDel(argv[1]);
                printf("%s\n", value);
                  return 0;
                  }
                  