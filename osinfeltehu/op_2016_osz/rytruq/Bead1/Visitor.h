#include <time.h>

struct Visitor
{
	int event_id;
	char name[100];
	char e_mail[100];
	time_t sign_date;
};

void print(char* name);

void print_all();
void print_event_members(int event_id);

void add_visitor(char* name, char* e_mail, int event_id);

void delete_visitor(char* name);
void delete_event_members(int id_event);

void modify_name(char* name, char* name_modify);
void modify_e_mail(char* name, char* modify_e_mail);
void modify_event_id(char* name , char* event_id_modify);

