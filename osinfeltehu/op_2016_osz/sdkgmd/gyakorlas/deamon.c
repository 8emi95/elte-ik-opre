//-------------------- 
// Demon.c 
//-------------------- 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
int main() { 
     daemon(1,1); // A démon folyamat indítása. 
     // Az első paraméter nulla, akkor a gyökérkönyvtárra vált. 
     // A második paraméter nulla, akkor a standard csatornákat átirányítja. 
     while (1) { 
          printf("demon vagyok\n"); 
          sleep(1); 
     } 
     return 0; 
} 
//--------------------
