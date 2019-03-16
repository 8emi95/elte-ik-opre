#include <stdio.h>
#include <stdlib.h>
#include <errno.h> //perror
#include <string.h> //memcpy
#include <time.h>

//--------------------------------------------------------------------------------------------------
//-                                       TYPES AND STRUCTURES                                     -
//--------------------------------------------------------------------------------------------------

typedef struct {
	char name[20];
	char email[20];
	int line_number;
	int program_id;
	time_t registration_time;
} GUEST_DATA;

typedef struct {
	int id;
	int guest_num;
} PROGRAM_DATA;


//--------------------------------------------------------------------------------------------------
//-                                          FILE HANDLING                                         -
//--------------------------------------------------------------------------------------------------


FILE* safeOpenFile(const char* path, const char* mode) {
	FILE* f = fopen(path, mode);
	if (f == NULL) {
		if (errno == 2 && (strcmp(mode, "rb") == 0 || strcmp(mode, "r") == 0|| strcmp(mode, "rb+") == 0)) {
			//file not exists - try to create then open it
			f = fopen(path, "wb");
			fclose(f);
			f = fopen(path, mode);
			if (f == NULL) {
				perror("File error");
				printf("Error while trying to open %s\n", path);
				exit(1);
			}
		} else {
			perror("File error");
			printf("Error while trying to open %s\n", path);
			exit(1);
		}
	}
	return f;
}


//--------------------------------------------------------------------------------------------------
//-                                        DATABASE HANDLING                                       -
//--------------------------------------------------------------------------------------------------

void printGuestDataHeader() {
	printf("\n\n| Registration Time | Name | Email | Program ID | Number |\n");
	printf(    "----------------------------------------------------------\n");
}

void printGuestData(GUEST_DATA guest) {
	printf("| %s | %s | %s | %4d | %4d |\n",
			ctime(&guest.registration_time), guest.name, guest.email, guest.program_id, guest.line_number);
}

/*char* readData(char* text) {
	char dd;
	printf("%s", text);
	scanf(" %c", dd);
	return dd;
}*/

void flush() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int readInt(char* text) {
	int i = 0;
	printf("%s \n", text);
	scanf("%d", &i);
	return i;
}

int getLineNumber(int id){
	int num = 1;
	GUEST_DATA guest;
	FILE* f = safeOpenFile("guest.db", "rb");
	while (fread(&guest, sizeof(guest), 1, f) > 0) {
		if (id == guest.program_id) {
			num = num + 1;
		}
	}
	fclose(f);
	printf("Your number: %d \n", num);
	return num;
}


void listGuest(/*int id*/) {
	FILE* f = safeOpenFile("guest.db", "rb");
	GUEST_DATA guest;
	printGuestDataHeader();
	while (fread(&guest, sizeof(guest), 1, f) > 0) {
	//	if (id == guest.program_id) {
			printGuestData(guest);
	//	}
	}
	fclose(f);
}

void newGuest() {
	GUEST_DATA guest;
	
	guest.registration_time = time(0);
	printf("Type your Name: \n");
	flush();
	gets(guest.name);
	printf("Type your Email: \n");
	gets(guest.email);
	guest.program_id = readInt("Add Program ID");
	guest.line_number = getLineNumber(guest.program_id);

	FILE* f = safeOpenFile("guest.db", "ab");
	fwrite(&guest, sizeof(guest), 1, f);
	fclose(f);
}

void dataCopy(){
	remove("tmp.db");
	FILE* f = safeOpenFile("guest.db", "rb");
	FILE* f2 = safeOpenFile("tmp.db", "ab");
	GUEST_DATA guest;
	while (fread(&guest, sizeof(guest), 1, f) > 0) {
			fwrite(&guest, sizeof(guest), 1, f2);
	}
	fclose(f);
	fclose(f2);
	remove("guest.db");
}

void edit(char* name, int i){
	dataCopy();
	char tmp[20];
	GUEST_DATA guest;
	FILE* f = safeOpenFile("guest.db", "ab");
	FILE* f2 = safeOpenFile("tmp.db", "rb");
	while (fread(&guest, sizeof(guest), 1, f2) > 0) {
		if(0 == strcmp(name,guest.name)){
			if(i ==0){
				printf("Add new name.\n");
				flush();
				gets(tmp);
				strcpy(guest.name, tmp);
			}
			if(i == 1){
				printf("Add new email.\n");
				flush();
				gets(tmp);
				strcpy(guest.email, tmp);
			}

		}
		fwrite(&guest, sizeof(guest), 1, f);
	}
	fclose(f);
	fclose(f2);
}

void showMenu2() {
	printf("\n\n1 - Edit Name\n2 - Edit Email");
	printf("\n\n0 - Main menu\n\nYour choice: ");
}

void GuestDataModifications(){
	char name[20];
	printf("Add name, you would like to modify.\n");
	flush();
	gets(name);
	int choice = -1;
	do {
		choice = -1;
		while (choice < 0 || choice > 2) {
			showMenu2();
			scanf("%d", &choice);
		}
		switch (choice) {
			case 1: edit(name, 0);
					break;
			case 2: edit(name, 1);
					break;
			case 0: break;
			default: printf("\nInvalid menu option.");
		}
	} while (choice != 0);
}

void guestDataDelete(int id){
	dataCopy();
	char name[20]; 
	GUEST_DATA guest;
	FILE* f2 = safeOpenFile("tmp.db", "rb");
	FILE* f = safeOpenFile("guest.db", "ab");
	int mmm;
	if(id == -1){
		printf("Add name, you would like to deleted.\n");
		flush();
		gets(name);
		while (fread(&guest, sizeof(guest), 1, f2) > 0) {
			if (0 == strcmp(name,guest.name)) {
				printf("Delet Completed!\n");
			}else{
				fwrite(&guest, sizeof(guest), 1, f);
			}
		}
	}else{
		while (fread(&guest, sizeof(guest), 1, f2) > 0) {
			if (id != guest.program_id) {
				fwrite(&guest, sizeof(guest), 1, f);
			}
		}
	}
	fclose(f);
	fclose(f2);
}

void NewProgramRegistration(int id){
	PROGRAM_DATA data;
	data.id = id;
	data.guest_num = 0;
	
	FILE* f = safeOpenFile("program.db", "ab");
	fwrite(&data, sizeof(data), 1, f);
	fclose(f);
}

void deleteProgramData(int id){
	PROGRAM_DATA program;
	FILE* f = safeOpenFile("program.db", "rb+");
	while (fread(&program, sizeof(program), 1, f) > 0) {
		if (id == program.id) {
			program.guest_num = 0;
		}
	}
	fclose(f);
	guestDataDelete(id);
}




//--------------------------------------------------------------------------------------------------
//-                                          MENU HANDLING                                         -
//--------------------------------------------------------------------------------------------------


void showMenu() {
	printf("\n\n1 - Guest's Registration\n2 - List of Guest");
	printf("\n3 - Guest Data Modifications\n4 - Delete Guest");
	printf("\n5 - New Program Registration\n6 - Delete Program Data");
	printf("\n\n0 - Exit\n\nYour choice: ");
	
}

//-------------------------------------


void handleMenu() {
	int choice = -1;
	do {
		choice = -1;
		while (choice < 0 || choice > 6) {
			showMenu();
			scanf("%d", &choice);
		}
		int id;
		switch (choice) {
			case 1: newGuest();
					break;
			case 2: listGuest();
					break;
			case 3: GuestDataModifications();
					break;
			case 4: guestDataDelete(-1);
					break;
			case 5: id = readInt("Add id the new Program.");
					NewProgramRegistration(id);
					break;
			case 6: id = readInt("Add id, you would like to deleted.");
					deleteProgramData(id);
					break;
			case 0: break;
			default: printf("\nInvalid menu option.");
		}
	} while (choice != 0);
}


//--------------------------------------------------------------------------------------------------
//-                                          MAIN PROGRAM                                          -
//--------------------------------------------------------------------------------------------------

int main() {
	handleMenu();
	return 0;
}
