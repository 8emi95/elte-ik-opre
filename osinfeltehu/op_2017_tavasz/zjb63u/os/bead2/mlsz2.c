#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#define MAX_SIZE 2048
void arrived (int param)
{
	printf ("\npartner: Keszen all a feladatt vegrehajtasara.\n");
}

void handler(int signumber){
	//---------
}

int main (int argc,char* argv[]) 
{ 

  if(argc < 2){
	  printf("Nem adta meg a varos nevet!\n");
	  exit(1);
  }

  signal(SIGTERM,arrived);
  signal(SIGRTMAX,handler);
  
  pid_t pid; 
  int mypipe[2]; 
  char readbuffer[MAX_SIZE];
  /*pipe létrehozása*/ 
  pipe (mypipe);
  /* child process letrehozasa */ 
  pid = fork ();
  system("clear");
  if (pid > 0) 						/*PARENT*/ 
	{ 
	FILE *kerdesek;
	
	
	kerdesek = fopen("kerdesek.txt","r");
	
char line[MAX_SIZE];
char questions[50][MAX_SIZE];
int index = 0;

pause();

while(fgets(line,sizeof(line),kerdesek) != NULL)
{
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];

	sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
	if(atoi(&definitive[0]) == 1){
		strcpy(questions[index], line);
		index++;
	}
	//---------------------
}
int i;

srand ( time(NULL) );
int r[3];
int equal = 1;
	
	
for(i=0; i<3;i++){
	equal = 1;
	while(equal == 1){
		equal = 0;
		r[i] = rand() % index;
		int j;
		for(j=0; j<i; j++){
			if(r[j] == r[i]) equal=1;
		}
	}
	write(mypipe[1], questions[r[i]] ,200); 
	sleep(1);
	pause();
}

	char result[3][4][20];
	char resul_answers[3][4][50];
	
	for(i=0; i<3; i++){
		pause();
		read(mypipe[0],readbuffer,50);
		sscanf(readbuffer,"%s %s %s %s",result[i][0],result[i][1],result[i][2],result[i][3]);
		memset(readbuffer, 0, sizeof readbuffer);
	}
	
	close(mypipe[0]);
	close(mypipe[1]);
	
	printf("\n\nKonzultacio vege!\n");
	printf("Konzultacio eredmenye:\n");
	printf("=========================\n");
	
	int j;
	
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];
	
	for(i=0; i<3;i++)
	{
		j=0;
		sscanf(questions[r[i]],"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		for(j = 0; j<strlen(question); j++)
		{
			if(question[j] == '#') question[j] = ' ';
		}
		printf("%s\n",question);
		printf("-------------------------\n");
		for(j = 0; j<strlen(answer); j++)
		{
			if(answer[j] == ';'){
				answer[j] = '\n';
			}
		}
		if(atoi(numberOfAnswer)==4){
			sscanf(answer,"%s %s %s %s",resul_answers[i][0],resul_answers[i][1],resul_answers[i][2],resul_answers[i][3]);
		
			printf("%s %s\n",resul_answers[i][0],result[i][0]);
			printf("%s %s\n",resul_answers[i][1],result[i][1]);
			printf("%s %s\n",resul_answers[i][2],result[i][2]);
			printf("%s %s\n\n",resul_answers[i][3],result[i][3]);
		}else if(atoi(numberOfAnswer)==3){
			sscanf(answer,"%s %s %s",resul_answers[i][0],resul_answers[i][1],resul_answers[i][2]);
		
			printf("%s %s\n",resul_answers[i][0],result[i][0]);
			printf("%s %s\n",resul_answers[i][1],result[i][1]);
			printf("%s %s\n\n",resul_answers[i][2],result[i][2]);
		}else if(atoi(numberOfAnswer)==2){
			sscanf(answer,"%s %s",resul_answers[i][0],resul_answers[i][1]);
		
			printf("%s %s\n",resul_answers[i][0],result[i][0]);
			printf("%s %s\n\n",resul_answers[i][1],result[i][1]);
		}
	};
	
	fclose(kerdesek);
	
	wait();
	
	printf("Szulo vege!\n");
	
    exit(EXIT_SUCCESS); 
  } 
	  
  if (pid == 0) 					/*CHILD*/ 
  { 
    char lines[3][200]; 
	int i=0;
	srand ( time(NULL) );
	sleep(1);
	kill(getppid(),SIGTERM); 
	sleep(1);
	
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];
	
	char questions_question[3][MAX_SIZE];
	char questions_NoOfAnswers[3][10];
	char questions_answers[3][MAX_SIZE];
	
	while(i < 3)
	{
		read(mypipe[0],readbuffer,200);
		strcat(lines[i], readbuffer);
		
		sscanf(readbuffer,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		
		strcat(questions_question[i], question);
		strcat(questions_NoOfAnswers[i], numberOfAnswer);
		strcat(questions_answers[i], answer);
		
		memset(readbuffer, 0, sizeof readbuffer);
		memset(question, 0, sizeof question);
		memset(numberOfAnswer, 0, sizeof numberOfAnswer);
		memset(answer, 0, sizeof answer);
		i++;
		sleep(1);
		
		kill(getppid(),SIGRTMAX); 
	}
	
	int konzultalok = rand() % 10 + 10;
	
	printf("\n\nkonzultalok szama: %i\n\n", konzultalok);
	
	

	int j;
	int kerdesekValaszok[3][4];
	int valasz;
	
	for(i=0; i<3; i++){
		for(j=0; j<konzultalok; j++){
			if(atoi(questions_NoOfAnswers[i]) == 0){
				valasz = rand() % 2;
			}else{
				valasz = rand() % atoi(questions_NoOfAnswers[i]);
			}			
			kerdesekValaszok[i][valasz]++;
		}
	}
	
	char result[20];
	char temp[20];
	for(i=0;i<3;i++){
		memset(result, 0, sizeof result);
		sprintf(result, "%i",kerdesekValaszok[i][0]);
		strcat(result," ");
		sprintf(temp, "%i",kerdesekValaszok[i][1]);
		strcat(result, temp);
		strcat(result," ");
		sprintf(temp, "%i",kerdesekValaszok[i][2]);
		strcat(result, temp);
		strcat(result," ");
		sprintf(temp, "%i",kerdesekValaszok[i][3]);
		strcat(result, temp);

		write(mypipe[1],result,50); 
		
		sleep(1);
		kill(getppid(),SIGRTMAX); 
		
	}
	
	sleep(3);
	
	printf("Gyerek vege!\n");

    exit(EXIT_SUCCESS); 
   } 
   return 0;
} 
