#include  <stdio.h>
#include  <sys/types.h>
#include  <unistd.h>
#include  <semaphore.h>
#include  <signal.h>
#include <time.h>
#include <stdlib.h>
#define QuestionSize 1000

struct question
{
	int id,finalised;
	char question[QuestionSize],optA[100],optB[100],optC[100],optD[100],createdDate[100];
};
typedef struct 
{
	int id,finalised;
	char question[QuestionSize],optA[100],optB[100],optC[100],optD[100],createdDate[100];
}question;

struct anserw
{
	int optA,optB,optC,optD;
};
typedef struct 
{
	int optA,optB,optC,optD;
	
}anserw;


FILE *qFile;

int questionCount=0;

char buff[QuestionSize];
struct question questions[10000];

void LoadQuestions()
{
	questionCount=0;
	
	if(access("q",F_OK) != -1)
	{
		printf("Loading\n");
		
		qFile= fopen("q","r");
		fgets(buff, QuestionSize, (FILE*)qFile); //darabszám
		questionCount = atoi(buff);
		
		struct question q;
		int i=0;
		for(i=0;i<questionCount;++i)
		{
			q.id=i;
			fgets(buff, QuestionSize, (FILE*)qFile);
			q.finalised=atoi(buff);

			fgets(buff, QuestionSize, (FILE*)qFile);
			strcpy(q.question,strtok(buff,"\n"));
			
			fgets(buff, QuestionSize, (FILE*)qFile);
			strcpy(q.optA,strtok(buff,"\n"));
			
			fgets(buff, QuestionSize, (FILE*)qFile);
			strcpy(q.optB,strtok(buff,"\n"));
			
			fgets(buff, QuestionSize, (FILE*)qFile);
			strcpy(q.optC,strtok(buff,"\n"));
			
			fgets(buff, QuestionSize, (FILE*)qFile);
			strcpy(q.optD,strtok(buff,"\n"));
			
			fgets(buff, QuestionSize, (FILE*)qFile);
			strcpy(q.createdDate,strtok(buff,"\n"));
			
			questions[i]=q;
		}
		
		fclose(qFile);
		printf("Loading Done\n\n");
	}
	else
	{
		printf("question file missing! \n");
		exit(1);
	}
}

void  ChildProcess(void);
void  ParentProcess(int pid);

void ToChild(int signum){printf("ToChild\n");}

void ToParent(int signum){printf("ToParent\n");}

	int pipefd[2];
	char sz[100];
	
void  main(void)
{
     pid_t  pid;
	 
	 if (pipe(pipefd) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
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
	/*
	printf("child\n");
	kill(ParentProcesId, SIGUSR1);
	pause();
	printf("child exit\n");*/
	printf("KB biztos start \n");
	int ParentProcesId=getppid();
	pause();
	printf("KB biztos készen áll \n");
	printf("Kb résztvevőket keres \n");
	
	srand(time(NULL));
	int participants=rand() % 11 + 10;
	
	printf("%d\n",participants);

	//========================================================
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
	
	struct anserw answ[3];
	
	answ[0].optA=0;
	answ[0].optB=0;
	answ[0].optC=0;
	answ[0].optD=0;
	
	answ[1].optA=0;
	answ[1].optB=0;
	answ[1].optC=0;
	answ[1].optD=0;
	
	answ[2].optA=0;
	answ[2].optB=0;
	answ[2].optC=0;
	answ[2].optD=0;
	
	int options=0;
	int r=0;
	int i=0;
	int p=0;
	while(p < participants)
	{
		for(i=0;i<3;i++)
		{
			options=0;
			if(strncmp(questions[i].optD,"NAN",3)!=0)
			{
				options=4;
			}
			else if(strncmp(questions[i].optC,"NAN",3)!=0)
			{
				options=3;
			}
			else 
			{
				options=2;
			}
			
			r=rand() % options;
			if(r==0){answ[i].optA++;}
			if(r==1){answ[i].optB++;}
			if(r==2){answ[i].optC++;}
			if(r==3){answ[i].optD++;}
		}
		p++;
	}
	
	write(pipefd[1], &participants,sizeof(int));
	write(pipefd[1], &answ[0],sizeof(anserw));
	write(pipefd[1], &answ[1],sizeof(anserw));
	write(pipefd[1], &answ[2],sizeof(anserw));
	
	printf("Kb válaszok küldése\n");
	printf("Kérdező biztos kilép\n");
	kill(ParentProcesId, SIGUSR2);
	exit(1);
}

void  ParentProcess(int ChildProcesId)
{/*
	printf("parent\n");
	pause();
	kill(ChildProcesId, SIGUSR1);*/
	
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
		
		if(questions[r].finalised == 1 
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
	
	//========================================================
	kill(ChildProcesId, SIGUSR1);
	pause();
	printf("MLSZ kérdések küldése \n");
	
	write(pipefd[1], &questions[selectedQ[0]],sizeof(question));
	write(pipefd[1], &questions[selectedQ[1]],sizeof(question));
	write(pipefd[1], &questions[selectedQ[2]],sizeof(question));
	
	printf("Kérdések küldve \n");
	//========================================================
	kill(ChildProcesId, SIGUSR1);
	pause();
		
	printf("válaszok fogadása \n");
	
	int p=0;
	struct anserw asw[3];
	
	read(pipefd[0],&p,sizeof(int));
	read(pipefd[0],&asw[0],sizeof(anserw));
	read(pipefd[0],&asw[1],sizeof(anserw));
	read(pipefd[0],&asw[2],sizeof(anserw));
	
	printf("\n\n Résztvevők: %d\n\n",p);
	
	int i=0;
	for(i=0;i<3;i++)
	{
		printf(" Kérdés %d: %s \n",i,questions[selectedQ[i]].question);
		
		printf("  %d     %s  \n", asw[i].optA,questions[selectedQ[i]].optA);
		
		printf("  %d     %s  \n", asw[i].optB,questions[selectedQ[i]].optB);
		
		if(strncmp(questions[selectedQ[i]].optC,"NAN",3)!=0)
		{
			printf("  %d     %s  \n", asw[i].optC,questions[selectedQ[i]].optC);
			
			if(strncmp(questions[selectedQ[i]].optD,"NAN",3)!=0)
			{
				printf("  %d     %s  \n", asw[i].optD,questions[selectedQ[i]].optD);
			}
		}
		
		printf("\n\n");
	}
	
	wait();
	printf("MLSZ Kilép kilép\n");
}










































