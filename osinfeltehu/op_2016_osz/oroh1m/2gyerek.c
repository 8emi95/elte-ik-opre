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
  printf("Vegeztem");
  printf("\n");
}



int main(int argc, char *argv[])
    {
      int gypipe2[2],gypipe1[2],szpipe[2];
      int n = 3;
      int sikeres=0;
      char* uzenet;
      char* megjott;
      int emberekszama;
      int random[5];
      char* jotte[5];
      char* nemjot[n];
      int bol=1;
      int szam;
      pipe(gypipe1);
      pipe(gypipe2);
      pipe(szpipe);
    signal(SIGTERM,handler);

 pid_t child_a, child_b;

child_a = fork();

if (child_a == 0) {
    close(gypipe1[0]);
    close(szpipe[1]);


 printf("elso gyerek");
 printf("\n");
 kill(getppid(),SIGTERM);
int j=0;
while(j<5){
    read(szpipe[0],&random[j],sizeof(random[j]));
    printf("Az emberek azonosito %d \n",random[j]);
    if(random[j] < 20){
        nemjot[j] = "nem";
    }
    else{
        nemjot[j] = "igen";
    }
    j++;
}
close(szpipe[0]);
uzenet = "Mukodik";
write(gypipe1[1],&uzenet,sizeof(uzenet));
close(gypipe1[1]);

j=0;
while(j<5){
    
    printf("%d   %s",j,nemjot[j]);
    j++;
}


    /* Child A code */
} else {
    child_b = fork();
    close(szpipe[1]);
    close(gypipe2[0]);
    if (child_b == 0) {
        /* Child B code */
        sleep(2);
        kill(getppid(),SIGTERM);
        printf("masodik gyerek");
        printf("\n");
        
     /*   sleep(10);
        read(gypipe1[0],&uzenet,sizeof(uzenet));
        close(gypipe1[0]);

        printf("\n");
        printf("%s", uzenet);
        */
    } else {
        /* Parent Code */
         close(szpipe[0]);
         close(gypipe1[1]);
        
         sleep(3);
        pause();
        //printf("en vagyok az apad");
        //printf("\n");
        printf("Rendben, olvasom a neveket");
        printf("\n");
        emberekszama = atoi(argv[1]);
        printf("Az embrek szama: %d", emberekszama);
        printf("\n");
        int i=0;
        while(i<emberekszama){
            random[i] = rand() % 100;
            write(szpipe[1],&random[i],sizeof(random[i]));
            //printf("%d ember azonosito szama %d \n",i,random[i]);
            i++;
            }
            close(szpipe[1]);
            sleep(5);
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
