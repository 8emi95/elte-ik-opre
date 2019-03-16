struct event
{
	int id;
	char location[50];
	int visitors;
};

struct event_node
{
	struct event data;
	struct event_node* next;
};

void insert_event(struct event_node* events, struct event* new_event)
{
	struct event_node* new_event_node = malloc(sizeof(struct event_node));
	new_event_node->data = *new_event;
	new_event_node->next = events->next;
	events->next = new_event_node;
}

struct event* find_event(struct event_node* events, int id)
{
	for (events = events->next; events != NULL; events = events->next)
	{
		if (events->data.id == id)
		{
			return &events->data;
		}
	}
	
	return NULL;
}

int remove_event(struct event_node* events, int id_to_delete)
{
	for (; events->next != NULL; events = events->next)
	{
		if (events->next->data.id == id_to_delete)
		{
			struct event_node* deletable = events->next;
			events->next = events->next->next;
			free(deletable);
			return 1;
		}
	}
	
	return 0;
}

void load_events(struct event_node* events, const char* file_name)
{
	int input;
	input = open(file_name, O_RDONLY);
	if (input < 0)
	{
		printf("Nem található adatfájl (%s).\n", file_name);
		return;
	}
	
	struct event current_event;
	while (read(input, &current_event, sizeof(current_event)))
	{
		insert_event(events, &current_event);
	}
	
	close(input);
}

void save_events(struct event_node* events, const char* file_name)
{
	int output = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (output < 0)
	{
		perror("Hiba az adatok mentése közben.\n");
		return;
	}
	
	events = events->next;
	while (events != NULL)
	{
		if (write(output, &(events->data), sizeof(events->data)) != sizeof(events->data))
		{
			perror("Hiba az adatok mentése közben.\n");
			return;
		};
		
		events = events->next;
	}
	
	close(output);
}
