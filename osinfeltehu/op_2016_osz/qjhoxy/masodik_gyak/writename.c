#include "stdlib.h"
#include "stdio.h"

int main()
{
	FILE* f = fopen("out.txt", "w");
	fputs("Laszlo Tamas\n", f);
	fprintf(f, "Laszlo Tamas\n");
	fclose(f);
}
