#include <stdio.h>


int main(int argc, char** argv) {
 if (argc != 3) {
  printf("Usage: %s <text to print> <number of times>\n", argv[0]);
 }

 int times = atoi(argv[2]);
 int i;
 for (i = 0; i < times; ++i) {
  printf("%s\n", argv[1]);
 }
 
 return 0;
}
