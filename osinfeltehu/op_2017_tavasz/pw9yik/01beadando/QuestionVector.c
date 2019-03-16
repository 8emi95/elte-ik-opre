#include "QuestionVector.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

void Qprint(Question *q)
{
	printf("%s\n", q->question);
	int j;
	for(j = 0; j < 4; j++)
	{
		if(strstr(q->answers[j], "<empty>") == NULL)
			printf("%i. %s\n", j + 1, q->answers[j]);
	}
	char buff[70];
    strftime(buff, sizeof buff, "%d/%m/%Y, %H:%M:%S", q->date);
	printf("Last modified: %s\n", buff);
	printf("\n");
}

void QaddAnswer(Question *q, char* str)
{
	if(q->answerCount < 4)
	{
		strcpy(q->answers[q->answerCount], str);
		q->answerCount++;
	}
	else
	{
		printf("The question already has 4 answers, you cant add more.\n");
	}
}

void QremoveAnswer(Question *q, int pos)
{
	if(q->answerCount > 2)
	{
		strcpy(q->answers[pos], q->answers[q->answerCount - 1]);
		strcpy(q->answers[q->answerCount - 1], "<empty>");
		q->answerCount--;
	}
	else
	{
		printf("The question needs 2 answers at least.");
	}
}

int QgetAnswerCount(Question *q)
{
	return q->answerCount;
}

void QVinit(QuestionVector* qv)
{
	qv->actualSize = 0;
	qv->maxSize = BASE_QV_SIZE;
	qv->ptr = (Question*)malloc(BASE_QV_SIZE * sizeof(Question));
}

void Qcpy(Question* lhs, Question *rhs)
{
	strcpy(lhs->question, rhs->question);
	int i;
	for (i = 0; i<4; i++)
	{
		strcpy(lhs->answers[i], rhs->answers[i]);
	}
	lhs->answerCount = rhs->answerCount;
	lhs->date = rhs->date;
}

void QVpush_back(QuestionVector* qv, Question *q)
{
	if(qv->actualSize < qv->maxSize)
	{
		Qcpy(&qv->ptr[qv->actualSize], q);
	}
	else
	{
		qv->maxSize *= 2;
		Question* tmp = qv->ptr;
		qv->ptr = (Question*)malloc(qv->maxSize * sizeof(Question));
		int i;
		for(i = 0; i < qv->actualSize; i++)
		{
			qv->ptr[i] = tmp[i];
		}
		Qcpy(&qv->ptr[qv->actualSize], q);
		free(tmp);
	}
	
	qv->actualSize++;
}

void Qswap(Question* lhs, Question* rhs)
{
	Question tmp;
	Qcpy(&tmp, lhs);
	Qcpy(lhs, rhs);
	Qcpy(rhs, &tmp);
}

void QVremove(QuestionVector* qv, int pos)
{
	if (qv->actualSize == 0) return;
	int i;
	for (i  = pos; i < qv->actualSize - 1; i++)
	{
		Qswap(&qv->ptr[i], &qv->ptr[i+1]);
	}
	qv->actualSize--;
}
int QVsize(QuestionVector* qv)
{
	return qv->actualSize;
}
Question* QVat(QuestionVector* qv, int pos)
{
	return &qv->ptr[pos];
}
void QVfree(QuestionVector* qv)
{
	free(qv->ptr);
}