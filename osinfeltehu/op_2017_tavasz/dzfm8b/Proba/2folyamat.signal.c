	
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
			 int notacommonvalue=1;
			 printf("The value is %i before forking \n",notacommonvalue);
			 
			 pid_t  childa=fork();
			
			 
			 if (childa<0){perror("The fork calling was not succesful\n"); exit(1);} 
			
			 
			 if (childa>0) 
			{ 
				 	
				  pid_t  childb=fork();
				 if(childb>0){
					 
					 pause(); //waits till a signal arrive 
					printf("Child A megérkezett\n",SIGUSR1);
					
					 pause(); //waits till a signal arrive 
					printf("Child B megérkezett\n",SIGUSR2);

					printf("MEgérkeztek\n");
					 
					// waitpid(childa,&status,0); 
					waitpid(childa,&status,0);
					 waitpid(childb,&status2,0); 
				printf("The value is %i in childb process \n",notacommonvalue);
	
				 }else{
					 //ChildB
					//  waitpid(childa,&status2,0); 
					sleep(2);
					 	kill(getppid(),SIGUSR2); 
						 
						 notacommonvalue=6; //it changes the value of the copy of the variable
					printf("The value is %i in childb process \n",notacommonvalue);
					printf("ChildB vege\n");
					 
				 }
				
				 
			 
			 }
			 else //childA process
			 {
				 
				kill(getppid(),SIGUSR1); 
				sleep(1);
				notacommonvalue=5; //it changes the value of the copy of the variable
				printf("The value is %i in childa process \n",notacommonvalue);
				printf("ChildA vege\n");
			 }
				return 0;
		   
		   
	   }
 
