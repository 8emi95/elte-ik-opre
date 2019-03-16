#ifndef view_h
#define view_h

#include "controller.h"

struct view
{
	controller c;
};

typedef struct view view;

void run(view* v);
void print_menu();

#endif
