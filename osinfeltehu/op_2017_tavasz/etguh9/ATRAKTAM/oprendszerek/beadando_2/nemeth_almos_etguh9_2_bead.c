/*
HASZNÁLAT:

Az előző beadandó ('nemeth_almos_etguh9_1_bead.c') segítségével készítsük el a kérdéssort. Ezután futtassuk ezt a programot. Amennyiben a program mappájában megtalálható az előző beadandó által létrehozott 'data' nevű fájl és abban megfelelő számú (tehát min. 3) kérdés van, a program elvégzi a kívánt tevékenységet. Ellenkező esetekben hibaüzenetet ír a kontolra és befejezi a futást.
*/




#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>




#define MAX_QUESTION_SIZE 200
#define MAX_ANSWER_SIZE 80




//Question struct:

struct Question
{
	char question[MAX_QUESTION_SIZE];
	int answers_Count;
	char answers[4][MAX_ANSWER_SIZE];
	size_t date;
};




//global variables:

struct Question* questions;
int questions_Count;




//function declarations:

void load_Questions();
void sighandler();




//main function:

int main(int argc,char** argv)
{
	if (argc != 2)
	{
		printf("Hiba! A program pontosan egy argumentummal hívható!\n");
		exit(1);
	}
	
	printf("\nMLSZ KONZULTACIO\n\n\n");
	
	srand(time(NULL));
	
	load_Questions();
	
	if (questions_Count < 3)
	{
		printf("Hiba! Tul keves kerdes van a fajlban!\n");
		exit(1);
	}
	else
	{
		signal(SIGUSR1, sighandler);

		sigset_t sig_Set;
		sigemptyset(&sig_Set);
		sigaddset(&sig_Set, SIGUSR1);
				
		sigprocmask(SIG_BLOCK, &sig_Set, NULL);
	
	
		
		int pipe_FD[2];	
		pid_t pid;
		
		if (pipe(pipe_FD) == -1)
		{
			printf("Hiba a pipe nyitaskor!\n");
			exit(1);
		}
		
		pid = fork();
	
		if (pid == -1)
		{
			printf("Hiba a forkolaskor!\n");
			exit(1);
		}
		if (pid == 0)
		{
			kill(getppid(), SIGUSR1);
			
			
			
			sigfillset(&sig_Set);
			sigdelset(&sig_Set, SIGUSR1);
			sigsuspend(&sig_Set);
			
			sigprocmask(SIG_UNBLOCK, &sig_Set, NULL);
			
			
			
			struct Question recieved_Questions[3];
			
			read(pipe_FD[0], &recieved_Questions, sizeof(recieved_Questions));
			
			close(pipe_FD[0]);
			
			
			
			int citizens_Count = rand() % 11 + 10;
			write(pipe_FD[1], &citizens_Count, sizeof(int));
			
			int i;
			for (i = 0; i < 3; ++i)
			{
				int answers_Count = recieved_Questions[i].answers_Count;
				int j;
				for (j = 0; j < citizens_Count; ++j)
				{
					int answer = rand() % answers_Count;
					write(pipe_FD[1], &answer, sizeof(int));
				}
			}
			
			close(pipe_FD[1]);
			
			kill(getppid(), SIGUSR1);
		} 
		else 
		{
			sigfillset(&sig_Set);
			sigdelset(&sig_Set, SIGUSR1);
			sigsuspend(&sig_Set);
			
			sigprocmask(SIG_UNBLOCK, &sig_Set, NULL);
			
			
			
			int r1 = rand() % questions_Count;
			int r2 = rand() % (questions_Count - 1);
			int r3 = rand() % (questions_Count - 2);
			if (r2 >= r1)
			{
				++r2;
				if (r3 >= r1)
				{
					++r3;
					if (r3 >= r2)
					{
						++r3;
					}
				}
			}
			else
			{
				if (r3 >= r2)
				{
					++r3;
					if (r3 >= r1)
					{
						++r3;
					}
				}
			}
			
			write(pipe_FD[1], &questions[r1], sizeof(struct Question));
			write(pipe_FD[1], &questions[r2], sizeof(struct Question));
			write(pipe_FD[1], &questions[r3], sizeof(struct Question));
			
			close(pipe_FD[1]);
			
			kill(pid, SIGUSR1);
			
			
			
			sigsuspend(&sig_Set);
			
			int number_Of_Citizens;
			read(pipe_FD[0], &number_Of_Citizens, sizeof(int));
			
			printf("A %s-i polgárok véleménye a labdajátékokról:\n\n\n", argv[1]);
			
			int r[3] = {r1, r2, r3};
			int i;
			for (i = 0; i < 3; ++i)
			{
				printf("%i. kérdés:\n\n%s\n\nVálaszok:\n", i+1, questions[r[i]].question);
				int j;
				int answers_Count = questions[r[i]].answers_Count;
				int *cumulated_Answers = (int *)malloc(answers_Count * sizeof(int));
				for (j = 0; j < answers_Count; ++j)
				{
					cumulated_Answers[j] = 0;
				}
				for (j = 0; j < number_Of_Citizens; ++j)
				{
					int answer;
					read(pipe_FD[0], &answer, sizeof(int));
					++cumulated_Answers[answer];
				}
				for (j = 0; j < answers_Count; ++j)
				{
					printf("%s  (%i ember)\n", questions[r[i]].answers[j], cumulated_Answers[j]);
				}
				free(cumulated_Answers);
				printf("\n\n");
			}
			
			close(pipe_FD[0]);
			
			
			
			int status;
			waitpid(pid, &status, 0);
		}
	}
	
	free(questions);
	
	return 0;
}




//function definitions:

void sighandler() {}


void load_Questions()
{
	questions_Count = 0;

	int f = open("data", O_RDONLY);
	if (f != -1)
	{
		read(f, &questions_Count, sizeof(int));
		questions = (struct Question*) malloc(questions_Count * sizeof(struct Question));
	
		int i;
		for (i = 0; i < questions_Count; ++i)
		{
			read(f, &(questions[i]), sizeof(struct Question));
		}

		close(f);
	}
	else
	{
		printf("Hiba! Nem sikerult megnyitni a kerdeseket tartalmazo fajlt\n");
		exit(1);
	}
}