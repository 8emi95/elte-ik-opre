#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Questions
{
	char question[100];
	char answerA[100];
	char answerB[100];
	char answerC[100];
	char answerD[100];
	char signUptime[20];

} Questions;


static int questionCount = 0;
static int finalCount = 0;
Questions questionDatabase[1000];
Questions finalDatabase[1000];

void Open_Database()
{
	FILE * fQuestion;
	 fQuestion=fopen("question.dat","rb");
	 if (fQuestion!=NULL)
	 {
	 	int i=0;
	 	while (!feof(fQuestion))
	 	{
	 		fread(&questionDatabase[i],sizeof(questionDatabase[i]),sizeof(questionDatabase[i]),fQuestion);
	 		++i;
	 	}
	 questionCount = i-1;
	}
	else {perror("File opening error\n"); exit(1);}
	fclose(fQuestion);

	FILE* fFinal;
	 fFinal=fopen("final.dat","rb");
	 if (fFinal!=NULL)
	 {
	 	int i=0;
	 	while (!feof(fFinal))
	 	{
	 		fread(&finalDatabase[i],sizeof(finalDatabase[i]),sizeof(finalDatabase[i]),fFinal);
	 		++i;
	 	}
	 finalCount = i-1;
	}
	else {perror("File opening error\n"); exit(1);}
	fclose(fFinal);
}

 void Write_Database()
 {
 	FILE * fQuestion;
 	fQuestion=fopen("question.dat","wb");
 	if(fQuestion==NULL){perror("File opening error\n"); exit(1);}
 	for(int i=0; i<questionCount; ++i)
 	{
 		fwrite(&questionDatabase[i],sizeof(questionDatabase[i]),sizeof(questionDatabase[i]),fQuestion);
 	}
 	fclose(fQuestion);

 	FILE* fFinal;
 	 fFinal=fopen("final.dat","wb");
 	if(fFinal==NULL){perror("File opening error\n"); exit(1);}
 	for(int i=0; i<finalCount; ++i)
 	{
 		fwrite(&finalDatabase[i],sizeof(finalDatabase[i]),sizeof(finalDatabase[i]),fFinal);
 	}
 	fclose(fFinal);
 }

void SignUpTime(char _time[20])
{
	time_t current_time;
	struct tm * time_info;

	time(&current_time);
	time_info = localtime(&current_time);

	strftime(_time,20,"%Y.%m.%d %H:%M:%S", time_info);
}

int Getline(char line[], int max)
{
	int nch = 0;
	int c;
	max = max - 1;
	while((c = getchar()) != EOF)
	{
		if(nch!=0)
		{
			if(c == '\n')
				break;
			if(nch < max)
			{
				line[nch-1] = c;
				nch = nch + 1;
			}
		}
		else
			nch++;
	}
	if(c == EOF && nch == 0)
		return EOF;
	line[nch-1] = '\0';
	return nch;
}

void ListFinal()
{
	printf("\n********************************\nFinal Questions \n********************************\n");
	for(int i =0; i<finalCount; ++i)
	{
		printf("%d. Question: %s. A: %s. B: %s. C: %s. D: %s\n",i+1,&(finalDatabase[i].question),finalDatabase[i].answerA,finalDatabase[i].answerB, finalDatabase[i].answerC, finalDatabase[i].answerD, finalDatabase[i].signUptime);
	}
}

void NewQuestion()
{
	Questions question;
	int c = 0;
	char tmp[100] = " \t";
	do{
		printf("How many answer option?");
		scanf("%d",&c);
		//printf(c);
	}while (c<2 || c>4);
	printf("Enter the question: \n");
	printf("Question: ");
	Getline(question.question,50);
	printf("Answer A: ");
	scanf("%s",question.answerA);
	printf("Answer B: ");
	scanf("%s",question.answerB);
	if (c>2){
		printf("Answer C: ");
		scanf("%s",question.answerC);
	}
	if(c>3){
		printf("Answer D: ");
		scanf("%s",question.answerD);
	}
	SignUpTime(question.signUptime);
	questionCount++;
	questionDatabase[questionCount-1] = question;
	switch(questionCount){
		case 1: printf("This is the 1st Question!\n"); break;
		case 2: printf("This is the 2nd Question!\n"); break;
		case 3: printf("This is the 3rd Question!\n"); break;
		default: printf("This is the %dth Question",questionCount); break;
	}
}

