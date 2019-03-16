#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <fcntl.h> 
#include <time.h> 
#include <errno.h> 
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <sys/stat.h>	
	
void shuffle(int *array, size_t sizeArray, size_t size)
    {   
        if (sizeArray > 1)  
        {   
            size_t i;
            for (i = 0; i < size - 1; i++) 
            {   
              size_t j = i + rand() / (RAND_MAX / (sizeArray - i) + 1); 
              int t = array[j];
              array[j] = array[i];
              array[i] = t;
            }   
        }   
    }   
	
	void handler(int signumber){
	printf("Signal with number %i is arriving\n",signumber);
	}
       int main(int argc, char *argv[])
       {
		   
		   char city[20];
		   strcpy(city, argv[1]);
           int pipefd1[2]; 
		   int pipefd2[2];
           pid_t pid;
           
		   signal(SIGUSR1,handler); 
						   
		  
			srand(time(NULL)); 
			
           if (pipe(pipefd1) == -1) 
	   {
               perror("Opening error!");
               exit(EXIT_FAILURE);
           }
		   if (pipe(pipefd2) == -1) 
	   {
               perror("Opening error!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	
           if (pid == -1) 
	   {
               perror("Fork error");
               exit(EXIT_FAILURE);
           }

		   
           if (pid == 0) 
	   {		printf("Waits 3 seconds, then send a SIGUSR1 %i signal\n",SIGUSR1);
				sleep(3);
				kill(getppid(),SIGUSR1); 		
			
	       sleep(3);	
           close(pipefd1[1]); 
	       printf("%s read an array of 3 numbers from the pipe!\n", city);
		   int l;
		   int questionsArray[3];
		   for (l=0;l<3;l++){
           read(pipefd1[0],&questionsArray[l],sizeof(int));
			   
           printf("%s reads the message: %d\n", city, questionsArray[l]);
		   }
			printf("\n");
			close(pipefd1[0]); 	   
		
					
				close(pipefd2[0]); 
				int b, a, c=0; 
				int arrayOfAnswerss[30]; 
				for (a=0;a<3;a++){
				for (b=0;b<10;b++){ 
					int r=rand()%3+1;					
					arrayOfAnswerss[c]=questionsArray[a]+r;	
					c++;					
				}	
				}	
				
				
				write(pipefd2[1], &arrayOfAnswerss, sizeof(arrayOfAnswerss)); 
				
				close(pipefd2[1]); 
				printf("%s wrote the message to the pipe!\n", city);
				fflush(NULL); 	
				printf("%s finished the job\n", city);
				
           } 
           else 
           {    pause(); 
				printf("Signal arrived\n");
				
				
               printf("Hungarian National Board starts job!\n");
               close(pipefd1[0]); 			
	
			   int numberOfAllLines=0;
			   int questionsArray[3]; 		   
			   int y=0;
                         
				int f; 
				f=open("qu.txt",O_RDONLY); 
				if (f<0){ perror("Error at opening the file\n");exit(1);}
				char c;
				while (read(f,&c,sizeof(c))){    
		
				if (c == '\n')
					{
					numberOfAllLines++;
					if (numberOfAllLines % 6==1){
						questionsArray[y]=numberOfAllLines;  
						y++;}						
					}
             
				} 				
				 close(f);
				
				shuffle(questionsArray,y,y);	
				int x;
				int questionNumber[3];
				for (x=0;x<3;x++){
				questionNumber[x]=questionsArray[x];
				}
			  
				write(pipefd1[1], &questionNumber, sizeof(questionNumber)); 		   
				   
				close(pipefd1[1]); 
				printf("message wrote  to the pipe!\n");
				fflush(NULL); 	          
						
		   sleep(3);	
           close(pipefd2[1]);  
	       printf(" reading the answer from the second pipe!\n");
		  
			int arrayOfAnswers[30];
			read(pipefd2[0], &arrayOfAnswers, sizeof(arrayOfAnswers));
			int a;
			for (a = 0; a < 30; a++) {
            printf(" reads the message: %d\n", arrayOfAnswers[a]);
			}
	       printf("\n");
           close(pipefd2[0]); 
		   wait(NULL);		
		   
		   FILE *fp;		   
		   int count=0;
		   fp=fopen("qu.txt","r");
		  
		   if(fp==NULL)
			{
			printf("\nError, can not open the files");        
			}
			printf("The  answers are following\n");
			int ll;
			int MAXL=500;
			size_t len = 0;
			char line[MAXL];
			
				while (fgets (line, MAXL, fp) != NULL)
				{
				len = strlen (line);
				while (len > 0 && line[len-1] == '\n')
				line[--len] = 0;  
				count++;
				for (ll=0;ll<30;ll++){			
				if(count==arrayOfAnswers[ll]) 
				printf ("%s\n", line); 
				}			
				
				}

				  
			fclose(fp);		
			
			printf(" Done !\n");	
	   }
		
		exit(EXIT_SUCCESS);	
       }

 