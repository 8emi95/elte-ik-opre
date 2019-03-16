       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
       #include <signal.h>
       #include <sys/types.h>
    //
	// unnamed pipe example
	//

void handler(){
  printf("Megerkeztem");
  printf("\n");
}



int main(int argc, char *argv[])
    {
      int gypipe2[2],gypipe1[2],szpipe[2];
      int n = 3;
      int sikeres=0;
      int random[n];
      char* uzenet;
      char* megjott;
      char* emberek[]={"Nagy Lajos","Nagy Imre","Kis Ember"};
      char* nemjot[n];
      int bol=1;
      int szam;
      pipe(gypipe1);
      pipe(gypipe2);
      pipe(szpipe);
    

 pid_t child_a, child_b;

child_a = fork();

if (child_a == 0) {
 printf("elso gyerek");
 printf("\n");
    /* Child A code */
} else {
    child_b = fork();

    if (child_b == 0) {
        /* Child B code */
        sleep(2);
        printf("masodik gyerek");
        printf("\n");
    } else {
        /* Parent Code */
        sleep(3);
        printf("en vagyok az apad");
        printf("\n");
    }
}



 //     pid_t child = fork();
      
    //  signal(SIGTERM,handler);

      

          //gyerek
          //close(gypipe[0]);
          //close(szpipe[1]);
      
          //jelzes kuldes kill(getppid(),SIGTERM);
   

      
       
   //     close(gypipe[1]);
   

         
      
        
        
        
        
        return 0;
    }
