#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define question_length 500
#define answer_length 100

#define filename "MLSZ_questions.txt"

struct question
{
    int id, finalized;
	char question[question_length], optionA[answer_length], optionB[answer_length], optionC[answer_length], optionD[answer_length], createdDate[60];
};

FILE *file;

int question_count;
char buff[question_length];
struct question questions[10000];

time_t timer;
struct tm* time_info;



void init()
{
	if(access(filename,F_OK) != -1)
	{
		printf("Loading...\n");
		
		file= fopen(filename,"r");
		fgets(buff, question_length, (FILE*)file);
		question_count = atoi(buff);
		
		
		struct question q;
		int i=0;
		for(i=0;i<question_count;++i)
		{
			q.id=i;
			fgets(buff, question_length, (FILE*)file);
			q.finalized=atoi(buff);

			fgets(buff, question_length, (FILE*)file);
			strcpy(q.question,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)file);
			strcpy(q.optionA,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)file);
			strcpy(q.optionB,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)file);
			strcpy(q.optionC,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)file);
			strcpy(q.optionD,strtok(buff,"\n"));
			
			fgets(buff, question_length, (FILE*)file);
			strcpy(q.createdDate,strtok(buff,"\n"));
			
			questions[i]=q;
		}
		
		fclose(file);
		printf("Loading Done\n\n");
	}
	else
	{
		file =fopen(filename,"w");
		fprintf(file,"0\n");
		fclose(file);
        question_count = 0;
	}
}

void show_question(struct question q)
{
		printf("ID: %d\n",q.id);
		if(q.finalized == 0)
		{
			printf("NOT finalized\n");
		}
		else
		{
			printf("finalized\n");
		}
		printf("Created date: %s \n\n",q.createdDate);
		printf("%s\n",q.question);
		printf("  A: %s\n",q.optionA);
		printf("  B: %s\n",q.optionB);
		if(strcmp(q.optionC,"NAN")!=0)
		{
			 printf("  C: %s\n",q.optionC);
		}
		if(strcmp(q.optionD,"NAN")!=0)
		{
			printf("  D: %s\n\n",q.optionD);
		}
		printf("\n\n");
}

void show_questions(bool finalized)
{
	if(finalized) {printf("Displaying finalized questions :\n\n");}
    else {printf("Displaying questions:\n\n");}
    int i;
	for(i=0; i < question_count; ++i)
	{
		if(finalized == false)
        {
            show_question(questions[i]);
        }
        else if(questions[i].finalized == 1)
        {
            show_question(questions[i]);
        }
	}
}

bool yes_or_no(char message[])
{
    do
	{
		printf(message);
		fgets(buff,question_length,stdin); strcpy(buff,strtok(buff,"\n"));
	} while(buff[0] != 'Y' && buff[0] != 'N' && buff[0] != 'y' && buff[0] != 'n');
    if(buff[0] == 'Y' || buff[0] == 'y')
    {
        return true;
    }
    return false;
}

int give_the_id()
{
    int d;
	do
	{
		printf("Give the id: ");
		fgets(buff,question_length,stdin); strcpy(buff,strtok(buff,"\n"));
		d=atoi(buff);
			
	} while( d < 0 || d >= question_count );
    return d;
}

struct question create_question()
{
    struct question s;
	printf("Type Question: ");
	fgets(buff,question_length,stdin);
	strcpy(buff,strtok(buff,"\n"));
	strcpy(s.question,buff);
	
	printf("Type optionA: ");
	fgets(buff,question_length,stdin);
	strcpy(buff,strtok(buff,"\n"));
	strcpy(s.optionA,buff);
	
	printf("Type optionB: ");
	fgets(buff,question_length,stdin); strcpy(buff,strtok(buff,"\n"));
	strcpy(s.optionB,buff);

	if(yes_or_no("Add option C?(Y/N):"))
	{
		printf("Type option C: ");
		fgets(buff,question_length,stdin); strcpy(buff,strtok(buff,"\n"));
		strcpy(s.optionC,buff);

		if(yes_or_no("Add option D?(Y/N):"))
		{
			printf("Type option D: ");
			fgets(buff,question_length,stdin); strcpy(buff,strtok(buff,"\n"));
			strcpy(s.optionD,buff);
		}
		else
		{
			strcpy(s.optionD,"NAN");
		}
	}
	else
	{	
		strcpy(s.optionC,"NAN");
		strcpy(s.optionD,"NAN");
	}

	printf("\n");

    s.finalized = 0;
    
    time(&timer);
    time_info = localtime(&timer);
    strftime(buff, 26, "%Y-%m-%d %H:%M:%S", time_info);
	strcpy(s.createdDate,buff);

    return s;
}

void add_question()
{
	struct question s = create_question();
	s.id = question_count;

    questions[question_count] = s;
	++question_count;
}

void modify_question()
{
	printf("Modify question\n\n");
	int d = give_the_id();
		
	show_question(questions[d]);

	if(yes_or_no("Sure?(Y/N): ") == false)
	{
		return;
	}
	
	struct question s = create_question();
	s.id = d;
		
	questions[d] = s;
}

void delete_question()
{
	printf("Delete question\n\n");
	int d = give_the_id();
		
	show_question(questions[d]);

	if(yes_or_no("Sure?(Y/N): ") == false)
	{
		return;
	}
	
	int i=0;
	for(i=d;i<question_count;++i)
	{
		questions[i] = questions[i+1];
		questions[i].id = i;
	}
	
	--question_count;
}

void finalise_questions()
{
	printf("Finalise questions\n\n");
	int i;
	for(i=0; i<question_count; ++i)
	{
		show_question(questions[i]);

		if(yes_or_no("Do you want to finalise?(Y/N):"))
		{
			questions[i].finalized = 1;
		}
	}
	
	save_changes();
}

void save_changes()
{
	printf("Saving...\n");
	
	file= fopen(filename,"w");
	fprintf(file,"%d\n",question_count);
	int i=0;
	for(i=0;i<question_count;++i)
	{
		fprintf(file,"%d\n",questions[i].finalized);
		fprintf(file,"%s\n",questions[i].question);
		fprintf(file,"%s\n",questions[i].optionA);
		fprintf(file,"%s\n",questions[i].optionB);
		fprintf(file,"%s\n",questions[i].optionC);
		fprintf(file,"%s\n",questions[i].optionD);
		fprintf(file,"%s\n",questions[i].createdDate);
	}
	fclose(file);
	printf("Saving Done\n\n");
}

void show_menu()
{
	
	printf("Main menu\n");
	printf("  1. Add new question\n");
	printf("  2. Modify question\n");
	printf("  3. Delete question\n");
	printf("  4. List questions\n");
	printf("  5. Finalise questionary\n");
	printf("  6. Final questions\n");
	printf("  7. Exit\n\n");
}

int main(int argc, char ** argv) 
{
    printf("Welcome to the MLSZ questionaire builder!\n\n");
    init();
	do 
	{
		show_menu();
		fgets(buff,question_length,stdin);
        printf("\n");
		switch (buff[0])
		{
			case '1': add_question(); save_changes(); break;
			case '2': modify_question(); save_changes(); break;
			case '3': delete_question(); save_changes(); break;
			case '4': show_questions(false); break;
			case '5': finalise_questions(); break;
			case '6': show_questions(true); break;
			case '7': printf("Goodbye!\n\n"); break;
			default: printf("Wrong command.. :(\n\n");
		}
	} while( buff[0] != '7');
	return 0;
}
