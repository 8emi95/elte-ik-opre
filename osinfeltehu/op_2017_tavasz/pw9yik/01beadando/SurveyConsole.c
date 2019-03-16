#include "SurveyConsole.h"
#include "utility.h"

void SCrun(SurveyConsole* sc)
{
	int exit = 0;
	SMinit(&sc->sm);
	
	while(exit == 0)
	{
		printf("Please choose an option.\n");
		printf("1. Add a new question\n");
		printf("2. Modify an existing question\n");
		printf("3. Remove a question\n");
		printf("4. Finalize survey\n");
		printf("5. Display survey\n");
		printf("6. Exit\n");
		
		int answer = getintc(1,6);
		printf("\n");
		switch(answer)
		{
			case 1:
				SMaddQuestion(&sc->sm);
				break;
			case 2:
				SMmodifyQuestion(&sc->sm);
				break;
			case 3:
				SMdeleteQuestion(&sc->sm);
				break;
			case 4:
				SMfinalize(&sc->sm);
				break;
			case 5:
				SMprintQuestions(&sc->sm);
				break;
			case 6:
				exit = 1;
				break;
		}
	}
	
	SMfree(&sc->sm);
}