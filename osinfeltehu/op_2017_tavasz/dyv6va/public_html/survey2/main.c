#include <time.h>
#include <signal.h>
#include "controller.h"
#define CITY_LENGTH 200
#define MIN_PEOPLE 10
#define MAX_PEOPLE 20
#define ERR_ARG "Please pass the name of a city as an argument"
#define ERR_WRITING "Error writing to pipe"
#define ERR_READING "Error reading from pipe"
#define ERR_PIPE "Error opening pipe"
#define ERR_PROC "Error creating process"
#define proc_owner "MLSZ"
#define proc_owner2 "Administrator"

/*
Qian L�via
DYV6VA

A Magyar Labdaj�t�kok Szervezete (MLSZ) nemzeti konzult�ci�t hirdetett a labdaj�t�kok felvir�goztat�s�ra! Els� l�p�sk�nt megalkotjuk azt a k�rd�ssort �s hozz� k�rd�senk�nt azt a minimum kett�, maximum 4 v�laszt�si lehet�s�get amelyet majd a labdaj�t�kok jobb�t�sa ir�nt felel�ss�get �rz�k kit�lthetnek. Miut�n v�gleges�tett�k a k�rd�seket az MLSZ (sz�l�) k�rdez�biztost(gyerek) k�ld a param�terk�nt megadott labdaj�t�k centrumba, v�rosba.

Az MLSZ v�letlenszer�en kiv�laszt 3 k�rd�st, amit cs�vezet�ken kik�ld a k�rdez�biztosnak azut�n, hogy az bejelentkezett az MLSZ-hez, hogy k�szen �ll a feladat v�grehajt�s�ra. A k�rdez�biztos v�letlenszer�en v�laszt 10 �s 20 k�zti konzult�lni k�v�n� polg�rt, akik kit�ltik a k�rd��vet (v�letlenszer�en). Az eredm�nyt visszak�ldi cs�vezet�ken az MLSZ-be, majd a k�rdez�biztos befejezi a konzult�ci�t. Az MLSZ fogadja az eredm�nyt �s a k�perny�re �rja a konzult�ci� eredm�ny�t.

K�sz�ts�nk C programot, amellyel a le�rt konzult�ci� elv�gezhet�.
*/

void handler(int signumber)
{

}

