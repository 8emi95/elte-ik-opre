signal:

#include <signal.h>

int (*signal (jelzes, fuggveny))();
int jelzes;
int (*fuggveny)();

vagy

#include <signal.h>

void (*signal (int jelzes, void (*fuggveny)(int)))(int);
