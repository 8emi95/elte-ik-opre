#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "bead2.h"


int main(int argc, char** argv){
	system("clear");
	
	setlocale(LC_ALL, "");
	printf("=====================\n    King of Stands\n=====================\n\n");
	printf("loading users...");
	loadUsers();
	printf("done\n");
	printf("loading events...");
	loadEvents();
	printf("done\n");
	printf("\n");
	printf("Nyomj egy gombot a kezdéshez\n");
	getchar();
	
	clearInput();
	do{
		system("clear");
		printf("FŐMENÜ\n---------------------\n");
		printf("user: felhasználók kezelése\n");
		printf("event: események kezelése\n");
		printf("x: mentés és kilépés\n");
		printf("---------------------\n");
		readline();
		if (strcmp(input, "user") == 0){
			manageUser();
			clearInput();
		} else if (strcmp(input, "event") == 0){
			manageEvent();
			clearInput();
		}
	} while (strcmp(input, "x") != 0);
	
	//save data
	saveUsers();
	saveEvents();
	system("clear");
	return 0;
}

