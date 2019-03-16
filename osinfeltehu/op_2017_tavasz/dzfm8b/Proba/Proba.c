	
	   #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <signal.h>
		#include <sys/types.h>
		#include <errno.h>
		
	int getRandom(){
  int random=(rand()%10)+10; 
  return random;

	return 0;
}
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
           
		   char kapott[512];
			 int status;
			 int status2;
			 int pipefd[2][2];
			 
int i=0;
	for (i=0; i<3; ++i){
		if(pipe(pipefd[i])==-1){perror("Pipe fail! \n");exit(1);}
	}

			 
	
			 pid_t  childa=fork();
		
			 
			 if (childa<0){perror("The fork calling was not succesful\n"); exit(1);}
			
			 
			 if (childa>0) 
			{ 
				 	
				 pid_t  childb=fork();
				 if(childb>0){
					 	printf("Szulo indul!\n");
					 pause(); //waits till a signal arrive 
					printf("Child A megérkezett\n",SIGUSR1);
					
					 pause(); //waits till a signal arrive 
					printf("Child B megérkezett\n",SIGUSR2);
					
		 
					 printf("Megérkeztek\n");
					 
					 
					static char kitoltott[512];
					 
					 int num;
					sscanf (argv[1],"%d",&num);
					char str[15];
				//konvert int to string;
	
					
					int i;
					for( i=0;i<num;++i){
								sprintf(str, "Azonosito %d:\n",getRandom());
				
					strcat(kitoltott, str);
						
					}
					 
					// printf("%s",kitoltott);
               close(pipefd[0][0]); //Usually we close unused read end
               write(pipefd[0][1], kitoltott,sizeof(kitoltott));
               close(pipefd[0][1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
					 
					 
					 
					 
					// waitpid(childa,&status,0); 
					waitpid(childa,&status,0);
					 	waitpid(childb,&status2,0); 
					printf("I'm the parrent %i\n",getpid());
	
				 }else{
					 //ChildB
					//  waitpid(childa,&status2,0); 
				
					kill(getppid(),SIGUSR2); 
						sleep(2);
					
				
					
					
				
               close(pipefd[0][1]);  //Usually we close the unused write end
			   
	       printf("GYEREKB: elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0][0],kapott,sizeof(kapott)); // reading max 100 chars
			
               printf("GYEREKB: olvasta uzenet: %s",kapott);
			      	printf("I'M Child B %i my Parrent %i\n",getpid(),getppid());
	       printf("\n");
               close(pipefd[0][0]); // finally we close the used read end
					
					
					
				
					printf("ChildB vege\n");
					 
				 }

			 }
			 else //childA process
			 {
				
				kill(getppid(),SIGUSR1); 
				sleep(1);
			
				printf("I'M Child A %i my Parrent %i\n",getpid(),getppid());
			
				printf("GYEREKA: vege\n");
			 }
				return 0;
		   
		   
	   }
 
