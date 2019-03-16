#include <stdio.h>      /* printf, fgets */
#include <stdlib.h> 	/* atoi */

int main(int argc, char** argv)
{
  char* text = argv[1];
  int count = atoi(argv[2]);
  
  for (; count != 0; --count) printf("%s\n",text);
  return 0;
}