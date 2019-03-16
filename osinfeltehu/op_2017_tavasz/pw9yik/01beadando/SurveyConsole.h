#ifndef SV_H
#define SV_H

#include "SurveyManager.h"

struct SurveyConsole
{
	SurveyManager sm;
};

typedef struct SurveyConsole SurveyConsole;

void SCrun(SurveyConsole* sc);

#endif