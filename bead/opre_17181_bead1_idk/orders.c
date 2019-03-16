#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum routes {
	PARLIAMENT, HOSOK_SQUARE, CASTLE
};

char *enum_string(enum routes r){
	char *string[] = {"Parliament", "Hosok square", "Castle"};
	
	return string[r];
}

typedef struct Tours {
	char email[50];
	char name[50];
	char phone_number[20];
	int  participant;
	enum routes route;
	char order_time[50];
}Tours;

typedef struct Orders {
	Tours tours[1000];
	int orders_count;
}Orders;

void collect_name(char name[50]){
	printf("Name: ");
	scanf(" %s", name);
}

void collect_email(char email[50]){
	printf("E-mail: ");
	scanf(" %s", email);
}

void collect_phone_number(char phone_number[20]){
	printf("Phone number: ");
	scanf(" %s", phone_number);
}

int collect_participant(){
	int participant = 0;
	char turing_in_string[50]; 
	while(participant == 0){
		printf("Number of participant: ");
		scanf(" %s", &turing_in_string);
		sscanf(turing_in_string, "%d", &participant);
		if(participant == 0){
			printf("Bad number. Try again\n ");
		}
	}
}

char* collect_time(){
	char now[50];
	time_t t = time(NULL);
	struct tm * p = localtime(&t);

	strftime(now, 50, "%B %d %Y", p);
	
	char* return_time = now;
	
	return return_time;
}

enum routes collect_route_info(){
	printf("Choose a tour:\n");
	char tour_type[10];
	int is_tour_type = 0;
	while(is_tour_type == 0){
		printf("(P)arliament - (H)osok square - (C)astle\n");
		is_tour_type = 1;
		scanf(" %s", &tour_type);
		switch(*tour_type){
			case 'P':
				return PARLIAMENT;
			case 'H':
				return HOSOK_SQUARE;
			case 'C':
				return CASTLE;
			default:
				is_tour_type = 0;
				printf("Your first button not a tour type char. Please choose againg.\n");
		}
	}
}

void print_order(Orders orders, int order_number){
	
	if(orders.tours[order_number].email[0] != '#'){
		printf("Name: %s\n", orders.tours[order_number].name);
		printf("E-mail: %s\n", orders.tours[order_number].email);
		printf("Phone: %s\n", orders.tours[order_number].phone_number);
		printf("Participant: %d\n", orders.tours[order_number].participant);
		printf("Order date: %s\n", orders.tours[order_number].order_time);
		printf("Tour: %s\n------\n", enum_string(orders.tours[order_number].route));
	}
}

struct Tours tours_collector(){	
	Tours tour;

	char name[50];
	collect_name(name);
	strncpy(tour.name, name, 50);
	
	char email[50];
	collect_email(email);
	strncpy(tour.email, email, 50);
	
	char phone_number[20];
	collect_phone_number(phone_number);
	strncpy(tour.phone_number, phone_number, 50);
	
	tour.participant = collect_participant();
	
	tour.route = collect_route_info();
	
	strncpy(tour.order_time, collect_time(), 50);
		
	printf("Name: %s\n", tour.name);
	printf("E-mail: %s\n", tour.email);
	printf("Phone number: %s\n", tour.phone_number);
	printf("participants: %d\n", tour.participant);
	printf("Order date: %s\n", tour.order_time);
	printf("Your tour: %s\n", enum_string(tour.route));
	
	return tour;
}

Orders create_tour(Orders orders){
	Tours tour;
	tour = tours_collector();
	orders.tours[orders.orders_count] = tour;
	++orders.orders_count;
	
	return orders;
}

char pop_up_modify_menu(){
	char menu_button[10];
	int is_menu_point = 0;
	printf("Please choose a Menu point.\n");
	while(is_menu_point == 0){			
		printf("(E)mail\n(N)ame\n(P)articipant\n(T)our\n");
		scanf(" %s", &menu_button);
		if(menu_button[0] == 'E' || menu_button[0] == 'N' || menu_button[0] == 'P' || menu_button[0] == 'T'){
			is_menu_point = 1;
		}else{
			printf("Your first button not a menu point. Please choose againg.\n");
		}
	}
		
	return menu_button[0];
}

