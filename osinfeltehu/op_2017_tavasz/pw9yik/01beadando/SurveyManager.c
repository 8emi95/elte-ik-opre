#include "SurveyManager.h"
#include "stdio.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "utility.h"

void SMinit(SurveyManager* sm)
{
	QVinit(&sm->qv);
	sm->f = fopen("database.txt", "r");
	if (sm->f == NULL)
	{
		return;
	}
	char line[160];
	while (!feof(sm->f))
	{
		Question q;
		q.answerCount = 0;
		fgets(line, sizeof(line), sm->f);
		if(!feof(sm->f))
		{
			strcpy(q.question, line);
			strtok(q.question, "\n");
			int i;
			int fill = 0;
			for(i = 0; i<4; i++)
			{
				fgets(line, sizeof(line), sm->f);
				if(strstr(line, "<empty>") == NULL)
				{
					strcpy(q.answers[i], line);
					strtok(q.answers[i], "\n");
					fill++;
					q.answerCount++;
				}
			}
			for(; fill<4; fill++)
			{
				strcpy(q.answers[fill], "<empty>");
			}
			fgets(line, sizeof(line), sm->f);
			time_t tmp = toTime_t(line);
			q.date = malloc(sizeof(struct tm));
			*q.date = *localtime(&tmp);
			fgets(line, sizeof(line), sm->f); // ignores another line
			QVpush_back(&sm->qv, &q);
		}
	}
}
void SMfree(SurveyManager* sm)
{
	if(sm->f != NULL)
		sm->f = freopen("database.txt", "w", sm->f);
	else
		sm->f = fopen("database.txt", "w");
	int i;
	for(i = 0; i < QVsize(&sm->qv); i++)
	{
		fputs(QVat(&sm->qv, i)->question, sm->f);
		fputs("\n", sm->f);
		int j;
		for(j = 0; j < 4; j++)
		{
			fputs(QVat(&sm->qv, i)->answers[j], sm->f);
			fputs("\n", sm->f);
		}
		char buff[70];
		strftime(buff, sizeof buff, "%d/%m/%Y, %H:%M:%S", QVat(&sm->qv, i)->date);
		fputs(buff, sm->f);
		free(QVat(&sm->qv, i)->date);
		fputs("\n\n", sm->f);
	}
	fclose(sm->f);
	QVfree(&sm->qv);
}

void SMaddQuestion(SurveyManager* sm)
{
	Question q;
	q.answerCount = 0;
	printf("Please type in your question.\n\n");
	
	char * tmp = getlinec();
	strcpy(q.question, tmp);
	free(tmp);
	
	printf("\nPlease type in your answers one by one. Remember, that you need a minimum of two,\nand a maximum of four. Type \"exit\" if you are done.\n\n");
	int i;
	tmp = getlinec();
	for(i = 0; i < 4; i++)
	{
		if (strcmp(tmp, "exit") != 0)
		{
			strcpy(q.answers[i], tmp);
			free(tmp);
			tmp =  getlinec();
			q.answerCount++;
		}
		else if (q.answerCount < 2)
		{
			printf("You need at least 2 answers per question.\n");
			i--;
			free(tmp);
			tmp =  getlinec();
		}
		else
			strcpy(q.answers[i], "<empty>");
	}
	free(tmp);
	time_t curTime = time(NULL);
	q.date = malloc(sizeof(struct tm));
	*q.date = *localtime(&curTime);
	QVpush_back(&sm->qv, &q);
	printf("\n");
}

int pickQuestion(SurveyManager* sm)
{
	SMprintQuestions(sm);
	printf("Please select a question from the list above.\n");
	return getintc(1, QVsize(&sm->qv));
}
int pickAnswer(Question* q)
{
	printf("Please select an answer from the list above.\n");
	return getintc(1, QgetAnswerCount(q));
}

void SMmodifyQuestion(SurveyManager* sm)
{
	Question* picked = QVat(&sm->qv, pickQuestion(sm) - 1);
	printf("The question you picked:\n\n");
	Qprint(picked);
	printf("\nSelect between the options:\n1. Modify question\n2. Modify answer\n3. Add answer\n");
	int answer = getintc(1, 3);
	switch(answer)
	{
		case 1:
			printf("Enter the new answer.\n");
			strcpy(picked->question, getlinec());
			strtok(picked->question, "\n");
			break;
		case 2:
			answer = pickAnswer(picked) - 1;
			printf("Enter the modified answer.\n");
			strcpy(picked->answers[answer], getlinec());
			strtok(picked->answers[answer], "\n");
			break;
		case 3:
			printf("Enter the new answer.\n");
			QaddAnswer(picked, getlinec());
			strtok(picked->answers[picked->answerCount - 1], "\n");
			break;
	}
	time_t curTime = time(NULL);
	*picked->date = *localtime(&curTime);
}
void SMdeleteQuestion(SurveyManager* sm)
{
	int pickedIndex = pickQuestion(sm) - 1;
	Question* picked = QVat(&sm->qv, pickedIndex);
	printf("The question you picked:\n");
	Qprint(picked);
	printf("\nSelect between the options:\n1. Delete question\n2. Delete answer\n");
	int answer = getintc(1, 3);
	switch(answer)
	{
		case 1:
			QVremove(&sm->qv, pickedIndex);
			break;
		case 2:
			answer = pickAnswer(picked) - 1;
			QremoveAnswer(picked, answer);
			time_t curTime = time(NULL);
			*picked->date = *localtime(&curTime);
			break;
	}
}
void SMprintQuestions(SurveyManager* sm)
{
	int i;
	for(i = 0; i < QVsize(&sm->qv); i++)
	{
		printf("(%i) ", i + 1);
		Qprint(QVat(&sm->qv, i));
	}
}
void SMfinalize(SurveyManager* sm)
{
	printf("\nPlease finalize that which questions will remain in your survey.\n\n");
	int i;
	for(i = 0; i < QVsize(&sm->qv); i++)
	{
		printf("Should this question remain in your survey? y/n\n\n");
		Qprint(QVat(&sm->qv, i));
		char *tmp = getlinec();
		printf("\n");
		if(strcmp(tmp, "y") != 0 && strcmp(tmp, "Y") != 0)
		{
			QVremove(&sm->qv, i);
			printf("%i. %s\n",i + 1, tmp);
			i--;
		}
		free(tmp);
	}
}