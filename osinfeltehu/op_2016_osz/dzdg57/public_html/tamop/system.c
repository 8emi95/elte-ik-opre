#include <stdio.h>

void main() {
	int status;
	printf("Elinditom az ls -al parancsot!\n");
	status = system("ls -al");
	// Ennek a v�grehajt�sa alatt a program v�rja a system
	// rendszerh�v�s v�g�t.
	printf("Az eredmeny: %d", status);
}