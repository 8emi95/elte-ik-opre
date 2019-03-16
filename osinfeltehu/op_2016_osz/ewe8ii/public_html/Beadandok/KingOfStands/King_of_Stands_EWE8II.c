//todo: char name[50]; itt kell /0 a végére?
//add new event működik. listázás is. törlésnl már a filet törli. amikor hozáadok uj embert, akkor majd az eventid.txt filehoz kell hozzáadni. minden eventhez külön file

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
 
/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>

#define clear() printf("\033[H\033[J")

#define EVENT_NAME_SIZE 60
#define GUEST_NAME_SIZE 60
#define GUEST_EMAIL_SIZE 100

typedef enum { false, true } bool;

typedef struct {
	char name[EVENT_NAME_SIZE];
	int id;
	int headcount;
} Event;

typedef struct {
	char name[GUEST_NAME_SIZE];
	char email[GUEST_EMAIL_SIZE];
	int event_id;
	struct tm sysdate;
} Visitor;

//menu
void menu();

//event
void add_new_event();
int list_events();
void delete_event();
bool is_valid_event_id(int id);
void inc_event_headcount(int event_id);

//visitor
void add_new_visitor();
int list_visitors();
void delete_visitor();
bool is_email_in_use_at_all(char* email);
bool is_email_in_use(char* email, int event_id);
void modify_visitor();

//file
void copy_file(const char *from,const char *to);
int file_exist (char *filename);

int main(int argc,char *argv[])
{
    menu();
	
	return 0;
}

void menu()
{
	clear();

	int ch;

	do{
		printf("Menu\n\n");
		printf("1 - Add visitor\n");
		printf("2 - Add event\n");
		printf("3 - List events\n");
		printf("4 - List visitors\n");
		printf("5 - Delete event\n");
		printf("6 - Delete visitor\n");
		printf("7 - Modify visitor\n");
		printf("x - Exit\n\n");
		printf(">>");

		//workaround?
		while ((ch = getchar()) == '\n');

		switch (ch){
			case '1':
				add_new_visitor();
				break;
			case '2':
				add_new_event();
				break;
			case '3':
				list_events();
				break;
			case '4':
				list_visitors();
				break;
			case '5':
				delete_event();
				break;
			case '6':
				delete_visitor();
				break;
			case '7':
				modify_visitor();
				break;
			case 'x':
				break;
			default:
				printf("Invalid menu option\n");
		}

		printf("\n");

	}while (ch != 'x');
}

