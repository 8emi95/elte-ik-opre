	
#include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <signal.h>
#include <sys/types.h>
	//
	// unnamed pipe example
	//
	void handler(int signumber){
		if(signumber==SIGUSR1){
			 printf("utas jelentkezik\n",signumber);
			
		}
	}
 
       int main(int argc, char *argv[])
       {
		    signal(SIGUSR1,handler);
			//signal(SIGUSR2,handler);
 
			 int status;
			 int status2;
		
			int pipefd[2];
			int pipefd2[2];
			pipe(pipefd);
			pipe(pipefd2);

			 pid_t  childa=fork();
		 
			 if (childa<0){perror("The fork calling was not succesful\n"); exit(1);} 
			
			 
			 if (childa>0) 
			{ 
				 	
				  pid_t  childb=fork();
				   if (childb<0){perror("The fork calling was not succesful\n"); exit(1);} 
				  int pid=getpid();
				 if(childb>0){
					 
					 //KÖZPONT
					printf("Kozpont folyamat indul\n");
					
					pause(); //waits till a signal arrive 
					
					printf("Kozpont:Kaptam egy jelzest\n");
					char sz[512];
			
					sleep(2);	// sleeping a few seconds, not necessary
					close(pipefd[1]);  //Usually we close the unused write end
				   printf("KOZPONT: olvassa az adatot!\n");
				   read(pipefd[0],sz,sizeof(sz)); 
				   printf("KOZPONT:Ezt a lakcimet olvasom: %s \n",sz);
				 
					close(pipefd[0]); // finally we close the used read end
									 
					 	char str[15];
					//konvert int to string;
	
					 char uzenetTotaxi [512];
					 
					 strcat(uzenetTotaxi,sz);
					 strcat(uzenetTotaxi," TEL:");
					 sprintf(str, "%d", pid);
					 strcat(uzenetTotaxi,str);
					
				close(pipefd2[0]); //Usually we close unused read end
               write(pipefd2[1], uzenetTotaxi,sizeof(uzenetTotaxi));
               close(pipefd2[1]); // Closing write descriptor 
               printf("KOZPONT:Taxisnak elküldtem a címet és a telefonszamot\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
				waitpid(childa,&status,0);
					waitpid(childb,&status2,0); 
				printf("KOZPONT:VÉGEZTEM \n");
	
				 }else{
					//Utas
					//  waitpid(childa,&status2,0); 
					sleep(1);
					kill(getppid(),SIGUSR1); 
					sleep(2);
			
				
				
				close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], argv[1],sizeof(argv[1]));
               close(pipefd[1]); // Closing write descriptor 
               printf("UTAS:elkuldte a lakcímét\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)

					printf("UTAS:VÉGEZTEM\n");

					 
				 }

			 }
			 else //AUTO
			 {
				
				sleep(5);
				char kapott[512];
				
			close(pipefd2[1]);  //Usually we close the unused write end
	       printf("TAXI:Olvassa az adatot!\n");
           read(pipefd2[0],kapott,sizeof(kapott)); // reading max 100 chars
            printf("TAXI:Ezt a lakcimet es telefonszamot olvastam: %s \n",kapott);
            close(pipefd2[0]); // finally we close the used read end

			
				printf("TAXI:VEGEZTEM\n");
	
	}
				return 0;
		   
		   
	   }
 
