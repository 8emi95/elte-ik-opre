       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>

int main()
{
	int valtozo;
	
     valtozo=fork();          /* Megszuljuk a gyermeket ... */
     if (valtozo < 0) {
          /* Hibauzenet kiirasa, a sikertelen rendszerhivasrol! */
     } else {
          if (valtozo != 0) {
			  printf("szulo\n");  
                             /* A gyermek ezt csinalja  ... */
          } else if(valtozo==0 ){
              printf("gyerek\n");            /* A szulo pedig ezt ... */
          }
	}    
}