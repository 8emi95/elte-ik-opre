#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void to_date(time_t raw_time, char date_buffer[15])
{
	struct tm* tm_info = localtime(&raw_time);
	strftime(date_buffer, 15, "%Y. %m. %d.\n", tm_info);
}

struct visitor
{
	char name[100];
	char mail_address[100];
	int event_id;
	time_t register_date;
};

struct visitor_node
{
	struct visitor data;
	struct visitor_node* next;
};

void print_visitor(struct visitor* visitor_to_print)
{
		printf("\nNév: %s.\n", visitor_to_print->name);
		printf("E-mail cím: %s.\n", visitor_to_print->mail_address);
		printf("Rendezvény azonosítója: %i.\n", visitor_to_print->event_id);
		
		char date_buffer[15];
		to_date(visitor_to_print->register_date, date_buffer);
		printf("A jelentkezés dátuma: %s", date_buffer);
}

void insert_visitor(struct visitor_node* visitors, struct visitor* new_visitor)
{
	struct visitor_node* new_visitor_node = malloc(sizeof(struct visitor_node));
	new_visitor_node->data = *new_visitor;
	new_visitor_node->next = visitors->next;
	visitors->next = new_visitor_node;
}

struct visitor* find_visitor(struct visitor_node* visitors, char name[100])
{
	for (visitors = visitors->next; visitors != NULL; visitors = visitors->next)
		if (strcmp(visitors->data.name, name) == 0)
			return &visitors->data;
	
	return NULL;
}

int remove_visitor(struct visitor_node* visitors, char name_to_delete[100])
{
	for (; visitors->next != NULL; visitors = visitors->next)
	{
		if (strcmp(visitors->next->data.name, name_to_delete) == 0)
		{
			struct visitor_node* deletable = visitors->next;
			visitors->next = visitors->next->next;
			free(deletable);
			return 1;
		}
	}
	
	return 0;
}

void load_visitors(struct visitor_node* visitors, const char* file_name)
{
	int input;
	input = open(file_name, O_RDONLY);
	if (input < 0)
	{
		printf("Nem található adatfájl (%s).\n", file_name);
		return;
	}
	
	struct visitor current_visitor;
	while (read(input, &current_visitor, sizeof(current_visitor)))
	{
		insert_visitor(visitors, &current_visitor);
	}
	
	close(input);
}

void save_visitors(struct visitor_node* visitors, const char* file_name)
{
	int output = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (output < 0)
	{
		perror("Hiba az adatok mentése közben.\n");
		return;
	}
	
	visitors = visitors->next;
	while (visitors != NULL)
	{
		if (write(output, &(visitors->data), sizeof(visitors->data)) != sizeof(visitors->data))
		{
			perror("Hiba az adatok mentése közben.\n");
			return;
		};
		
		visitors = visitors->next;
	}
	
	close(output);
}
