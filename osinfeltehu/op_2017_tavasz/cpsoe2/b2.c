#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#define question_length 500
#define answer_length 100

#define filename "MLSZ_questions.txt"

struct question
{
	int id,finalized;
	char question[question_length],optionA[answer_length],optionB[answer_length],optionC[answer_length],optionD[answer_length],createdDate[60];
};
typedef struct 
{
	int id,finalized;
	char question[question_length],optionA[answer_length],optionB[answer_length],optionC[answer_length],optionD[answer_length],createdDate[60];
}question;

struct answer
{
	int optionA,optionB,optionC,optionD;
};
typedef struct 
{
	int optionA,optionB,optionC,optionD;
	
}answer;


FILE *qFile;

int questionCount=0;

char buff[question_length];
struct question questions[10000];

void LoadQuestions()
{
	questionCount=0;
	
	if(access(filename,F_OK) != -1)
	{
		printf("Loading\n");
		
		qFile= fopen(filename,"r");
		fgets(buff, question_length, (FILE*)qFile);
		questionCount = atoi(buff);
		
		struct question q;
		int i=0;
		for(i=0;i<questionCount;++i)
		{
			q.id=i;
			fgets(buff, question_length, (FILE*)qFile);
			q.finalized=atoi(buff);

			fgets(buff, question_length, (FILE*)qFile);
			strcpy(q.question,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)qFile);
			strcpy(q.optionA,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)qFile);
			strcpy(q.optionB,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)qFile);
			strcpy(q.optionC,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)qFile);
			strcpy(q.optionD,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)qFile);
			strcpy(q.createdDate,strtok(buff,"\n"));
			
			questions[i]=q;
		}
		
		fclose(qFile);
		printf("Loading Done\n\n");
	}
	else
	{
		printf("Question file is missing!\n");
		exit(1);
	}
}

void  ChildProcess(void);
void  ParentProcess(int pid);

void ToChild(int signum){printf("ToChild\n");}

void ToParent(int signum){printf("ToParent\n");}

	int pipefd[2];
	char sz[answer_length];
	
void  main(void)
{
     pid_t  pid;
	 
	 if (pipe(pipefd) == -1) 
	{
	   perror("Hiba a pipe nyitásakor!");
	   exit(EXIT_FAILURE);
	}
	 
	 signal(SIGUSR1, ToChild);
	 signal(SIGUSR2, ToParent);
	 
     pid = fork();
     if (pid == 0) ChildProcess();
     else ParentProcess(pid);
}



void  ChildProcess(void)
{
	printf("KB biztos start \n");
	int ParentProcesId=getppid();
	pause();
	printf("KB biztos készen áll \n");
	printf("Kb résztvevőket keres \n");
	
	srand(time(NULL));
	int participants=rand() % 11 + 10;
	
	printf("%d\n",participants);

	kill(ParentProcesId, SIGUSR2);
	pause();
	printf("Kb kérdések fogadása\n");
	 
	struct question questions[3];
	read(pipefd[0],&questions[0],sizeof(question));
	read(pipefd[0],&questions[1],sizeof(question));
	read(pipefd[0],&questions[2],sizeof(question));
		
	printf("%s\n",questions[0].question);
	printf("%s\n",questions[1].question);
	printf("%s\n",questions[2].question);
	
	printf("Kb válaszok feldolgozása\n");
	
	struct answer answ[3];
	
	answ[0].optionA=0;
	answ[0].optionB=0;
	answ[0].optionC=0;
	answ[0].optionD=0;
	
	answ[1].optionA=0;
	answ[1].optionB=0;
	answ[1].optionC=0;
	answ[1].optionD=0;
	
	answ[2].optionA=0;
	answ[2].optionB=0;
	answ[2].optionC=0;
	answ[2].optionD=0;
	
	int options=0;
	int r=0;
	int i=0;
	int p=0;
	while(p < participants)
	{
		for(i=0;i<3;i++)
		{
			options=0;
			if(strncmp(questions[i].optionD,"NAN",3)!=0)
			{
				options=4;
			}
			else if(strncmp(questions[i].optionC,"NAN",3)!=0)
			{
				options=3;
			}
			else 
			{
				options=2;
			}
			
			r=rand() % options;
			if(r==0){answ[i].optionA++;}
			if(r==1){answ[i].optionB++;}
			if(r==2){answ[i].optionC++;}
			if(r==3){answ[i].optionD++;}
		}
		p++;
	}
	
	write(pipefd[1], &participants,sizeof(int));
	write(pipefd[1], &answ[0],sizeof(answer));
	write(pipefd[1], &answ[1],sizeof(answer));
	write(pipefd[1], &answ[2],sizeof(answer));
	
	printf("Kb válaszok küldése\n");
	printf("Kérdező biztos kilép\n");
	kill(ParentProcesId, SIGUSR2);
	exit(1);
}

void  ParentProcess(int ChildProcesId)
{
	sleep(1);
	printf("MLSZ kérdéseket választ \n");
	LoadQuestions();
	int selectedQ[3];
	selectedQ[0]=-1;
	selectedQ[1]=-1;
	selectedQ[2]=-1;
	
	int selectedCnt=0;
	
	srand(time(NULL));
	int r;
	
	while(selectedCnt<3)
	{
		r = rand() % questionCount;
		
		if(questions[r].finalized == 1 
		   && r != selectedQ[0]
		   && r != selectedQ[1]
		   && r != selectedQ[2]
		   )
		{
			selectedQ[selectedCnt]=r;
			selectedCnt++;
		}
	}
	
	printf("%d\n",selectedQ[0]);
	printf("%d\n",selectedQ[1]);
	printf("%d\n",selectedQ[2]);
	

	kill(ChildProcesId, SIGUSR1);
	pause();
	printf("MLSZ kérdések küldése \n");
	
	write(pipefd[1], &questions[selectedQ[0]],sizeof(question));
	write(pipefd[1], &questions[selectedQ[1]],sizeof(question));
	write(pipefd[1], &questions[selectedQ[2]],sizeof(question));
	
	printf("Kérdések küldve \n");


	kill(ChildProcesId, SIGUSR1);
	pause();
		
	printf("válaszok fogadása \n");
	
	int p=0;
	struct answer asw[3];
	
	read(pipefd[0],&p,sizeof(int));
	read(pipefd[0],&asw[0],sizeof(answer));
	read(pipefd[0],&asw[1],sizeof(answer));
	read(pipefd[0],&asw[2],sizeof(answer));
	
	printf("\n\n Résztvevők: %d\n\n",p);
	
	int i=0;
	for(i=0;i<3;i++)
	{
		printf(" Kérdés %d: %s \n",i,questions[selectedQ[i]].question);
		
		printf("  %d     %s  \n", asw[i].optionA,questions[selectedQ[i]].optionA);
		
		printf("  %d     %s  \n", asw[i].optionB,questions[selectedQ[i]].optionB);
		
		if(strncmp(questions[selectedQ[i]].optionC,"NAN",3)!=0)
		{
			printf("  %d     %s  \n", asw[i].optionC,questions[selectedQ[i]].optionC);
			
			if(strncmp(questions[selectedQ[i]].optionD,"NAN",3)!=0)
			{
				printf("  %d     %s  \n", asw[i].optionD,questions[selectedQ[i]].optionD);
			}
		}
		
		printf("\n\n");
	}
	
	wait();
	printf("MLSZ Kilép\n");
}










































