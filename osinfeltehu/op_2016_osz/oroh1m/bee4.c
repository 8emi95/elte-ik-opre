       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
       #include <signal.h>
       #include <sys/types.h>
	//
	// unnamed pipe example
	//
       


       
       int main(int argc, char *argv[])
       {
           int pfd1[2],pfd2[2];
            // unnamed pipe file descriptor array
           pid_t pid;
           char uzenet[20];
           char uzenet1[20];
           char* emberek[5]={"Nagy Lajos","Nagy Imre","Osvath Gabor","Fazekas Tamas"};
           int random[5];
           int sikeres;
           int valt;
           int i=0;
           int j=0;

         

           while(j<4){
               random[j] = rand() % 100;
               printf("%d \n", random[j]);
               j++;
           }
           

          if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1)) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	// creating parent-child processes
           if (pid == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	   {		    	// child process
	       close(pfd1[1]);
           close(pfd2[0]);
           
           read(pfd1[0],&uzenet,sizeof(uzenet));
           close(pfd1[0]);
           printf("Vettem: %s \n",uzenet);
           sleep(3);

           strcpy(uzenet1,"Megerkeztem");
           write(pfd2[1],&uzenet1,sizeof(uzenet1));
           close(pfd2[1]);
           
           /*read(pfd1[0],ember,sizeof(ember));
           close(pfd1[0]);
           printf("Az emberek: %s", ember);
           printf("\n");*/

           //sleep(5);

           
           read(pfd1[0],emberek[0],sizeof(emberek[0]));
           close(pfd1[0]);
           printf("Az  emberek: %s", emberek[0]);
           printf("\n");
           //random[0] = rand() % 100;
           //printf("A szam: %d", random[0]);

            read(pfd1[0],emberek[1],sizeof(emberek[1]));
           close(pfd1[0]);
           printf("%s", emberek[1]);
           printf("\n");
           //random[1] = rand() % 100;

            read(pfd1[0],emberek[2],sizeof(emberek[2]));
           close(pfd1[0]);
           printf("%s", emberek[2]);
           printf("\n");
           //random[2] = rand() % 100;

            read(pfd1[0],emberek[3],sizeof(emberek[3]));
           close(pfd1[0]);
           printf("%s", emberek[3]);
           printf("\n");
           //random[3] = rand() % 100;

           //sleep(3);
           
           /*valt = rand() % 100;
           sikeres = &valt;*/
             valt = rand() % 100;
           printf(" ez a siker %d\n", valt);
           sikeres = valt;

           write(pfd2[1],&sikeres,sizeof(sikeres));
           close(pfd2[1]);

     
        
               /*if (random[0] < 10){
                   write(pfd2[1],emberek[0],sizeof(emberek[0]));
                   close(pfd2[1]);
                   sleep(3);
               }

               
               if (random[1] < 10){
                   write(pfd2[1],emberek[1],sizeof(emberek[1]));
                   close(pfd2[1]);
                   sleep(3);
               }
               
               if (random[2] < 10){
                   write(pfd2[1],emberek[2],sizeof(emberek[2]));
                   close(pfd2[1]);
                   sleep(3);
               }
               
               if (random[3] < 10){
                   write(pfd2[1],emberek[3],sizeof(emberek[3]));
                   close(pfd2[1]);
                   sleep(3);
               }*/
           


           } 
           else 
           {    // szulo process 
               close(pfd1[0]);
               close(pfd2[1]);
               
               strcpy(uzenet,"Menj a rendv");
               write(pfd1[1],&uzenet,sizeof(uzenet));
               close(pfd1[1]);
               

              read(pfd2[0],&uzenet1,sizeof(uzenet1));
              close(pfd2[0]);
               
               
               printf("Rendben kuldom a neveket");
               printf("\n");

             //  sleep(2);

               //strcpy(uzenet,"Bela");
               //write(pfd1[1],ember,sizeof(ember));
               //close(pfd1[1]);

               //sleep(2);
               
               write(pfd1[1],emberek[0],sizeof(emberek[0]));
               close(pfd1[1]);
               
               //sleep(2);

               write(pfd1[1],emberek[1],sizeof(emberek[1]));
               close(pfd1[1]);
               
               //sleep(2);

               write(pfd1[1],emberek[2],sizeof(emberek[2]));
               close(pfd1[1]);
               
              // sleep(2);

               write(pfd1[1],emberek[3],sizeof(emberek[3]));
               close(pfd1[1]);
               
               //sleep(5);
               
               read(pfd2[0],&sikeres,sizeof(sikeres));
               close(pfd2[0]);
               printf("A rendezveny sikere: %d %",sikeres);

               
               /*read(pfd2[0],emberek[i],sizeof(emberek[i]));
               close(pfd2[0]);
               printf("Nem jott el : %s",emberek[i]);
               */
             
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }