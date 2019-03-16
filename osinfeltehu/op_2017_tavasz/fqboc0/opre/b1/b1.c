#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h> //ctime
#include <time.h>
#define QuestionSize 1000


FILE *qFile;

struct question
{
	int id,finalised;
	char question[QuestionSize],optA[100],optB[100],optC[100],optD[100],createdDate[100];
};

void dispQuestion(struct question q)
{
		printf("Id: %d\n",q.id);
		if(q.finalised == 0)
		{
			printf("NOT Finalised\n");
		}
		else
		{
			printf("Finalised\n");
		}
		printf("Created Date: %s \n\n",q.createdDate);
		printf("%s\n",q.question);
		printf("  A: %s\n",q.optA);
		printf("  B: %s\n",q.optB);
		if(strcmp(q.optC,"NAN")!=0)
		{
			 printf("  C: %s\n",q.optC);
		}
		if(strcmp(q.optD,"NAN")!=0)
		{
			printf("  D: %s\n\n",q.optD);
		}
		printf("\n\n");
}

int questionCount=0;

char buff[QuestionSize];
struct question questions[10000];

time_t timer;
struct tm* tm_info;

int main(int argc,char ** argv) 
{
	printf("\n\n\nHello Stranger!\n");
	printf("Welcome to MLSZ questionnaire builder!\n\n");
	
	Init();
	
	do 
	{
		dispMenu();
		
		fgets(buff,QuestionSize,stdin);
		
		switch (buff[0])
		{
			case '1': addQuestion();saveQuestionList(); break;
			case '2': alterQuestion();saveQuestionList(); break;
			case '3': deleteQuestion(); saveQuestionList();break;
			case '4': listQuestions();break;
			case '5': finaliseQuestions();break;
			case '6': listFinalisedQuestions(); break;
			case '7': printf("exiting\n\n"); break;
			default: printf("Wrong command\n\n");
		}
		
	} while( buff[0] != '7');
	
	return 0;
}
void Init()
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
		qFile =fopen("q","w");
		fprintf(qFile,"0\n");
		fclose(qFile);
	}
}

void dispMenu()
{
	
	printf("Main menu\n");
	printf("  1. Add new question\n");
	printf("  2. Modify question\n");
	printf("  3. Delete question\n");
	printf("  4. List questions\n");
	printf("  5. Finalise questionary\n");
	printf("  6. Final questions\n");
	printf("  7. exit\n\n");
}

void listQuestions()
{
	printf("Displaying question :\n\n");
	int i=0;
	
	for(i=0; i<questionCount; ++i)
	{
		dispQuestion(questions[i]);
	}
}

void addQuestion()
{
	struct question s;
	printf("Type Question: ");
	fgets(buff,QuestionSize,stdin);
	strcpy(buff,strtok(buff,"\n"));
	strcpy(s.question,buff);
	
	printf("Type optionA: ");
	fgets(buff,QuestionSize,stdin);
	strcpy(buff,strtok(buff,"\n"));
	strcpy(s.optA,buff);
	
	printf("Type optionB: ");
	fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	strcpy(s.optB,buff);

	do
	{
		printf("Add opton3?(Y/N):");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	}
	while(buff[0] != 'Y' && buff[0] != 'N');	
	
	if(buff[0] == 'Y')
	{
		printf("Type optionC: ");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
		strcpy(s.optC,buff);
		
		do
		{
			printf("Add opton4?(Y/N):");
			fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
		}
		while(buff[0] != 'Y' && buff[0] != 'N');
			
		if(buff[0] == 'Y')
		{
			printf("Type optionD: ");
			fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
			strcpy(s.optD,buff);
		}
		else
		{
			strcpy(s.optD,"NAN");
		}
	}
	else
	{	
		strcpy(s.optC,"NAN");
		strcpy(s.optD,"NAN");
	}
	
	printf("\n");
	
	s.id = questionCount;
	s.finalised=0;
		
	time(&timer);
		
    tm_info = localtime(&timer);

		
    strftime(buff, 26, "%Y-%m-%d %H:%M:%S", tm_info);
		
	strcpy(s.createdDate,buff);
		
	questions[questionCount] = s;
		
	++questionCount;
}

