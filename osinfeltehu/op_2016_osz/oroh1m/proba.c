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
      int gypipe[2],szpipe[2];
      int n = 3;
      int sikeres=0;
      int random[n];
      char* uzenet;
      char* megjott;
      char* emberek[]={"Nagy Lajos","Nagy Imre","Kis Ember"};
      char* nemjot[n];
      int bol=1;
      int szam;
      pipe(gypipe);
      pipe(szpipe);

 //     pid_t child = fork();
      
      signal(SIGTERM,handler);

      
      if (fork()){
          //szulo
        close(szpipe[0]);
        close(gypipe[1]);
        
        uzenet = "Menj a rendezvenyre!";
        write(szpipe[1], &uzenet , sizeof(uzenet));
        close(szpipe[1]);


        pause();
        printf("Kuldom a neveket akkor:");
        printf("\n");

        sleep(3);
        
        int i =0;
        while(i<n){
            write(szpipe[1],&emberek[i],sizeof(emberek[i]));
            i++;
        }
        close(szpipe[1]);

        printf("A rendezveny folyamatbna... \n");

        sleep(10);

        read(gypipe[0],&sikeres,sizeof(sikeres));

        printf("A rendezveny sikere %d \n", sikeres);

        read(gypipe[0],&szam,sizeof(szam));

         i =0;
        if (szam == 0){
        printf("Mindenki eljott! \n");
        }
        else{
            printf("Akik nem jottek:\n ");
            while(i<szam){
                read(gypipe[0],&nemjot[i],sizeof(nemjot[i]));
                printf("%s\n",nemjot[i]);
                i++;
            }
        }
      //  sleep(3);
        

     
        
        close(gypipe[0]);
    
        exit(0);
      }
      else{
          //gyerek
          close(gypipe[0]);
          close(szpipe[1]);
         
          read(szpipe[0],&uzenet,sizeof(uzenet));
          printf("%s : -Rendben elindultam", uzenet);
          printf("\n");

          sleep(3);
          kill(getppid(),SIGTERM);
   

          sleep(3);

          int j=0;
          while(j<n){
              read(szpipe[0],&emberek[j],sizeof(emberek[j]));
              printf("%s \n",emberek[j]);
              j++;
          }
          close(szpipe[0]);

          //sleep(3);
          
          sikeres = rand() % 100;


          write(gypipe[1],&sikeres,sizeof(sikeres));
  

         int k = 0;
         int l = 0;
         while(k<n){
             random[k] =  rand() % 100;
            // printf("Random szam : %d", random[k]);
             if (random[k] < 10){
                 bol = 0;
                 nemjot[l] = emberek[k];
                 l++; 
             }
             k++;
         }
         szam = l;
         write(gypipe[1],&szam,sizeof(szam));
 
         k =0;
         while(k < szam){
             write(gypipe[1],&nemjot[k],sizeof(nemjot[k]));
             k++;
         }
       
       
       
        close(gypipe[1]);
   

          exit(0);
      }  
        
        
        
        
        return 0;
    }
