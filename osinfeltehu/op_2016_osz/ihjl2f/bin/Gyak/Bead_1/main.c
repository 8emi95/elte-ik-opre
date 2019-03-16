#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

#include "event_functions.h"

const int number_of_options = 8;
const char available_avtions[] = {'1', '2', '3', '4', '5', '6', '7', 'q'};

void show_main_menu() {
	separator();
	printf("Available actions:\n");
	printf("1. Event list\n");
	printf("2. New event\n");
	printf("3. Register guest\n");
	printf("4. Guest list\n");
	printf("5. Modify guest\n");
	printf("6. Delete registration\n");
	printf("7. Clear event\n");
	printf("\nq. Quit\n");
}

char main_menu() {
	show_main_menu();
	system("/bin/stty raw");
	while(1) {
		char selected = getchar();
		int index;
		for (index = 0; index < number_of_options; index = index + 1) {
			if (selected == available_avtions[index]) {
				system("/bin/stty cooked");
				return selected;
			}
		}
	}
}

int main(int argc,char** argv){
	init_process_file();
	
	char action;
	do {
		action = main_menu();
		switch (action) {
			case '1':
				show_event_list();
				break;
			case '2':
				create_new_event();
				break;
			case '3':
				register_guest();
				break;
			case '4':
				show_guest_list();
				break;
			case '5':
				modify_guest(0);
				break;
			case '6':
				//modify_guest(1);
				delete_guest();
				break;
			case '7':
				reset_event();
				break;
			default:
				break;
		}
	} while(action != 'q');
	
	printf("\n\nFerwell!\n\n");

	return 0;
}