#include <stdio.h> 
#include <stdlib.h> 

void main() 
{ 
     int a, b; 
     a = fork(); 
     if (a < 0) 
     { 
          printf( "A fork hivas sikertelen!\n" ); 
          exit(1); 
     } 
     if (a) 
     { 
          printf( "Hello, ez a szolo!\n" ); 
          printf( "A gyerekem pid-je: %d \n", a ); 
          printf( "Most varakozik a wait....\n" ); 
          wait(&b); 
          printf( "Most van vege a szulo varakozasanak...\n" ); 
     } 
     else 
     { 
          printf( "Ez a gyerek processz! \n" ); 
          printf( "Billentyuleutesre varunk...\n" ); 
          getchar(); 
          printf( "Es ezzel a kiirassal a gyerek processz le is fut...\n" ); 
          exit(0); 
     }
}