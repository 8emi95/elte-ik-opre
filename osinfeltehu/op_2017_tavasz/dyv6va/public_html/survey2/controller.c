#include "controller.h"

void c_init(controller* c)
{
	qa_init(&c->qa, 0);
	c->f = fopen("data.txt", "r+"); // Read from data.txt
	if (c->f == NULL)
	{
		perror("File error");
		exit(1);
	}
	char* line = ALLOC(char, LENGTH);
	while (!feof(c->f))
	{
		question q;
		q.count = 0;
		fgets(line, LENGTH, c->f);
		if(!feof(c->f))
		{
			strtok(line, "\n"); // Cut the newline character from the end of the string
			strcpy(q.ques, line); // The actual question
			fgets(line, LENGTH, c->f);
			strtok(line, "\n");
			strcpy(q.t, line); // Date
			int i;
			int answer_count = 0;
			for(i = 0; i < MAX_ANS; i++) // Answers
			{
				fgets(line, LENGTH, c->f);
				if(strstr(line, "<empty>") == NULL)
				{
					strcpy(q.answers[i], line);
					strtok(q.answers[i], "\n");
					answer_count++;
				}
			}
			q.count = answer_count;
			while (answer_count < MAX_ANS) // Fill remaining entries with <empty> tags
			{
				strcpy(q.answers[answer_count], "<empty>");
				answer_count++;
			}
			fgets(line, LENGTH, c->f); // Provided that the questions are separated by a line break
			new_question(&c->qa, &q);
		}
	}
	fclose(c->f);
}

// Select questions
void select_questions(controller* c)
{
	qa_init(&c->selected, NUM_NEEDED);
	int max = get_question_size(&c->qa);
	if (max < NUM_NEEDED)
	{
		perror("Not enough questions are provided.");
		exit(1);
	}
	srand(time(NULL));
	int i, j;
	int check[max];
	for (j = 0; j < max; j++)
	{
		check[j] = 0;
	}
	
	for (i = 0; i < NUM_NEEDED; i++)
	{
		c->sel_ind[i] = rand() % max;
		while (check[c->sel_ind[i]] == 1)
		{
			c->sel_ind[i] = rand() % max;
		}
		new_question(&c->selected, at(&c->qa, c->sel_ind[i]));
		check[c->sel_ind[i]] = 1;
	}
}

// Show selected questions
void print_questions(controller* c)
{
	print_all(&c->selected);
}