#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  printf("Sizeof: %d", sizeof(argv[1]));
  printf("Strlen: %d", strlen(argv[1]));
}
