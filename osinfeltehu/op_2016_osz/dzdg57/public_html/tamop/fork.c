#include <iostream> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;
int main() { 
     int a, b; 
     a = fork(); 
     if (a < 0) { 
          cerr << "A fork hivas sikertelen!\n"; 
          exit(1); 
     }; 
     if (a) { 
          cout <<"Hello, ez a szolo!\n"; 
          cout <<"A gyerekem pid-je: " << a << "\n"; 
          cout <<"Most varakozik a wait....\n"; 
          wait(&b); 
          cout<<"Most van vege a szulo varakozasanak...\n"; 
     } else { 
          cout <<"Ez a gyerek processz! \n"; 
          cout <<"Billentyuleutesre varunk...\n"; 
          getchar(); 
          cout <<"Es ezzel a kiirassal a gyerek processz le is fut...\n"; 
          exit(0); 
     }; 
}