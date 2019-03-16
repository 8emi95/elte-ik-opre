#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Visitor.h"
#include "Event.h"

//void sign_visitor();


int main(void)
{
	struct Visitor visitor;
	struct Event event;
	//void print(char* name);
	
	new_event("Lagzi", 1);
	new_event("Buli", 2);
	
	
	add_visitor("Csopi", "cs@cs.hu", 2);
	add_visitor("Robert", "robert@outlook.hu", 2);
	
	delete_visitor("Csopi");
	
	print("Csopi");
	print_all();
	
	print_all_events();
	
	delete_event("Lagzi");
	print_all_events();
	
	return 0;
}
