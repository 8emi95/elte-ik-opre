#ifndef SM_H
#define SM_H

#include "QuestionVector.h"
#include "stdio.h"

struct SurveyManager
{
	QuestionVector qv;
	FILE* f;
};

typedef struct SurveyManager SurveyManager;

void SMinit(SurveyManager* sm);
void SMfree(SurveyManager* sm);
void SMaddQuestion(SurveyManager* sm);
void SMmodifyQuestion(SurveyManager* sm);
void SMdeleteQuestion(SurveyManager* sm);
void SMprintQuestions(SurveyManager* sm);
void SMfinalize(SurveyManager* sm);

int getintc(int lowerBound, int upperBound);

#endif