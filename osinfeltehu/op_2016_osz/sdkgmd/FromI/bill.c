//#include <stdio.h>
#include <curses.h>
#include <wchar.h>
int main()
{
	printf("Nyomj le egy billentyut!\n");
	while(getch()==ERR);
}
