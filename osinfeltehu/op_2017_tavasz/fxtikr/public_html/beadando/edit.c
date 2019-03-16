#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit.h"

void edit(){

   char input[128];
   char line[512];
   int i = 0;
   int b = 1;
   FILE* f;

   while(b){

      printf("Adja meg a módosítani kívánt kérdés számát: \n");
      fgets(input, sizeof(input), stdin);
      
      i = atoi(input);
      if ( i >= 1 ) b = 0;

   }

   f = fopen("kerdesek.txt", "r+");


   // Kiszámolja a megadott kérdésig a fájlt

      int jelenlegikerdes = 1;      

      while ( jelenlegikerdes < i ) {
         fgets(line, sizeof(line), f); // Kérdés neve
         memset(line, 0, sizeof line);
         int skip = atoi(fgets(line, sizeof(line), f));
         int j;
         for(j = 0; j < skip; j++){
            fgets(line, sizeof(line), f);
            memset(line, 0, sizeof line);
         }
         fgets(line, sizeof(line), f); // Dátumot is át kell ugrani
         jelenlegikerdes += 1;
      } 

   // Adatok felülírása a fájlban
   printf("Adja meg az új kérdést (max 128 karakter): ");
   fgets(input, sizeof(input), stdin);
   input[strcspn(input, "\n")] = 0;
   fprintf(f, "%s", input);

   fgets(line, sizeof(line), f);
   int valaszok_regi = atoi(fgets(line, sizeof(line), f));

   for(i = 0; i < valaszok_regi; i++) {
         printf("%i. Válasz: ", i+1);
         fgets(input, sizeof(input), stdin);
         input[strcspn(input, "\n")] = 0;
         fprintf(f, "%s", input);
         fgets(line, sizeof(line), f);
   }
   fclose(f);  
}

void deleteq() {
   char input[128];
   char line[512];

   int i;
   int b = 1;

   FILE *f, *g;

   while(b){
      printf("Adja meg a törlendő kérdés sorszámát: ");
      fgets(input, sizeof(input), stdin);
      i = atoi(input);
      if(i >= 1) b = 0;
   }
   
   f = fopen("kerdesek.txt", "r+");
   g = fopen("kerd_tmp.txt", "a+");

      // Fájl átmásolása egy ideiglenes fájlba a törlendő kivételével
      int jelenlegikerdes = 1;
      int skip = 0;
      while ( !feof(f) ) {
         if(atoi(input) != jelenlegikerdes) {

           fgets(line, sizeof(line), f); // Kérdés neve
           if(feof(f)) break;
           fprintf(g, "%s", line);

           skip = atoi(fgets(line, sizeof(line), f));
           fprintf(g, "%i\n", skip);

           int j;
           for(j = 0; j < skip; j++){
             fgets(line, sizeof(line), f);
             fprintf(g, "%s", line);
           }
           fgets(line, sizeof(line), f); // Dátumot
           fprintf(g, "%s", line);

        } else {
	   fgets(line, sizeof(line), f);
           skip = atoi(fgets(line, sizeof(line), f));
           int k;
           for(k = 0; k < skip; k++) { fgets(line, sizeof(line), f);  }
           fgets(line, sizeof(line), f);
        }
       jelenlegikerdes += 1;
     }

     fclose(f);
     fclose(g);
     remove("kerdesek.txt");

     // Fájl visszamásolása
     f = fopen("kerdesek.txt", "a+");
     g = fopen("kerd_tmp.txt", "r+");
     while( !feof(g) ) {
	fgets(line, sizeof(line), g);
        if( feof(g) ) break;
        fprintf(f, "%s", line);
     }
     fclose(g);
     fclose(f);
     remove("kerd_tmp.txt");
}
