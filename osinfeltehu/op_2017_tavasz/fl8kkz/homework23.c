#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct quest_bank{

	char questions[80];
	char answers[4][80];
	char timeadded[80];
};


void handler(int signumber){
  printf("Signal with number %i is arriving\n",signumber);
}

int main(int argc, char *argv[]){
	if (argc!=2){perror("You have to use program with one argument-the number of voters"); exit(0);}
		   char nameOfcity[20];
		   strcpy(nameOfcity, argv[1]);
		   
	int cnt = 0;
	struct quest_bank qbank[250];
	int num_of_q_a = 0;
	//int num_of_lin = 0;	 
    
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                       
  
  srand(time(NULL));
  pid_t child;
  int rand1, rand2;
  int pipefd[2]; 
  int pipefd2[2]; 
  pid_t pid;; 
  key_t key; 
  char sz[100];  
  int sz2[100];


  if (pipe(pipefd2) == -1 ){
  	perror("Opening error second pipe!");
    exit(EXIT_FAILURE);
  }

  if (pipe(pipefd) == -1) 
  {   
    perror("Opening error!");
    exit(EXIT_FAILURE);
  }
   
  		   
  if((child = fork()) < 0){
	perror("Fork error");
  }
	  
  if (child>0) // parent 
  { 
	  printf("Society of Hungarian Games began work\n"); 
	  pause();
	  printf("%s is ready\n", nameOfcity);
	  //choose 3 random questions
		char input[250];
		int cnt = 0;
		//open for reading
		FILE *my_file = fopen("questionnaire.txt", "r");
		if(my_file == NULL) 
		{
			perror("Error opening file");
			//return(-1);
		}
  
		//check for empty file
		while(fgets(input, 250, my_file)!= NULL){

			int cnt_answ = 0;
			strcpy(qbank[cnt].questions, input);
			//num_of_lin++;
			while(cnt_answ < 4){ 
				//read and copy question lines
				fgets(input, 250, my_file);
				strcpy(qbank[cnt].answers[cnt_answ], input);
				cnt_answ++;
				//num_of_lin++;
			}
			fgets(input, 250, my_file);
			strcpy(qbank[cnt].timeadded, input);
			//num_of_lin++;
			cnt++;
		}
	fclose(my_file);
	num_of_q_a = cnt;
	
	
	  int r1 = rand() % 10;
	  int r2 = rand() % 10;
	  int r3 = rand() % 10;
	  
	 // char writePipe[100];
	 int cnt_answ = 0;
	 int j=0;
	  for(j = 0; j < num_of_q_a; j++){
    	printf("The content of the file: %s%s%s%s%s%s\n\n", qbank[j].questions, qbank[j].answers[cnt_answ],
    		qbank[j].answers[cnt_answ + 1], qbank[j].answers[cnt_answ + 2], qbank[j].answers[cnt_answ + 3],
			qbank[j].timeadded);
    }
	  close(pipefd[0]);
	  close(pipefd2[1]);

      //write to 1st pipe
      write(pipefd[1], qbank[r1].questions,strlen(qbank[r1].questions));
      write(pipefd[1], qbank[r2].questions,strlen(qbank[r2].questions));
      write(pipefd[1], qbank[r3].questions,strlen(qbank[r3].questions));
      
      close(pipefd[1]);

     char output[250];
	/*int cnt_answ = 0;

	//open file for writing
	FILE *my_file2 = fopen("questionnaire.txt", "w");	
	if(my_file2 == NULL)	 
    {
      	perror("Error opening file");
      	//return(-1);
    }
    //printf("Number of questions should be written back %d\n",num_of_q_a );
    //read back struct to file
    int j;
    for(j = 0; j < num_of_q_a; j++){
    	fprintf(my_file2, "%s%s%s%s%s%s", qbank[j].questions, qbank[j].answers[cnt_answ],
    		qbank[j].answers[cnt_answ + 1], qbank[j].answers[cnt_answ + 2], qbank[j].answers[cnt_answ + 3],
			qbank[j].timeadded);
    }
    fclose(my_file);
	*/
	
      pause();
      printf("%s finished reading questions\n", nameOfcity);
      
      pause();
      read(pipefd2[0], sz2, sizeof(sz2));
      int i ;
      
      for(i=1; i < sz2[0]; i++ ){
      	printf("This is the answers of filled survey %d\n", sz2[i]);
      }
	      
	  close(pipefd2[0]);
	  

      wait(NULL);
	  printf("Society of Hungarian Games finished their work\n");
	  
  }
  else  //child
  {
	  sleep(1);
	  printf("%s is sending signal..\n", nameOfcity);
	  kill(getppid(), SIGUSR1); 
	  sleep(1);
	  
	  close(pipefd[1]);
	  close(pipefd2[0]);

	  //read pipe
	  read(pipefd[0], sz, sizeof(sz));
	  printf("Society of Hungarian Ball Games sent following questions: %s\n", sz);
	  close(pipefd[0]);
	  kill(getppid(), SIGUSR1);
	  sleep(1);
	  int people[70];
	  int rand_people[20];
	  int rnd, i;
	  //fill all people
	  for(i = 0; i < 50; i++){
	  	people[i] = i; 
	  }
	  //choose random people
	  for(i = 0; i < 20; i++){
	  	rnd = rand() % 50;
	  	rand_people[i] = rnd;

	  }
	  int filled_survey[85];
	  //fill the survey
	  filled_survey[0] = 3*(sizeof(rand_people) / sizeof(int));
	  for(i = 1; i < 3*(sizeof(rand_people) / sizeof(int)); i++){
	  	filled_survey[i] = rand() % 5;
	  	//printf("Filled survey %d\n", filled_survey[i]);

	  }

	  //write to second pipe
	  write(pipefd2[1], filled_survey,filled_survey[0]*4);
	  close(pipefd2[1]);
	  kill(getppid(), SIGUSR1);

	  printf("%s finished his work\n", nameOfcity); 

  }
  return 0;
}