void alterQuestion()
{
	printf("Alter question\n\n");
	int d=0;
	do
	{
		printf("Give the id\n");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
		d=atoi(buff);
			
	}while( d <= 0|| d >= questionCount );
		
	dispQuestion(questions[d]);
	
	do
	{
		printf("Sure?(Y/N)\n");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	}
	while(buff[0] != 'Y' && buff[0] != 'N');
	
	if(buff[0] == 'N')
	{
		return;
	}
	
	struct question s;
	printf("Type Question: ");
	fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	strcpy(s.question,buff);
	
	printf("Type optionA: ");
	fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	strcpy(s.optA,buff);
	
	printf("Type optionB: ");
	fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	strcpy(s.optB,buff);

	do
	{
		printf("Add opton3?(Y/N):");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	}
	while(buff[0] != 'Y' && buff[0] != 'N');	
	
	if(buff[0] == 'Y')
	{
		printf("Type optionC: ");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
		strcpy(s.optC,buff);
		
		do
		{
			printf("Add opton4?(Y/N):");
			fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
		}
		while(buff[0] != 'Y' && buff[0] != 'N');
			
		if(buff[0] == 'Y')
		{
			printf("Type optionD: ");
			fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
			strcpy(s.optD,buff);
		}
		else
		{
			strcpy(s.optD,"NAN");
		}
	}
	else
	{	
		strcpy(s.optC,"NAN");
		strcpy(s.optD,"NAN");
	}
	
	printf("\n");
	
	s.id = questionCount;
	s.finalised=0;
		
	time(&timer);
		
    tm_info = localtime(&timer);

		
    strftime(buff, 26, "%Y-%m-%d %H:%M:%S", tm_info);
		
	strcpy(s.createdDate,buff);
		
	questions[d] = s;
	
}

void deleteQuestion()
{
	printf("Delete question\n\n");
	int d=0;
	do
	{
		printf("Give the id\n");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
		d=atoi(buff);
			
	}while( d <= 0|| d >= questionCount );
		
	dispQuestion(questions[d]);

	do
	{
		printf("Sure?(Y/N)\n");
		fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
	}
	while(buff[0] != 'Y' && buff[0] != 'N');
	
	if(buff[0] == 'N')
	{
		return;
	}
	
	int i=0;
	for(i=d;i<questionCount;++i)
	{
		questions[i] = questions[i+1];
		questions[i].id = i;
	}
	
	--questionCount;
}

void finaliseQuestions()
{
	printf("Finalise question :\n\n");
	int i=0;
	
	for(i=0; i<questionCount; ++i)
	{
		dispQuestion(questions[i]);
		
		do
		{
			printf("Do you want to finalise?(Y/N):");
			fgets(buff,QuestionSize,stdin); strcpy(buff,strtok(buff,"\n"));
		}
		while(buff[0] != 'Y' && buff[0] != 'N');
			
		if(buff[0] == 'Y')
		{
			questions[i].finalised=1;
		}
	}
	
	saveQuestionList();
}

void listFinalisedQuestions()
{
	printf("Displaying finalized questions:\n\n");
	int i=0;
	
	for(i=0; i<questionCount; ++i)
	{
		if(questions[i].finalised ==1)
		{
			dispQuestion(questions[i]);
		}
	}
}

void saveQuestionList()
{
	printf("Saving\n");
	
	qFile= fopen("q","w");
	fprintf(qFile,"%d\n",questionCount);
	int i=0;
	for(i=0;i<questionCount;++i)
	{
		fprintf(qFile,"%d\n",questions[i].finalised);
		fprintf(qFile,"%s\n",questions[i].question);
		fprintf(qFile,"%s\n",questions[i].optA);
		fprintf(qFile,"%s\n",questions[i].optB);
		fprintf(qFile,"%s\n",questions[i].optC);
		fprintf(qFile,"%s\n",questions[i].optD);
		fprintf(qFile,"%s\n",questions[i].createdDate);
	}
	fclose(qFile);
	printf("Saving Done\n\n");
}
































