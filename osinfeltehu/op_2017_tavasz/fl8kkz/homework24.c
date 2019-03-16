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
struct resultToparent{

	char questions[80];
	char answers[10][80];
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
	
	int num_of_q_a = 0;	
    
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                       
  
  srand(time(NULL));
  pid_t child;
  int rand1, rand2;
  int pipefd[2]; 
  int pipefd2[2]; 
  pid_t pid;; 
  key_t key; 
  char sz[100];  
  int sz2[30];


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
	 
		char input[250];
		int cnt = 0;
		//open for reading
		FILE *my_file = fopen("questionnaire.txt", "r");
		if(my_file == NULL) 
		{
			perror("Error opening file");
			//return(-1);
		}
		struct quest_bank qbank[10];
		
		while(fgets(input, 250, my_file)!= NULL){

			int cnt_answ = 0;
			strcpy(qbank[cnt].questions, input);			
			while(cnt_answ < 4){ 				
				fgets(input, 250, my_file);
				strcpy(qbank[cnt].answers[cnt_answ], input);
				cnt_answ++;
				
			}
			fgets(input, 250, my_file);
			strcpy(qbank[cnt].timeadded, input);			
			cnt++;
		}
	fclose(my_file);
	num_of_q_a = cnt;
	
	struct quest_bank tochild[3];
	  int r1 = rand() % 10;
	  int r2 = rand() % 10;
	  int r3 = rand() % 10;
	  
	
	 int cnt_answ = 0;
	 int j=0;
	
	  for(j = 0; j < num_of_q_a; j++){
    	printf("The content of the file: %s%s%s%s%s%s\n", qbank[j].questions, qbank[j].answers[cnt_answ],
    		qbank[j].answers[cnt_answ + 1], qbank[j].answers[cnt_answ + 2], qbank[j].answers[cnt_answ + 3],
			qbank[j].timeadded);
    }
	  close(pipefd[0]);
	  close(pipefd2[1]);
	  int y;
		for (y=0;y<3;y++){
	  strcpy(tochild[y].questions, qbank[y].questions);
	  int yy;
		for (yy=0;yy<4;yy++)
	  strcpy(tochild[y].answers[yy], qbank[y].answers[yy]);
	  strcpy(tochild[y].timeadded, qbank[y].timeadded);
		}
     
	  write(pipefd[1], tochild, sizeof(tochild));
      
      close(pipefd[1]);

     char output[250];
	
      pause();
      printf("%s finished reading questions\n", nameOfcity);
      
      pause();
	 struct resultToparent fromchild[3];     
	 read(pipefd2[0], fromchild, sizeof(fromchild));
	 printf("%s sent back following results:\n", nameOfcity);
	 int jf;
	 int cnt_answw=0;
    for(jf = 0; jf < 3; jf++){
    	printf("%s%s%s%s%s%s%s%s%s%s%s%%s", fromchild[jf].questions, fromchild[jf].answers[cnt_answw],
    		fromchild[jf].answers[cnt_answw + 1], fromchild[jf].answers[cnt_answw + 2], fromchild[jf].answers[cnt_answw + 3],
			fromchild[jf].answers[cnt_answw + 4], fromchild[jf].answers[cnt_answw + 5], fromchild[jf].answers[cnt_answw + 6],
			fromchild[jf].answers[cnt_answw + 7], fromchild[jf].answers[cnt_answw + 8], fromchild[jf].answers[cnt_answw + 9],
			fromchild[jf].timeadded);
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

	  int t;
	  struct quest_bank fromparent[3];
	  
	  read(pipefd[0], fromparent, sizeof(fromparent));
	  printf("Society of Hungarian Ball Games sent following questions:\n" );
	  int j;
	  int cnt_answ = 0;
	  for(j = 0; j < 3; j++){
    	printf("%s%s%s%s%s%s\n", fromparent[j].questions, fromparent[j].answers[cnt_answ],
    		fromparent[j].answers[cnt_answ + 1], fromparent[j].answers[cnt_answ + 2], fromparent[j].answers[cnt_answ + 3],
			fromparent[j].timeadded);
    }
	  close(pipefd[0]);
	  kill(getppid(), SIGUSR1);
	  sleep(1);
	  int p=0;
	 
		struct resultToparent toparent[3];
		int y;
		for (y=0;y<3;y++){
		strcpy(toparent[y].questions, fromparent[y].questions);
		int yy;
		for (yy=0;yy<10;yy++){
			int rr=rand()%3;
		strcpy(toparent[y].answers[yy], fromparent[y].answers[rr]);
		}
		strcpy(toparent[y].timeadded, fromparent[y].timeadded);
		}
		int jf;
	 int cnt_answw=0;    
	  
	  write(pipefd2[1], toparent,sizeof(toparent));
	  close(pipefd2[1]);
	  kill(getppid(), SIGUSR1);

	  printf("%s finished his work\n", nameOfcity); 

  }
  return 0;
}