#include "question_creation.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>		//fork
#include <sys/wait.h>	//waitpid
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

void handler(){	
	printf("Kerdesbiztos keszen all\n");
}

int main(int argc, char** argv)
{
	printf("\n*********************** Kerdoiv osszeallito program ***********************\n\n");
	
	//Kérdőív összeállítása
	struct Question questions[100];
	int numberOfQuestions = 0;
	
	int stayInCycle = 1;
	while (stayInCycle)
	{
		printMenu();
		char c;
		c = getchar(); getchar();
		printf("\n\n");
		switch (c)
		{
			case '1': addQuestion(questions,numberOfQuestions); numberOfQuestions++; break;
			case '2': modifyQuestion(questions, numberOfQuestions); break;
			case '3': deleteQuestion(questions,numberOfQuestions); numberOfQuestions--; break;
			case '4': listQuestions(questions,numberOfQuestions); break;
			case '5': stayInCycle = 0; break;
			default: printf("A fenti opciok valamelyiket kell valasztania!\n\n"); break;
		}
		//while ((c = getchar()) != '\n') { }			//Több mint egy karakter megadása esetén a fölöslegesek kiürítése
		
	}

	//Kérdezőbiztos kiküldése
	int parentChildPipe[2];		//Szülő írja
	int childParentPipe[2];		//Gyerek írja
	pipe(parentChildPipe);
	pipe(childParentPipe);
	signal(SIGTERM,handler);
	srand(time(NULL));
	
	pid_t process_id = fork();
	if (process_id<0){perror("A fork hivas sikertelen volt!\n"); exit(1);}
	
	if (process_id > 0)			//Szülő
	{
		close(parentChildPipe[0]);
		close(childParentPipe[1]);
		int i;		
		
		//3 kérdés kiválasztása
		int questionsToSend[3];
		questionsToSend[0] = rand() % numberOfQuestions;
		questionsToSend[1] = rand() % numberOfQuestions;
		questionsToSend[2] = rand() % numberOfQuestions;
		while(questionsToSend[0]==questionsToSend[1] || questionsToSend[1]==questionsToSend[2] || questionsToSend[2]==questionsToSend[0])
		{
			if (questionsToSend[0]==questionsToSend[1]) questionsToSend[1] = rand() % numberOfQuestions;
			if (questionsToSend[1]==questionsToSend[2]) questionsToSend[1] = rand() % numberOfQuestions;
			if (questionsToSend[2]==questionsToSend[0]) questionsToSend[0] = rand() % numberOfQuestions;
		}		

		pause();
		//Kérdések sorszámának átküldése
		write(parentChildPipe[1],&questionsToSend[0],sizeof(int));
		write(parentChildPipe[1],&questionsToSend[1],sizeof(int));
		write(parentChildPipe[1],&questionsToSend[2],sizeof(int));
		
		printf("A kisorsolt kerdesek: ");
		for (i=0;i<3;++i)
		{
			printf("%d. ",questionsToSend[i]);
		} 
		
		//Válaszok fogadása, kiírása
		int numberOfParticipants;
		read(childParentPipe[0],&numberOfParticipants,sizeof(int));
		printf("\n%d ember vett reszt a kitoltesben\n\n",numberOfParticipants);
		printf("***** A kitoltott kerdoivek *****\n");
		char questionText[80];
		char answer[40];
		for (i=0;i<numberOfParticipants;++i)
		{
			int j;
			printf("\n%d. kitolto valaszai:\n\n",i+1);
			for (j=0;j<3;++j)
			{
				read(childParentPipe[0],questionText,sizeof(char)*80);
				read(childParentPipe[0],answer,sizeof(char)*40);
				printf("%s %s\n",questionText,answer);
			}
		}
		waitpid(process_id,NULL,0);
	}
	else						//Gyerek
	{
		if (argc == 2) printf("\n\nA kerdesbiztos megerkezett %s varosba\n\n",argv[1]);
		close(parentChildPipe[1]);
		close(childParentPipe[0]);
		sleep(2);
		kill(getppid(),SIGTERM);

		//3 kérdéssorszám kiolvasása a csőből
		int selectedQuestions[3];
		read(parentChildPipe[0],&selectedQuestions[0],sizeof(int));
		read(parentChildPipe[0],&selectedQuestions[1],sizeof(int));
		read(parentChildPipe[0],&selectedQuestions[2],sizeof(int));

		int i;
		
		//Kérdőívek kitöltése
		int numberOfParticipants = 10 + (rand() % 11);
		write(childParentPipe[1],&numberOfParticipants,sizeof(int));
		int selectedAnswer;
		for (i=0;i<numberOfParticipants;++i)
		{
			int j;
			for (j=0;j<3;++j)
			{
				selectedAnswer = rand() % questions[selectedQuestions[j]].numberOfAnswers;
				write(childParentPipe[1],questions[selectedQuestions[j]].text,sizeof(char)*80);
				write(childParentPipe[1],questions[selectedQuestions[j]].answers[selectedAnswer],sizeof(char)*40);
			}
		}
		
	}

	return 0;
}
