

struct Event
{
	int id;
	char name[100];
	int visitor_count;
};

void new_event(char* name , int id);
void print_all_events();
void delete_event(char* name);