void add_new_visitor(){
	clear();

	if(list_events() == 0){
		printf("Add event first");
		return;
	}

	int visitors_file = open("visitors.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR|S_IWUSR);

	if(visitors_file < 0) {
		perror("Error at opening file \n");
		exit(1);
	}

	Visitor new_visitor;
	printf("Visitor's name:\t");
	scanf("%s", new_visitor.name);
	
	do{
		printf("Event ID:\t");
		scanf("%d", &new_visitor.event_id);
		
		if(!is_valid_event_id(new_visitor.event_id)){
			printf("No event with id: %d\n", new_visitor.event_id);
		}

	}while(!is_valid_event_id(new_visitor.event_id));

	do{
		printf("Visitor's e-mail:\t");
		scanf("%s", new_visitor.email);

		if(is_email_in_use(new_visitor.email, new_visitor.event_id)){
			printf("Email already in use. \n");
		}

	}while(is_email_in_use(new_visitor.email, new_visitor.event_id));

	time_t t = time(NULL);
	new_visitor.sysdate = *localtime(&t);

	write(visitors_file, &new_visitor, sizeof(new_visitor));

	if(close(visitors_file) < 0) {
		perror("Error at closing file \n");
		exit(1);
	}

	inc_event_headcount(new_visitor.event_id);
}

void delete_visitor(){
	clear();

	if(list_visitors() == 0){
		printf("Add visitors first");
		return;
	} 

		int visitors_file = open("visitors.txt", O_RDONLY);
		int tmp_file = open("tmp_file.txt", O_WRONLY | O_APPEND | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR);

		if(visitors_file < 0) {
			perror("Error at opening file \n");
			exit(1);
		}

		Visitor current_visitor;

		char email[GUEST_EMAIL_SIZE];
		int event_id;

		list_events();

		do {
			printf("ID of the event you want to delete from:\t");
			scanf("%d",&event_id);
			if(!is_valid_event_id(event_id)){
				printf("No event with id: %d\n", event_id);
			}

		} while( !is_valid_event_id(event_id));

		list_visitors();
		do{
			printf("Visitor's e-mail you want to delete:\t");
			scanf("%s", email);

			if(!is_email_in_use(email, event_id)){
				printf("No such email. \n");
			}

		}while(!is_email_in_use(email, event_id));

		while(read(visitors_file, &current_visitor, sizeof(Visitor)) > 0){
			if(strcmp(current_visitor.email, email) != 0 || current_visitor.event_id == event_id){
				write(tmp_file, &current_visitor, sizeof(Visitor));
			}
		}

		if(close(visitors_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}

		copy_file("tmp_file.txt","visitors.txt");

		if(close(tmp_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}
}

void add_new_event(){
		clear();

		int events_file = open("events.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR|S_IWUSR);

		if(events_file < 0) {
			perror("Error at opening file \n");
			exit(1);
		}
 
		Event new_event;

		printf("Event name:\t");
		scanf("%s", new_event.name);

		do{
			printf("Event ID:\t");
			scanf("%d", &new_event.id);

			if(is_valid_event_id(new_event.id)){
				printf("There is already an event with id: %d\n", new_event.id);
			}

		} while (is_valid_event_id(new_event.id));
		new_event.headcount = 0;

		write(events_file, &new_event, sizeof(Event));

		if(close(events_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}
}

void delete_event(){
	clear();

	if(list_events() == 0){
		printf("Add event first");
		return;
	} 

	int events_file = open("events.txt", O_RDONLY);
	int visitors_file = open("visitors.txt", O_RDONLY);
	int tmp_file = open("tmp_file.txt", O_WRONLY | O_APPEND | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR);

	int event_id;

	list_events();
	printf("ID of the event you want to delete:\t");
	do {
		
		scanf("%d",&event_id);
		if(!is_valid_event_id(event_id)){
			printf("No event with id: %d\n", event_id);
			printf("ID of the event you want to delete:\t");
		}

	} while( !is_valid_event_id(event_id));

	Event current_event;
	Visitor current_visitor;

	while(read(events_file, &current_event, sizeof(Event)) > 0){
		if( event_id != current_event.id){
			write(tmp_file, &current_event, sizeof(Event));
		}
	}

	if(close(events_file) < 0) {
		perror("Error at closing file \n");
		exit(1);
	}
	
	copy_file("tmp_file.txt","events.txt");

	if(close(tmp_file) < 0) {
		perror("Error at closing file \n");
		exit(1);
	}

	tmp_file = open("tmp_file.txt", O_WRONLY | O_APPEND | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR);

	if(list_visitors() != 0){
		while(read(visitors_file, &current_visitor, sizeof(Visitor)) > 0){
			if( event_id != current_visitor.event_id){
				write(tmp_file, &current_visitor, sizeof(Visitor));
			}
		}
	
		if(close(visitors_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}
	
		copy_file("tmp_file.txt","visitors.txt");
	}

	if(close(tmp_file) < 0) {
		perror("Error at closing file \n");
		exit(1);
	}
}

void modify_visitor(){
		clear();

	if(list_visitors() == 0){
		printf("Add visitors first");
		return;
	} 

		int visitors_file = open("visitors.txt", O_RDONLY);
		int tmp_file = open("tmp_file.txt", O_WRONLY | O_APPEND | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR);

		if(visitors_file < 0) {
			perror("Error at opening file \n");
			exit(1);
		}

		Visitor current_visitor;

		char email[GUEST_EMAIL_SIZE];
		char new_email[GUEST_EMAIL_SIZE];
		char new_name[GUEST_NAME_SIZE];

		list_visitors();
		do{
			printf("Visitor's e-mail you want to modify:\t");
			scanf("%s", email);

			if(!is_email_in_use_at_all(email)){
				printf("No such email. \n");
			}

		}while(!is_email_in_use_at_all(email));

		do{
			printf("New email: \t");
			scanf("%s", new_email);

			if(is_email_in_use_at_all(new_email)){
				printf("Email already in use. \n");
			}

		}while(is_email_in_use_at_all(new_email));

		printf("New name: \t");
		scanf("%s", new_name);

		while(read(visitors_file, &current_visitor, sizeof(Visitor)) > 0){
			if(strcmp(current_visitor.email, email) == 0){
				strcpy(current_visitor.name, new_name);
				strcpy(current_visitor.email, new_email);
				time_t t = time(NULL);
				current_visitor.sysdate = *localtime(&t);
			}

			write(tmp_file, &current_visitor, sizeof(Visitor));
		}

		if(close(visitors_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}

		copy_file("tmp_file.txt","visitors.txt");

		if(close(tmp_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}
}

int list_events(){
	clear();

	if (file_exist ("events.txt"))
	{
		int events_file = open("events.txt", O_RDONLY);
		int event_count = 0;

		if(events_file < 0) {
			perror("Error at opening file \n");
			exit(1);
		}

		Event current_event;

		while(read(events_file, &current_event, sizeof(Event)) > 0){
			event_count++;
			printf("The name of the event: %s\n", current_event.name);
			printf("The ID of the event: %d\n", current_event.id);
			printf("HeadCount: %d\n\n", current_event.headcount);	
		}

		if(close(events_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}

		if(event_count == 0){
			printf("No events added. ");
		}

		return event_count;

	}else{
		printf("No events added. ");
		return 0;
	}
}

int list_visitors(){
	clear();

	if (file_exist ("visitors.txt"))
	{
		int visitors_file = open("visitors.txt", O_RDONLY);
		int visitor_count = 0;

		if(visitors_file < 0) {
			perror("Error at opening file \n");
			exit(1);
		}

		Visitor current_visitor;

		while(read(visitors_file, &current_visitor, sizeof(Visitor)) > 0){
			printf("Visitor's name: %s\n", current_visitor.name);
			printf("Visitor's email: %s\n", current_visitor.email);
			printf("Event id: %d\n", current_visitor.event_id);	
			printf("Registered: %d-%d-%d %d:%d:%d\n\n", current_visitor.sysdate.tm_year + 1900, current_visitor.sysdate.tm_mon + 1, current_visitor.sysdate.tm_mday, current_visitor.sysdate.tm_hour, current_visitor.sysdate.tm_min, current_visitor.sysdate.tm_sec);
			visitor_count++;
		}

		if(close(visitors_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}

		if(visitor_count == 0){
			printf("No visitors added. ");
		}

		return visitor_count;

	}else{
		printf("No visitors added. ");
		return 0;
	}
}

//file másolása másik fileba
void copy_file(const char *from,const char *to){
	FILE *fd1 = fopen(from, "r");
	FILE *fd2 = fopen(to, "w");
	if(!fd1 || !fd2)
 		perror("Opening files. ");

	size_t l1;
	unsigned char buffer[8192]; 

	//Data to be read
	while((l1 = fread(buffer, 1, sizeof buffer, fd1)) > 0) {
  		size_t l2 = fwrite(buffer, 1, l1, fd2);
  			if(l2 < l1) {
    			if(ferror(fd2))
      				perror("Reading files");
    			else
      				perror("Reading files");
  			}
	}

	fclose(fd1);
	fclose(fd2);
}

//létezik e a file
int file_exist (char *filename)
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

//létezik e az event
bool is_valid_event_id(int id){
	int events_file = open("events.txt", O_RDONLY);

	if(events_file < 0) {
		perror("Error at opening file \n");
		exit(1);
	}

	Event current_event;

	while(read(events_file, &current_event, sizeof(Event)) > 0){
		if(current_event.id == id){
			return true;
		}
	}

	if(close(events_file) < 0) {
		perror("Error at closing file \n");
		exit(1);
	}

	return false;
}

//adott event headcount növelése egyel, és headcount kiratása
void inc_event_headcount(int event_id){
	int events_file = open("events.txt", O_RDONLY);
	int tmp_file = open("tmp_file.txt", O_WRONLY | O_APPEND | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR);


	if(events_file < 0) {
		perror("Error at opening file \n");
		exit(1);
	}

	Event current_event;

	while(read(events_file, &current_event, sizeof(Event)) > 0){
		if(current_event.id == event_id){
				current_event.headcount++;
				printf("\n Event headcount: %d\n", current_event.headcount);
		}
		
		write(tmp_file, &current_event, sizeof(Event));
	}

	if(close(events_file) < 0) {
		perror("Error at closing file \n");
		exit(1);
	}

	copy_file("tmp_file.txt","events.txt");

	if(close(tmp_file) < 0) {
		perror("Error at closing file \n");
		exit(1);
	}
}

//Egy emailel csak egyszer lehet regisztrálni egy eseményre
bool is_email_in_use_at_all(char* email){
	if(file_exist("visitors.txt")){
		int visitors_file = open("visitors.txt", O_RDONLY);

		if(visitors_file < 0) {
			perror("Error at opening file \n");
			exit(1);
		}

		Visitor current_visitor;

		while(read(visitors_file, &current_visitor, sizeof(Visitor)) > 0){
			if(strcmp(current_visitor.email, email) == 0){
				return true;
			}
		}

		if(close(visitors_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}
	}else{
		return false;
	}

	return false;
}

//Egy emailel csak egyszer lehet regisztrálni egy eseményre
bool is_email_in_use(char* email, int event_id){
	if(file_exist("visitors.txt")){
		int visitors_file = open("visitors.txt", O_RDONLY);

		if(visitors_file < 0) {
			perror("Error at opening file \n");
			exit(1);
		}

		Visitor current_visitor;

		while(read(visitors_file, &current_visitor, sizeof(Visitor)) > 0){
			if(current_visitor.event_id == event_id && strcmp(current_visitor.email, email) == 0){
				return true;
			}
		}

		if(close(visitors_file) < 0) {
			perror("Error at closing file \n");
			exit(1);
		}
	}else{
		return false;
	}

	return false;
}