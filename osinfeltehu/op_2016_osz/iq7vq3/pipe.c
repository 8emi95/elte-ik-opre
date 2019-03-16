       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <math.h>
	//
	// unnamed pipe example
	//
       int main(int argc, char *argv[])
       {
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1) 
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
	       sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 char
               printf("Gyerek olvasta uzenet: %s a kordinatakat!\n",sz);
			printf("Gyerek elkezdi szamolni a kordinatak tavolsagat!\n");
			char number[strlen(sz)];
			memset(number, 0, sizeof(number));
			int i =1;
			int j =0;
			int k =1;
			int vect[4] ;
			//printf("%d\n",strlen(number));
			while(i<strlen(sz)){
				if(sz[i]=='(' || sz[i]==')' || sz[i]==',' || sz[i]==' '){
					if(strlen(number)!=0){
					vect[j++]=atoi(number);
					memset(number, 0, sizeof(number));
					k=1;
					}
					++i;
				}
				else{
					memcpy( number, &sz[i-(k-1)], k ); 
					k++;
					i++;
				}
			}
			double d=sqrt(pow(vect[0]-vect[2],2)+pow(vect[1]-vect[3],2));
			printf("A ket kordinata tavolsaga: %f",d);
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end
           } 
           else 
           {    // szulo process 
               printf("Szulo indul!\n");
			   char Cords[]="(13,31) (2,42)";
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], Cords ,sizeof(Cords));
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta %s kordinatakat a csobe!\n",Cords);
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
