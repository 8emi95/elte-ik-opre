#include <stdio.h>

void increment(int * a) {
  *a = *a + 115;
}

int main() {
  char str[] = "sajtosszendvics\n";
  
  printf("asd %s f\n", str);

  int i = 115;

  increment(&i);

  printf("%i", i);
}
