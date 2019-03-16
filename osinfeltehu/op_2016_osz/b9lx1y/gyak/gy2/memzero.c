#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Rec {
  int id;
  char name[40];
};

int main(int argc, char ** argv) {
  struct Rec record;

  memset(&record, 0x0, sizeof(record));

  return 0;
}
