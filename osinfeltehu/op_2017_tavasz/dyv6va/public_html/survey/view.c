#include "view.h"

void run(view* v)
{
	int option = 0;
	c_begin(&v->c);

	printf("Welcome to our survey!\n");
	while(option != 5 && option != 6)
	{
		print_menu();
		option = get_int(1, 6);
		printf("\n");
		switch(option)
		{
			case 1:
				add_question(&v->c);
				break;
			case 2:
				edit_question(&v->c);
				break;
			case 3:
				delete_question(&v->c);
				break;
			case 4:
				print_questions(&v->c);
				break;
			case 5:
				finalize_questions(&v->c);
				break;
		}
		printf("---------\n\n");
	}

    if (option == 6)
        c_end(&v->c);
    else
        c_end_save(&v->c);
}

void print_menu()
{
    printf("Please choose an option from the list below.\n");
	printf("1. Add a new question\n");
	printf("2. Edit an existing question\n");
	printf("3. Remove a question\n");
	printf("4. Display survey\n");
	printf("5. Finalize survey\n");
	printf("6. Exit\n");
}
