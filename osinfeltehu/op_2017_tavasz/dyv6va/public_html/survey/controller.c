#include "controller.h"
#define LINE_SIZE 200

// Constructor
void c_begin(controller* c)
{
	qa_begin(&c->qa);
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
			q.t = ALLOC(char, LENGTH);
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
}

// Destructor (exit and save)
void c_end_save(controller* c)
{
	c->f = freopen("data.txt", "w", c->f); // Write to data.txt
	int i;
	for(i = 0; i < get_question_size(&c->qa); i++)
	{
	    question* curr_q = at(&c->qa, i);
		fputs(curr_q->ques, c->f); // Question
		fputs("\n", c->f);
		fputs(curr_q->t, c->f); // Time and date
		fputs("\n", c->f);

		int j;
		for(j = 0; j < MAX_ANS; j++) // Answers
		{
			fputs(curr_q->answers[j], c->f);
			fputs("\n", c->f);
		}
		fputs("\n", c->f);
	}
	fclose(c->f);
	qa_end(&c->qa);
}

// Destructor (exit without saving)
void c_end(controller* c)
{
    qa_end(&c->qa);
}

// When the user wants to add a new question
void add_question(controller* c)
{
	question q;
	time_t t;
	time(&t); // Get current time
	q.t = ctime(&t); // Current time added to the question
	strtok(q.t, "\n");
	q.count = 0;
	printf("Please enter the question you want to add:\n\n");
	char* line = ALLOC(char, LENGTH);

	fgets(line, LENGTH, stdin);
	strtok(line, "\n");
	strcpy(q.ques, line);

	printf("\nPlease type in your answers one by one (%i to %i answers are required). Type <exit> to finish.\n\n", MIN_ANS, MAX_ANS);
	int i;
    int exit = 0;
	for(i = 0; i < MAX_ANS; i++)
	{
        if (exit == 1)
        {
			strcpy(q.answers[i], "<empty>");
			printf("%i", i);
			printf("%s", q.answers[i]);
        }
        else
        {
            fgets(line, LENGTH, stdin);
            if (strstr(line, "<exit>") == NULL)
            {
                strtok(line, "\n");
                strcpy(q.answers[i], line);
                q.count++;
            }
            else if (q.count < MIN_ANS)
            {
                printf("At least %i answers are needed.\n", MIN_ANS);
                i--;
            }
            else
            {
                exit = 1;
                strcpy(q.answers[i], "<empty>");
            }
        }
	}
	new_question(&c->qa, &q);
	printf("\n");
}

int select_question(controller* c)
{
	print_questions(c);
	printf("Please select a question from the list above.\n");
	return get_int(1, get_question_size(&c->qa));
}
int select_answer(question* q)
{
	printf("Please select an answer from the list above.\n");
	return get_int(1, get_answer_size(q));
}

// Modify a question
void edit_question(controller* c)
{
    if (get_question_size(&c->qa) < 1)
    {
        printf("There are no questions yet.\n");
    }
    else
    {
        question* selected = at(&c->qa, select_question(c) - 1);
        printf("Question selected:\n\n");
        print_question(selected);
        printf("\nChoose an option:\n1. Modify question\n2. Modify an answer\n3. Add a new answer\n>");
        int option = get_int(1, 3);
        int answer;
        char* selected_ans;
        switch(option)
        {
            case 1:
                printf("Enter the new question.\n");
                fgets(selected->ques, LENGTH, stdin);
                strtok(selected->ques, "\n");
                break;
            case 2:
                printf("Enter the new answer.\n");
                answer = select_answer(selected);
                selected_ans = selected->answers[answer - 1];
                fgets(selected_ans, LENGTH, stdin);
                strtok(selected_ans, "\n");
                break;
            case 3:
                printf("Enter the new answer.\n");
                char* line = ALLOC(char, LENGTH);
                fgets(line, LENGTH, stdin);
                strtok(line, "\n");
                add_answer(selected, line);
                break;
        }
    }
}

// Delete question with all of its answers
void delete_question(controller* c)
{
    if (get_question_size(&c->qa) < 1)
    {
        printf("There are no questions yet.");
    }
    else
    {
        int ind = select_question(c) - 1;
        question* selected = at(&c->qa, ind);
        printf("Question selected:\n\n");
        print_question(selected);
        printf("\nChoose an option:\n1. Delete question\n2. Delete an answer belonging to this question\n");
        int option = get_int(1, 3);
        int answer;
        switch(option)
        {
            case 1:
                remove_question(&c->qa, ind);
                break;
            case 2:
                answer = select_answer(selected) - 1;
                remove_answer(selected, answer);
                break;
        }
    }
    printf("\n");
}

// Show all questions
void print_questions(controller* c)
{
	int i;
	for(i = 0; i < get_question_size(&c->qa); i++)
	{
		printf("#%i.: ", i + 1);
		print_question(at(&c->qa, i));
		printf("\n");
	}
}

// Let the user decide which questions should be kept and which ones removed
void finalize_questions(controller* c)
{
	printf("\nPlease finalize that which questions will remain in your survey.\n\n");
	int i;
	for(i = 0; i < get_question_size(&c->qa); i++)
	{
		printf("Should this question remain in the survey? Y/N\n\n");
		print_question(at(&c->qa, i));
		char* line = ALLOC(char, LENGTH);
		int l = 0;
		while (l == 0)
        {
            fgets(line, LENGTH, stdin);
            printf("\n");
            if (strcmp(line, "y\n") == 0 || strcmp(line, "Y\n") == 0)
            {
                l = 1;
            }
            else if (strcmp(line, "n\n") == 0 || strcmp(line, "N\n") == 0)
            {
                l = 1;
                remove_question(&c->qa, i);
            }
            else
                printf("Please enter Y or N.\n");
        }
	}
}

// Get a number between the lower and upper bound from the standard input
int get_int(int lower_bound, int upper_bound)
{
	int num;
	while (scanf("%d", &num) < 1 || num < lower_bound || num > upper_bound)
	{
		printf("Input error. Please enter a number from the list below.\n>");
		int c;
		while ((c = getchar()) != '\n' && c != EOF ) {}
	}
	getchar();
	return num;
}