int main(int argc, char* argv[])
{
	controller c;
	pid_t child;
	int status;
	int pipefd[2]; // unnamed pipe file descriptor array
	char city[CITY_LENGTH];
	char city2[CITY_LENGTH]; // char array for reading from the pipe
	question_arr questionnaire;
	int results[NUM_NEEDED][MAX_PEOPLE]; // Results
	char line[LENGTH]; // char array for writing to the pipe
	
	if (argc != 2)
	{
		perror(ERR_ARG);
		exit(1);
	}
	
	if (pipe(pipefd) == -1)
    {
        perror(ERR_PIPE);
        exit(EXIT_FAILURE);
    }
	
	signal(SIGRTMIN, handler);
	c_init(&c);
	qa_init(&questionnaire, NUM_NEEDED);
	child = fork();
	
	if (child < 0)
	{
		perror(ERR_PROC);
		exit(1);
	}
	else if (child > 0) // parent
	{
		strcpy(city, argv[1]);
		printf("%s: The name of the city is: %s.\n", proc_owner, city);
		if (write(pipefd[1], &city, sizeof(city)) < 0) // Send the name of the city
		{
			perror(ERR_WRITING);
			exit(EXIT_FAILURE);
		}
		pause(); // Waiting for the child to arrive
		
		// Select some questions
		select_questions(&c);
		printf("%s: The selected questions are:\n", proc_owner);
		print_questions(&c);
		int i, j;
		kill(child, SIGRTMIN);
		
		// Send the questionnaire
		for (i = 0; i < NUM_NEEDED; i++)
		{
			strcpy(line, get_ques(at(&c.selected, i)));
			if (write(pipefd[1], &line, sizeof(line)) < 0)
			{
				perror(ERR_WRITING);
				exit(EXIT_FAILURE);
			}
			pause();
			for (j = 0; j < MAX_ANS; j++)
			{
				strcpy(line, get_ans(at(&c.selected, i), j));
				if (write(pipefd[1], &line, sizeof(line)) < 0)
				{
					perror(ERR_WRITING);
					exit(EXIT_FAILURE);
				}
				pause();
			}
			int count = get_answer_size(at(&c.selected, i));
			if (write(pipefd[1], &count, sizeof(count)) < 0)
			{
				perror(ERR_WRITING);
				exit(EXIT_FAILURE);
			}
			pause();
		}
		printf("%s: Questionnaire sent.\n", proc_owner);
		
		int num; // Number of people who will participate in the survey
		kill(child, SIGRTMIN);
		
		waitpid(child, &status, 0);
		// Child process has ended
		
		if (read(pipefd[0], &num, sizeof(num)) < 0)
		{
			perror(ERR_READING);
			exit(EXIT_FAILURE);
		}
		
		printf("%s: I see that %i people took the survey.\n", proc_owner, num);
		
		for (i = 0; i < NUM_NEEDED; i++)
		{
			for (j = 0; j < num; j++)
			{
				if (read(pipefd[0], &results[i][j], sizeof(results[i][j])) < 0)
				{
					perror(ERR_READING);
					exit(EXIT_FAILURE);
				}
			}
		}
		
		printf("%s: I have received the results of the survey.\n", proc_owner);
		
		// Print the results
		printf("%s: The results are:\n", proc_owner);
		
		for (i = 0; i < NUM_NEEDED; i++)
		{
			printf("Question #%i:\n", i + 1);
			for (j = 0; j < num; j++)
			{
				printf("%i ", results[i][j]);
			}
			printf("\n\n");
		}
		
		close(pipefd[1]);
		close(pipefd[0]);
	}
	else // child
	{
		int i, j;
		if (read(pipefd[0], &city2, sizeof(city2)) < 0)
		{
			perror(ERR_READING);
			exit(EXIT_FAILURE);
		}
		printf("%s: I received the location.\n", proc_owner2);

		// The child process sends a signal to the parent
		printf("%s: I arrived in %s.\n", proc_owner2, city2);
		kill(getppid(), SIGRTMIN);
		pause();
		
		// Receiving the questionnaire
		for (i = 0; i < NUM_NEEDED; i++)
		{
			question q;
			if (read(pipefd[0], &q.ques, sizeof(q.ques)) < 0)
			{
				perror(ERR_READING);
				exit(EXIT_FAILURE);
			}
			kill(getppid(), SIGRTMIN);
			for (j = 0; j < MAX_ANS; j++)
			{
				if (read(pipefd[0], &q.answers[j], sizeof(q.answers[j])) < 0)
				{
					perror(ERR_READING);
					exit(EXIT_FAILURE);
				}
				kill(getppid(), SIGRTMIN);
			}
			if (read(pipefd[0], &q.count, sizeof(q.count)) < 0)
			{
				perror(ERR_READING);
				exit(EXIT_FAILURE);
			}
			kill(getppid(), SIGRTMIN);
			new_question(&questionnaire, &q);
		}
		pause();
		printf("%s: I have received the questionnaire.\n", proc_owner2);
		print_all(&questionnaire);
		
		// Survey time
		srand(time(NULL));
		int num2 = rand() % (MAX_PEOPLE + 1 - MIN_PEOPLE) + MIN_PEOPLE; // Number of people to be asked
		printf("%s: %i people are going to be asked.\n", proc_owner2, num2);
		if (write(pipefd[1], &num2, sizeof(num2)) < 0)
		{
			perror(ERR_WRITING);
			exit(EXIT_FAILURE);
		}
		
		printf("%s: These are the results:\n", proc_owner2);
		
		// Random generated results
		int results2 [NUM_NEEDED][num2];
		for (i = 0; i < NUM_NEEDED; i++)
		{
			for (j = 0; j < num2; j++)
			{
				results2[i][j] = rand() % (MAX_ANS) + 1;
				printf("%i ", results2[i][j]);
			}
			printf("\n");
		}
		
		printf("%s: Now I am trying to send the results back.\n", proc_owner2);
		if (write(pipefd[1], &results2, sizeof(results2)) < 0)
		{
			perror(ERR_WRITING);
			exit(EXIT_FAILURE);
		}
		
		printf("%s: Survey finished. Time to go home.\n", proc_owner2);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	
	return 0;
}
