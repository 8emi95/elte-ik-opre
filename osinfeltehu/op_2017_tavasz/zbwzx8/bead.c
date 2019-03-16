#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

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
		strtok(q[ind].choices[c],"\n");
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
struct Answers
{
	int answ[3];
};
int main()
{
	int fid=mkfifo("mlsz", S_IRUSR|S_IWUSR );
	pid_t child = fork();
	
	if(child < 0)
	{
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
	}
	else if(child > 0)
	{
		struct Question* q;

		int max_size = 10;//menuben a merete ha szukseges megvaltozik
		int size = 0;
		q = (struct Question*)malloc(max_size*sizeof(struct Question));
	
		menu(q,&size,&max_size);		
		CLEARSCR;
		if(size < 3)
		{
			printf("Too few questions were given.");
			exit(1);
		}

		char city[50];
		printf("Enter the name of the city:\n");
		fgets(city,sizeof(char)*50,stdin);
		strtok(city,"\n");
		fflush(stdin);
		fflush(stdout);

		struct Question rnd_questions[3];
		int ind_of_q[3];
		srand(time(NULL));
		ind_of_q[0] = rand() % size;
		int next = 1;
		int i = 0;			
		while(next < 3)
		{
			srand(time(NULL));
			int tmp = rand() % size;
			bool is_in = false;
			i = 0;
			while(!is_in && i < next)
			{
				if(tmp == ind_of_q[i])
				{
					is_in = true;
				}
				++i;
			}
			if(!is_in)
			{
				ind_of_q[next] = tmp;
				++next;
			}
		}

		for(i = 0; i < 3; ++i)
		{
			rnd_questions[i].num_of_choices = q[ind_of_q[i]].num_of_choices;
			strcpy(rnd_questions[i].date_of_creation,q[ind_of_q[i]].date_of_creation);
			strcpy(rnd_questions[i].question,q[ind_of_q[i]].question);
			int j = 0;
			for(j = 0; j < rnd_questions[i].num_of_choices; ++j)
			{
				strcpy(rnd_questions[i].choices[j],q[ind_of_q[i]].choices[j]);
			}
		}
		
		int ret = open("mlsz", O_WRONLY);
		for(i = 0; i < 3; ++i)
		{
			write(ret,&rnd_questions[i].date_of_creation,sizeof(char)*50);
			write(ret,&rnd_questions[i].question,sizeof(char)*50);
			int j = 0;
			write(ret,&rnd_questions[i].num_of_choices,sizeof(int));
			for(j = 0; j < rnd_questions[i].num_of_choices; ++j)
			{
				write(ret,&rnd_questions[i].choices[j],sizeof(char)*50);	
			}
		}
		
		close(ret);
		
		ret = open("mlsz", O_RDONLY);
		int num_of_people;

		read(ret,&num_of_people,sizeof(int));
		struct Answers answers[num_of_people];

		for(i = 0; i < num_of_people; ++i)
		{
			read(ret,&answers[i].answ,sizeof(struct Answers));
		}
		close(ret);
		
		printf("City:%s\n",city);
		printf("%i people filled out the questionnaire\n",num_of_people);
		printf("The results are:\n");
		//kiszamit + kiir
		int choices[4];
		choices[0] = 0;
		choices[1] = 0;
		choices[2] = 0;
		choices[3] = 0;
		for(i = 0; i < 3; ++i)
		{
			printf("%s",rnd_questions[i].question);
			int j = 0;
			for(j = 0; j < num_of_people; ++j)
			{
				++choices[answers[j].answ[i]];
			}
			for(j = 0; j < rnd_questions[i].num_of_choices; ++j)
			{
				printf("%s: %i\n",rnd_questions[i].choices[j], choices[j]);
				choices[j] = 0;
			}
		}
	}	
	else
	{
		srand(time(NULL));
		int num_of_people = (rand() % 11) + 10;
		
		struct Question questions[3];
		int ret = open("mlsz",O_RDONLY);			
		int i = 0;

		for(i = 0; i < 3; ++i)
		{
			read(ret,&questions[i].date_of_creation,sizeof(char)*50);
			read(ret,&questions[i].question,sizeof(char)*50);
			int j = 0;
			read(ret,&questions[i].num_of_choices,sizeof(int));
			for(j = 0; j < questions[i].num_of_choices; ++j)
			{
				read(ret,&questions[i].choices[j],sizeof(char)*50);	
			}
		}
		close(ret);
		ret = open("mlsz",O_WRONLY);

		write(ret,&num_of_people,sizeof(int));		
		struct Answers to_parent[num_of_people];
		for(i = 0; i < num_of_people; ++i)
		{
			int j = 0;
			for(j = 0; j < 3; ++j)
			{
				to_parent[i].answ[j] = rand() % questions[j].num_of_choices;
			}
		}
		for(i = 0; i < num_of_people; ++i)
		{
			write(ret,&to_parent[i].answ,sizeof(struct Answers));
		}
		close(ret);
	}
	return 0;
}
