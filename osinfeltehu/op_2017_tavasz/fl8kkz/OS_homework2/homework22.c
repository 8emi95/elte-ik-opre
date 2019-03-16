#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <fcntl.h> //open,create
#include <time.h> //ctime
#include <errno.h> //perror, errno
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h> //open
#include <sys/stat.h>	
	void shuffle(int *array, size_t array_size, size_t shuff_size) //classic shuffle algorithm of Fisher-Yates.
    {   
        if (array_size > 1)  
        {   
            size_t i;
            for (i = 0; i < shuff_size - 1; i++) 
            {   
              size_t j = i + rand() / (RAND_MAX / (array_size - i) + 1); 
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
		   if (argc!=2){perror("You have to use program with one argument-the number of voters"); exit(0);}
		   char nameOfcity[20];
		   strcpy(nameOfcity, argv[1]);
           int pipefd1[2]; // unnamed pipe file descriptor array		   
           pid_t pid;           
		   signal(SIGUSR1,handler); 
						   
		   //***************	FOR RANDOM NUMBERS	****************
			srand(time(NULL)); //the starting value of random number generation
			
			
			//**************	TO CHECK IF EVERYTHING WORKS	******************
           if (pipe(pipefd1) == -1) 
			{
               perror("Opening error!");
               exit(EXIT_FAILURE);
			}
		   
			/*if(unlink("fifo.ftc") == -1) {
			perror("Error unlinking before the program runs:");
			}			
			int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); 
			if (fid==-1)
			{
			printf("Error number: %i",errno);
			exit(EXIT_FAILURE);
			}*/
			pid = fork();	// creating parent-child processes
			if (pid == -1) 
			{
               perror("Fork error");
               exit(EXIT_FAILURE);
			}

		   
           if (pid == 0) {		
			kill(getppid(),SIGUSR1); 			
			// child process
	       //sleep(3);	// sleeping a few seconds, not necessary
           close(pipefd1[1]);  //Usually we close the unused write end
	       printf("%s starts to read an array of 3 numbers from the pipe!\n", nameOfcity);
		   int l;
		   int arrayOfQuestions[3];		   
			read(pipefd1[0],arrayOfQuestions,sizeof(arrayOfQuestions));
			
			for (l=0;l<3;l++){   
			printf("%s reads the message: %d\n", nameOfcity, arrayOfQuestions[l]);
			}
			printf("\n");
			close(pipefd1[0]); // finally we close the used read end		   
		
				int p, pp, ppp=0;
				int arrayOfAnswerss[30]; //This is for storing the numbers of lines with answers I will randomly generate 
				for (pp=0;pp<3;pp++){
				for (p=0;p<10;p++){ //for 10 answers for 1st question, 10 for 2nd, 10 for 3rd
					int r=rand()%3+1;					
					arrayOfAnswerss[ppp]=arrayOfQuestions[pp]+r;	
					ppp++;					
				}	
				}	
				int ka;
				for (ka=0;ka<30;ka++){
				printf("lala %d\n", arrayOfAnswerss[ka]);
				}
				
			//THIS ONE MUST BE IN PARENT PROCESS-> HERE IT IS JUST TO CHECK IF IT WORKS AT ALL
		   /*FILE *fp;		   
		   int count=0;
		   fp=fopen("questionnaire.txt","r");
		  
		   if(fp==NULL)
			{
			printf("\nError...cannot open/create files");        
			}
			
			int ll;
			int MAXL=500;
			size_t len = 0;
			char line[MAXL];
			//for (ll=0;ll<30;ll++){
				while (fgets (line, MAXL, fp) != NULL)
				{
				len = strlen (line);
				while (len > 0 && line[len-1] == '\n')
				line[--len] = 0;  
				count++;
				for (ll=0;ll<30;ll++){			
				if(count==arrayOfAnswerss[ll]) 
				printf ("%s\n", line); 
				}			
				
				}

				//}	   
			fclose(fp);    
			*/			
				/*printf("The result of named pipe opening in child: %d!\n",fid);
				int fd;
				fd=open("fifo.ftc",O_WRONLY);
				write(fd,arrayOfAnswerss,sizeof(arrayOfAnswerss));
				close(fd);				 
				printf("%s wrote the message to the named pipe!\n", nameOfcity);*/				
				printf("%s finished the job\n", nameOfcity);
				
           } 
           else 
           {    pause(); //waits till a signal arrive 
				printf("Signal arrived\n");										 
				
				 //Parent process 
               printf("Hungarian National Board starts job!\n");
               close(pipefd1[0]); //Usually we close unused read end			
	
			   int numberOfAllLines=0;
			   int arrayOfQuestions[3]; //This is for storing the numbers I will randomly generate 			   
			   int z=0;
			   //I FIRSTLY COUNT HOW MANY QUESTIONS DO I HAVE AT MY FILE
				int f; 
				f=open("questionnaire.txt",O_RDONLY); 
				if (f<0){ perror("Error at opening the file\n");exit(1);}
				char c;
				while (read(f,&c,sizeof(c))){    
		
				if (c == '\n')
					{
					numberOfAllLines++;
					if (numberOfAllLines % 6==1){
						arrayOfQuestions[z]=numberOfAllLines;  
						z++;}						
					}
             
				} 				
				 close(f);
				
				shuffle(arrayOfQuestions,z,z);	//FOR PICKING NON-REPEATING UNIQUE RANDON NUMBERS
				int x;
				int selectedQuestionNumbers[3];
				for (x=0;x<3;x++){
				selectedQuestionNumbers[x]=arrayOfQuestions[x];
				}	
				int i; 				
				for (i=0;i<3;i++){
					printf("the selected questions are: %d", selectedQuestionNumbers[i]);
				}
				write(pipefd1[1], selectedQuestionNumbers, sizeof(selectedQuestionNumbers)); //I am sending the array of numbers of lines where questions are stored			   
				   
				close(pipefd1[1]); // Closing write descriptor 
				printf("Hungarian National Board wrote the message to the pipe!\n");
				fflush(NULL); 	// flushes all write buffers (not necessary)         
						
		   wait(NULL);		// waiting for child process (not necessary) -Here I can put it, since child finished the job!!!           
	       printf("Hungarian National Board starts reading the answer from the second pipe!\n");
		  		   
			int arrayOfAnswers[30];			
			/*int fd;
			fd=open("fifo.ftc",O_RDONLY);
			read(fd,arrayOfAnswers,sizeof(arrayOfAnswers));			
			close(fd);			
			unlink("fifo.ftc");
			*/
			int a;
			for (a = 0; a < 30; a++) {
            printf("Hungarian National Board reads the message: %d\n", arrayOfAnswers[a]);
			}
	       printf("\n");
          
		   
		   //NOW I WILL READ THE FILE AND PRINT THE RESULT TO THE SCREEN
		   FILE *fp;		   
		   int count=0;
		   fp=fopen("questionnaire.txt","r");
		  
		   if(fp==NULL)
			{
			printf("\nError...cannot open/create files");        
			}
			printf("The possible answers are following\n");
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
			//wait(NULL);		// waiting for child process (not necessary) -Here I can put it, since child finished the job!!!
			printf("Hungarian National Board finished the job!\n");	
	   }
		
		exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
