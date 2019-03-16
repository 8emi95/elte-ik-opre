#include <stdio.h>
main()
{
   FILE *fp;
   char* name;

   fp = fopen("/tmp/test.txt", "w+");
   gets(name);
   fprintf(fp, name);
   fclose(fp);
}
