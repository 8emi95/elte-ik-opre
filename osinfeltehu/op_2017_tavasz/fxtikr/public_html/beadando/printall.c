#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "printall.h"

FILE* f;

void printall(const char* filename){   
   f = fopen(filename, "r");
   
   if( f == NULL ) { perror("Hiba a fájl megnyitásakor\n"); exit(1); }
   
   int i = 1;
   char line[500];
   while(!feof(f)){
    if (fgets(line, sizeof(line), f) != NULL ){  
      printf("%i. Kérdés: ", i);
      printf("%s", line);
      
      fgets(line, sizeof(line), f);
      int n = atoi(line);
      int j;
      for(j = 0; j < n; j++){
         fgets(line, sizeof(line), f);
         printf("\t\t%i) %s", j+1, line);
      }
     }
     fgets(line, sizeof(line), f);
     if(feof(f)) break;
     printf("\nKérdés hozzáadva: %s\n", line);
     i++;
   }

   fclose(f);
}
