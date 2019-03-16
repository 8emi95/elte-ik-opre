#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "add.h"

FILE *f;

void lwrite(const char* string){
   f = fopen("kerdesek.txt", "a");
   fprintf(f, "%s", string);
   fclose(f);
}

void nwrite(const char* string){
   f = fopen("kerdesek.txt", "a");
   fprintf(f, "%s\n", string);
   fclose(f);
}

void addmenu(){
   int b = 1;
   int i = 0;
   time_t date;
   char tmp[129];
   
   printf("\n\n\n\n");
   printf("Kérem adja meg a kérdést (max 128 karakter): \n");

   while(b) {
   fgets(tmp, sizeof(tmp), stdin);
   if(strlen(tmp) < 129 && strlen(tmp) > 0) {
      int j;
      for(j = strlen(tmp); j < 127; j++) {
        tmp[j-1] = ' ';
      }
      b = 0;
	  tmp[128] = '\0';
     }
    }
   b = 1;
   nwrite(tmp);
   while(b){ 
      printf("Kérem adja meg a válaszok számát (2-4): \n"); 
      fgets(tmp, sizeof(tmp), stdin);
      if(atoi(tmp) > 1 && atoi(tmp) < 5) b = 0;
   }
   lwrite(tmp);
   
   int n = atoi(tmp);
   for(i = 0; i < n; i++) {
      printf("%i. Válasz: ", i + 1);
      fgets(tmp, sizeof(tmp), stdin);
      int j;
      for(j = strlen(tmp); j < 127; j++) {
         tmp[j-1] = ' ';
      }
	  tmp[128] = '\0';
      nwrite(tmp);
   }
   time(&date);
   lwrite(ctime(&date));
}
