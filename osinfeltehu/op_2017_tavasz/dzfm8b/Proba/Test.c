	
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h> 
#include <signal.h>
#include <sys/types.h>
#include <signal.h>

	void handler(int signumber){
		if(signumber==SIGUSR1){
			 printf("Első megérkezett %i has arrived\n",signumber);
			
		}else if(signumber==SIGUSR2){
			 printf("Masodik megérkezett %i has arrived\n",signumber);
		}
}
	

       int main(int argc, char *argv[])
       {
		   
		    signal(SIGUSR1,handler);
			signal(SIGUSR2,handler);
           
		   
			 int status;
			 int status2;
		
			int pipe1[2];
			int pipe2[2];
			
			pipe(pipe1);
			pipe(pipe2);
				 
			char sz[512];
			int i;
			

			pid_t  childa=fork();
			
			 
			 if (childa<0){perror("The fork calling was not succesful\n"); exit(1);} 
			
			 
			 if (childa>0) 
			{ 
				 	
				  pid_t  childb=fork();
				 if(childb>0){
					 int i;
					for(i=0;i<2;++i){
						
						pause();
						printf("Szülő:Kaptam egy jelzést!\n");
					}
					
					printf("Szülő:Megérkeztek\n");
					 
					// waitpid(childa,&status,0); 
					
			int valami;
			
			valami=getpid();
					
				close(pipe1[0]); //Usually we close unused read end
               write(pipe1[1], &valami,sizeof(valami));
               close(pipe1[1]); // Closing write descriptor 
			   
			   char kuldes[512];
			   for(i=0;i<3;++i){
				   
				   strcat(kuldes,"Valami");
				   
			   }
			   	sleep(3);
			close(pipe1[0]); //Usually we close unused read end
               write(pipe1[1], kuldes,sizeof(kuldes));
               close(pipe1[1]); // Closing write descriptor 
					
					printf("Szülő:beirtam!\n");
					
					
					waitpid(childa,&status,0);
					waitpid(childb,&status2,0); 
		
					printf("Szulő:Vége\n");
	
				 }else{
					 //ChildB
					
					sleep(2);
					kill(getppid(),SIGUSR2); 
						 
	
			
			int kapott;
			sleep(3);	// sleeping a few seconds, not necessary
           close(pipe1[1]);  //Usually we close the unused write end
	       printf("GyerekB: elkezdi olvasni a csobol az adatokat!\n");
            read(pipe1[0],&kapott,sizeof(int)); // reading max 100 chars
			
			
            printf("GyerekB: olvasta uzenet: %i",kapott);
	       printf("\n");
               close(pipe1[0]); // finally we close the used read en
			   
			
		
	       printf("\n");
               close(pipe1[0]); // finally we close the used read en
			   
			
					
					printf("GyerekB:vege\n");
					 
				 }
				
				 
			 
			 }
			 else //childA process
			 {
				 
				kill(getppid(),SIGUSR1); 
				sleep(1);
				
			
		
				
				
				printf("GyerekA:vege\n");
			 }
				return 0;
		   
		   
	   }
 
