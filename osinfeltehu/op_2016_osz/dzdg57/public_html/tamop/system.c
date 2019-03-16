#include <stdio.h>

void main() {
	int status;
	printf("Elinditom az ls -al parancsot!\n");
	status = system("ls -al");
	// Ennek a végrehajtása alatt a program várja a system
	// rendszerhívás végét.
	printf("Az eredmeny: %d", status);
}