void ListQuestion()
{
	printf("\n********************************\nQuestions \n********************************\n");
	for(int i =0; i<questionCount; ++i)
	{
		printf("%d. Question: %s. A: %s. B: %s. C: %s. D: %s. Datum %s \n",i+1,&(questionDatabase[i].question), questionDatabase[i].answerA, questionDatabase[i].answerB, questionDatabase[i].answerC, questionDatabase[i].answerD, questionDatabase[i].signUptime);
	}
}

void SubMenuRun(int index)
{
	int c=0;
	printf("Which field would you like to modify?:\n");
	printf(" 1. -Question\n");
	printf(" 2. -A \n");
	printf(" 3. -B \n");
	printf(" 4. -C \n");
	printf(" 5. -D \n");
	printf(" 0. -Cancel\n");
	scanf("%d",&c);
	switch(c){
		case 1: printf("Question: "); scanf("%s",&(questionDatabase[index].question)); printf("modyfied to: %s\n",&(questionDatabase[index].question)); break;
		case 2: printf("A: "); scanf("%s",&(questionDatabase[index].answerA)); printf("modyfied to: %s\n",&(questionDatabase[index].answerA)); break;
		case 3: printf("B: "); scanf("%s",&(questionDatabase[index].answerB)); printf("modyfied to: %s\n",&(questionDatabase[index].answerB)); break;
		case 4: printf("C: "); scanf("%s",&(questionDatabase[index].answerC)); printf("modyfied to: %s\n",&(questionDatabase[index].answerC)); break;
		case 5: printf("D: "); scanf("%s",&(questionDatabase[index].answerD)); printf("modyfied to: %s\n",&(questionDatabase[index].answerD)); break;
		case 0: break;	
	}
}

void ModifyQuestion()
{
	int c=0;
	ListQuestion();
	printf("Whose datum would you like to modify?: ");
	scanf("%d",&c);
	if(c<=questionCount)
	{
		SubMenuRun(c-1);
		SignUpTime(questionDatabase[c-1].signUptime);	
	}
}

void DeletQuestion()
{
	int c=0;
	ListQuestion();
	printf("What do you want to delete?: ");
	scanf("%d",&c);
	if(c<=questionCount)
	{
		for(int i =c - 1 ; i <questionCount-1 ; ++i )
		{
			questionDatabase[i] = questionDatabase[i+1];
		}
		--questionCount;
	}

}

void finalizeQuestion()
{
	int c=0;
	ListQuestion();
	printf("What do you want to finalize?: ");
	scanf("%d",&c);
	finalDatabase[finalCount]=questionDatabase[c-1];
	finalCount++;
	if(c<=questionCount)
	{
		for(int i =c - 1 ; i <questionCount-1 ; ++i )
		{
			questionDatabase[i] = questionDatabase[i+1];
		}
		--questionCount;
	}
}


void MenuWrite() 
{
	printf("\n********************************\n MLSZ Database \n********************************\n");
	printf("\n");
	printf(" 1. -New Question\n");
	printf(" 2. -List Questions\n");
	printf(" 3. -Modify question\n"); 
	printf(" 4. -Delete Question\n");
	printf(" 5. -Finalize Question\n");
	printf(" 6. -List Final Questions\n");
	printf(" 0. -Exit\n");
}

void Run() 
{
	int c = 0; 
	do{
		MenuWrite(); 
		scanf("%d",&c); 
		switch(c){
			case 1: NewQuestion(); break;
			case 2: ListQuestion(); break; 
			case 3: ModifyQuestion(); break; 
			case 4: DeletQuestion(); break;
			case 5: finalizeQuestion(); break;
			case 6: ListFinal(); break;
		} 
	}while(c!=0);
}

int main()
{
	Open_Database();
	Run();
	Write_Database();
	return 0;
}