Orders modify_tour(Orders orders){
	char modify[50];
	collect_email(modify);

	int i = 0;
	for(i; i < orders.orders_count; ++i){
		if(strcmp(orders.tours[i].email, modify) == 0){
			print_order(orders, i);
			printf("What would you like to modify?\n");			
			char menu = pop_up_modify_menu();
			switch(menu){
			case 'E':{
				char email[50];
				collect_email(email);
				strncpy(orders.tours[i].email, email, 50);
				break;}
			case 'N':{
				char name[50];
				collect_name(name);
				strncpy(orders.tours[i].name, name, 50);
				break;}
			case 'P':
				orders.tours[i].participant = collect_participant();
				break;
			case 'T':
				orders.tours[i].route = collect_route_info();
				break;
			}
			return orders;
		}
	}
	printf("Email not found!\n");
	return orders;
}

Orders delete_tour(Orders orders){
	char delete[50];
	printf("Please enter the email address you would like to delete\n");	
	collect_email(delete);
	int i = 0;
	for(i; i < orders.orders_count; ++i){
		if(strcmp(orders.tours[i].email, delete) == 0){
			strncpy(orders.tours[i].email, "# ", 50);
			return orders;
		}
	}
	printf("Email not found!\n");
	return orders;
}

void list_orders(Orders orders){
	int i = 0;
	for(i; i < orders.orders_count; ++i){
		print_order(orders, i);
	}
}

void list_by_route(Orders orders){
	enum routes route = collect_route_info();
	int i = 0;
	for(i; i < orders.orders_count; ++i){
		if(route == orders.tours[i].route ){
			print_order(orders, i);
		}
	}
}

void save_file(Orders orders){
	FILE *file;
    file = fopen("orders", "w");
	int i = 0;
	for(i; i < orders.orders_count; ++i){
		if(orders.tours[i].email[0] != '#'){
			fprintf(file, "%s-%s-%s-%d-%s-", 
					orders.tours[i].email, 
					orders.tours[i].name, 
					orders.tours[i].phone_number, 
					orders.tours[i].participant, 
					orders.tours[i].order_time);
			char route[10];
			strncpy(route, enum_string(orders.tours[i].route), 10);
			fprintf(file, "%c\n", route[0]);
		}
	}
	fclose(file);
}

char pop_up_main_menu(){
	char menu_button[10];
	int is_menu_point = 0;
	printf("Please choose a Menu point.\n");
	while(is_menu_point == 0){
		printf("(O)rdering\n(M)odify an order\n(D)eleting\n(L)ist orders\nList by (R)oute\n");
		scanf(" %s", &menu_button);
		if(menu_button[0] == 'O' || menu_button[0] == 'M' || menu_button[0] == 'D' || menu_button[0] == 'L' || menu_button[0] == 'R'){
			is_menu_point = 1;
		}else{
			printf("Your first button not a menu point. Please choose againg.\n");
		}
	}
		
	return menu_button[0];
}

void work_with_database(Orders orders){
	
	int working = 1;
	
	while(working == 1){
		
		char menu = pop_up_main_menu();
		switch(menu){
			case 'O':
				orders = create_tour(orders);
				save_file(orders);
				break;
			case 'M':
				orders = modify_tour(orders);
				save_file(orders);
				break;
			case 'D':
				orders = delete_tour(orders);
				save_file(orders);
				break;
			case 'L':
				list_orders(orders);
				break;
			case 'R':
				list_by_route(orders);
		}
		printf("Do you want (C)ontinue working with database?\n");
		char working_issue[10]; 
		scanf(" %s", &working_issue);
		if(working_issue[0] != 'C'){
			working = 0;
		}
	}
}

Orders upload_orders_struct(){
	Orders orders;
    FILE* file = fopen("orders", "r");
    char line[256];

	orders.orders_count = 0;
	if(file){
		while (fgets(line, sizeof(line), file)) {
			char *split;
			Tours tour;

			split = strtok(line, "-");
			if (split != NULL) {
				strcpy(tour.email, split);	
				split = strtok(NULL, "-");
				strcpy(tour.name, split);	
				split = strtok(NULL, "-");
				strcpy(tour.phone_number, split);	
				split = strtok(NULL, "-");
				char participant_array[2];		
				strcpy(participant_array, split);
				tour.participant = atoi(participant_array);
				split = strtok(NULL, "-");
				strcpy(tour.order_time, split);	
				split = strtok(NULL, "-");
				switch(*split){
				case 'P':
					tour.route = PARLIAMENT;
					break;
				case 'H':
					tour.route = HOSOK_SQUARE;
					break;
				case 'C':
					tour.route = CASTLE;
				}
				orders.tours[orders.orders_count] = tour;
				++orders.orders_count;	
			}
		}
	
    fclose(file);
	}else{
		printf("Orders file not found.\n");
		exit(1);
	}
   
	
	return orders;
}

int main(){
	Orders orders;

	orders = upload_orders_struct();

	work_with_database(orders);
	
	printf("Exit\n");
}