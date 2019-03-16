#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>


#define CLEARSCR system ( "clear" )

typedef enum{false,true} bool;

struct Question
{
	char date_of_creation[50];
	char question[50];
	char choices[4][50];
	int num_of_choices;
};

int get_len (int val)
{
  int ret=1;
  while(val>9)
  { 
	ret++;
	val/=10;
  }
  return ret;
}

void read_int_input(int* choice,int leftbound,int rightbound)
{
	fflush(stdin);
	int len = get_len(rightbound) + 1;;
	char tmp[len+1];
	bool invalid = true;
	int c;
	while(invalid)
	{
		fflush(stdin);
		fgets(tmp,sizeof(tmp),stdin);
		c = atoi(tmp);
		if(!(c < leftbound || c >= rightbound))
		{
			invalid = false;
		}
		else
		{
			printf("Invalid input\n");
		}
	}
	fflush(stdin);
	*choice = c;
}

void read_question(struct Question* q, int ind)
{
	CLEARSCR;
	printf("Enter question(maximum 49 characters)\n");
	fflush(stdin);
	fgets(q[ind].question,sizeof(q[ind].question),stdin);
	
	printf("Enter how many choices the question should have(2..4):");
	int c;
	read_int_input(&c,2,5);
	
	q[ind].num_of_choices = c;
	
	c = 0;
	
	while(c < q[ind].num_of_choices)
	{
		printf("%i choice(maximum 49 characters):\n",c+1);
		fflush(stdin);
		fgets(q[ind].choices[c],sizeof(q[ind].choices[c]),stdin);
		++c;
	}
	time_t rawtime;
	time(&rawtime);
	strcpy(q[ind].date_of_creation,ctime(&rawtime));
}

void print_question(struct Question* q, int ind)
{
	printf("%i. question:\n",ind);
	printf("%s \n",&q[ind].question);
	printf("Choices:\n");
	int j;
	for(j = 0; j < q[ind].num_of_choices; ++j)
	{
		printf("%i. choice: ",j);
		printf("%s \n",&q[ind].choices[j]);
	}
}

void print_all_questions(struct Question* questions,int size)
{
	CLEARSCR;
	int i;
	for(i = 0; i < size; ++i)
	{
		print_question(questions,i);
	}
}

void modify_question(struct Question* questions, int size)
{
	CLEARSCR;
	if(size != 0)
	{
		print_all_questions(questions,size);
		printf("Enter the number of the question, which you want to modify:\n");
		
		int choice = 0;
		read_int_input(&choice,0,size);
		
		read_question(questions,choice);
	}
	else
	{
		printf("There are no questions!\n");
	}
}

void delete_question(struct Question* questions, int* size)
{
	CLEARSCR;
	if(*size != 0)
	{
		print_all_questions(questions,*size);
		printf("Enter the number of the question, which you want to delete:");
		
		int choice = 0;
		read_int_input(&choice,0,*size);
		
		if(choice != *size-1)
		{
			strcpy(questions[choice].question,questions[*size-1].question);
			
			int i;
			for(i = 0; i < questions[*size-1].num_of_choices; ++i)
			{
				strcpy(questions[choice].choices[i],questions[*size-1].choices[i]);
			}
			strcpy(questions[choice].date_of_creation,questions[*size-1].date_of_creation);
			questions[choice].num_of_choices = questions[*size-1].num_of_choices;
		}
		
		*size = *size - 1;
	}
	else
	{
		printf("There are no questions!\n");
	}
}

void write_to_file(struct Question* questions, int size, int* chosen_questions)
{
	FILE * f;
	f=fopen("kerdoiv","w");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	int i = 0;
	int j;
	while (i != size)
	{
		fprintf(f, "%s", questions[chosen_questions[i]].date_of_creation);
		fprintf(f, "%s", questions[chosen_questions[i]].question);
		fprintf(f, "%i\n", questions[chosen_questions[i]].num_of_choices);
		for(j = 0; j < questions[chosen_questions[i]].num_of_choices; ++j)
		{
		 fprintf(f, "%s", questions[chosen_questions[i]].choices[j]);
		}
	    ++i;
	} 
	 
	fclose(f);
}

bool is_in(int* arr,int curr_size, int val)
{
	int i = 0;
	bool ret_val = false;
	while(i < curr_size && !ret_val)
	{
		if(val == arr[i])
		{
			ret_val = true;
		}
		++i;
	}
	return ret_val;
}

void choose_questions(struct Question* questions, int size)
{
	CLEARSCR;
	int curr_size = 0;
	int* chosen_questions = malloc(size*sizeof(int));
	bool not_ready = true;
	int choice;
	int i;
	while(not_ready && curr_size != size)
	{
		for(i = 0; i < size; ++i)
		{
			if(is_in(chosen_questions,curr_size,i) == false)
			{
				print_question(questions,i);
			}
		}
		printf("Enter the number of the question you want to choose, or %i if there are no more:\n", size);
		read_int_input(&choice, 0, size+1);
		if(choice == size)
		{
			not_ready = false;
		}
		else
		{
			if(is_in(chosen_questions,curr_size,choice) == true)
			{
				printf("It is already in it.\n");
			}
			else
			{
				chosen_questions[curr_size] = choice;
				++curr_size;
			}
		}
	}
	write_to_file(questions, curr_size, chosen_questions);
	free(chosen_questions);
}

void menu(struct Question* questions, int* size, int* max_size)
{
	CLEARSCR;
	bool loop = true;
	char c[20];
	int choice;
	struct Question* tmp;
	while(loop)
	{
		printf("Menu:\n");
		printf("1.new question|2.modify question|\n");
		printf("3.delete question|4.list questions\n");
		printf("5.finalize questions\n");
		fflush(stdin);
		fgets(c,sizeof(c),stdin);
		choice = atoi(c);
		if(choice<1 || choice>5)
		{
			printf("Invalid input!\n");
		}
		else
		{
			switch(choice)
			{
				case 1://new question
				{
					if(*size == *max_size)
					{
						*max_size = *max_size * 2;
						tmp = questions;
						questions = realloc(tmp,*max_size*sizeof(*tmp));
						if(questions == NULL)
						{
							perror("realloc failed to allocate new memory!");
							exit(1);
						}
					}
					read_question(questions,*size);
					*size += 1;
					break;
				}
				case 2://modify question
				{
					modify_question(questions,*size);
					break;
				}
				case 3://delete question
				{
					delete_question(questions,size);
					break;
				}
				case 4://list question
				{
					print_all_questions(questions,*size);
					break;
				}
				case 5://finalize questions
				{
					choose_questions(questions,*size);
					loop = false;
					break;
				}
			}
		}
	}
}

int main()
{
	struct Question* q;
	int max_size = 10;//menuben a merete ha szukseges megvaltozik
	int size = 0;
	q = (struct Question*)malloc(max_size*sizeof(struct Question));
	
	menu(q,&size,&max_size);
	
	return 0;
}