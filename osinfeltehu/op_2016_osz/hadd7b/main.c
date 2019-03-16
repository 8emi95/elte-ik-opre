#include "menu.c"
/*#include <string.h>
#include <stdio.h>*/

#define CHOICE_LIMIT 5 
#define LINE_SIZE 256

int main(int argv, char** argc)
{
	print_menu();

	char buf[CHOICE_LIMIT];
	int choice;

	do
	{
		write(1, "Válasszon menüt.\n", 19);
		memset(buf, 0, CHOICE_LIMIT);
		read(0, buf, CHOICE_LIMIT);
		choice = atoi(buf);
		if(choice > mlen) choice = 6;

		// quantity of parameters of the member functions are stored
		// as members of the dispatcher
		int pc = dispatcher[choice].msg_count;
		char** params;
		params  = mmap(NULL, sizeof(char*)*3,
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,
			-1, 0);
		for(int i=0; i < pc; ++i)
		{
			write(1, dispatcher[choice].msgs[i], str_len(dispatcher[choice].msgs[i]));
			write(1, "\n", 1);
			params[i] = mmap(NULL, sizeof(char)*LINE_SIZE,
				PROT_READ|PROT_WRITE,
				MAP_PRIVATE|MAP_ANONYMOUS,
				-1, 0);
			read(0, params[i], LINE_SIZE);
			rm_newline(params[i]);
		}

		dispatcher[choice].funcptr(
			params[0],
			pc > 1 ? params[1] : NULL,
			pc > 2 ? params[2] : NULL
		);

	}
	// mlen is the size of the menu items
	// it's declared as extern close to the dispatcher
	while (1);

	return 0;	
}

