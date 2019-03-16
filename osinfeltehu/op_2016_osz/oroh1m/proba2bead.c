        #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pfd1[2],pfd2[2];
            // unnamed pipe file descriptor array
           pid_t pid;
           char uzenet[20];
           char ember[20];  // char array for reading from pipe
           //char nevek[3];

           //strcpy(nevek[0],"Bence");
           //strcpy(nevek[1],"Timi");
           //strcpy(nevek[2],"Tamas");

     
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
           printf("Rendben!  %s \n");
           
           close(pfd1[1]);
           close(pfd2[0]);
           
           strcpy(uzenet,"Megerkeztem");
           write(pfd2[1],&uzenet,sizeof(uzenet));
           close(pfd2[1]);
           
           sleep(3);
           close(pfd1[1]);
           close(pfd2[0]);

           sleep(3);

           read(pfd1[0],&uzenet,sizeof(uzenet));
           close(pfd1[0]);
           printf("Az I ember: %s", uzenet);

       //    read(pfd1[0],&ember,sizeof(ember));
       //    printf("Az II ember: %s",ember);
       //    close(pfd1[0]);
           /*read(pfd1[0],&uzenet,sizeof(uzenet));
           printf("Atvettem az uzenetet %s", uzenet);
           printf("\n");
           close(pfd1[0]);
           //sleep(5);
           strcpy(uzenet ,"Megerkeztem");
           write(pfd2[1],&uzenet,sizeof(uzenet));
           close(pfd2[1]);

           close(pfd1[1]);
           close(pfd2[0]);
           read(pfd1[0],&uzenet,sizeof(uzenet));
           printf("Az elso ember: %s", uzenet);
           close(pfd1[0]);
          // printf("befejeztem");
         //  printf("\n");*/
           } 
           else 
           {    // szulo process 
               close(pfd1[0]);
               close(pfd2[1]);
               strcpy(uzenet,"Menj a rendezvenyre!");
               write(pfd1[1],&uzenet,sizeof(uzenet));
               close(pfd1[1]);
               
               sleep(3);

               close(pfd1[0]);
               close(pfd2[1]);

               read(pfd2[0],&uzenet,sizeof(uzenet));
               close(pfd2[0]);
               printf("Megkaptam %s \n",uzenet);

               close(pfd1[0]);
               close(pfd2[1]);

               sleep(3);

               strcpy(uzenet,"Tokos");
               write(pfd1[1],&uzenet,sizeof(uzenet));
               close(pfd1[1]);              

         //      strcpy(ember,"Valaki");
           //    write(pfd1[1],&ember,sizeof(ember));
            //   close(pfd1[1]);
               
                /*strcpy(uzenet ,"Menj a helyszinre");
               write(pfd1[1],&uzenet,sizeof(uzenet));
               close(pfd1[1]);
               //sleep(5);
               read(pfd2[0],&uzenet,sizeof(uzenet));
               printf("Nyugtaztam az erkezest : %s", uzenet);
               printf("\n");
               close(pfd2[0]);
           sleep(3);
               close(pfd1[0]);
               close(pfd2[1]);

               strcpy(uzenet,"Tokos Bence");
               write(pfd1[1],&uzenet,sizeof(uzenet));
               close(pfd1[1]);
              // printf("Befejeztem teso en is");*/
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }