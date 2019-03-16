#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_SIZE 512
#define VISITOR_FILE "visitors.dat"
#define VISITOR_TEMP_FILE "visitors.tmp"
#define MODE_SKIP 0
#define MODE_FILE 1

typedef struct {
	char name[50];
	char email[255];
	char eventNumber[10];
} visitorStruct;

visitorStruct *visitors;
int visitorCount = 0;
FILE *visitorFile = NULL;

int menu();
void readData(const char* text, char* target);
void listVisitors();
void initVisitors();
void destroyVisitors();
void emptyVisitors();
void writeVisitor(visitorStruct visitor, int mode);
void writeVisitorToFile(visitorStruct* visitor);
void addVisitor();
void readGuestFile();
void closeFile(FILE **fp);

int main(int argc, char ** argv) {
	initVisitors();
	readGuestFile();
	int result = menu();
	destroyVisitors();

	return result;
}

void initVisitors() {
	visitors = malloc(sizeof(visitorStruct));
}

void destroyVisitors() {
	free(visitors);
}

void readGuestFile() {
	visitorStruct *tmp = malloc(sizeof(visitorStruct));
	char *mode = "wb";
	FILE *fp = fopen(VISITOR_FILE, "rb");

	if (NULL != fp) {
		mode = "ab";
		while (fread(tmp, sizeof(visitorStruct), 1, fp) == 1) {
			writeVisitor((*tmp), MODE_SKIP);
		}
		fclose(fp);

	} else {
		fp = fopen(VISITOR_FILE, mode);
		fclose(fp);
	}
	free(tmp);
}

void closeFile(FILE **fp) {
	fclose(*fp);
	*fp = NULL;
}

void readData(const char* text, char* target) {
	printf("%s\t: ", text);
	scanf("%s", target);
}

void listVisitors() {
	int cv;

	for (cv = 0; cv < visitorCount; cv++) {
		printf("%d. %s (%s - Event %s) \n", (cv + 1), visitors[cv].name,
				visitors[cv].email, visitors[cv].eventNumber);
	}
}

void writeVisitor(visitorStruct visitor, int mode) {
	visitorStruct *tmp = realloc(visitors,
			(visitorCount + 1) * sizeof(visitorStruct));
	if (tmp == NULL) {
		printf("Unfortunately can not write visitor !! \n");
	} else {
		visitors = tmp;
		visitors[visitorCount] = visitor;
		++visitorCount;
		if (mode == MODE_FILE) {
			writeVisitorToFile(&visitor);
		}
	}
}

void writeVisitorToFile(visitorStruct* visitor) {
	if (NULL == visitorFile) {
		visitorFile = fopen(VISITOR_FILE, "ab");
	}
	fwrite(visitor, sizeof(visitorStruct), 1, visitorFile);
	closeFile(&visitorFile);
}

void addVisitor() {
	visitorStruct visitor;
	printf("Please enter visitor's data\n");
	readData("Name", visitor.name);
	readData("Email", visitor.email);
	readData("Event", visitor.eventNumber);
	writeVisitor(visitor, MODE_FILE);
	printf("Visitor number is: %d", visitorCount);
}

void emptyVisitors() {
	destroyVisitors();
	initVisitors();
	visitorCount = 0;
	if (NULL != visitorFile) {
		closeFile(&visitorFile);
	}
	visitorFile = fopen(VISITOR_FILE, "wb");
	closeFile(&visitorFile);
}

void saveToFile() {
	int i;
	FILE *tempFile = fopen(VISITOR_TEMP_FILE, "r");
	if (tempFile != NULL){
		fclose(tempFile);
	}
	tempFile = fopen(VISITOR_TEMP_FILE, "w");
	for (i = 0; i < visitorCount; ++i) {
		fwrite(&visitors[i], sizeof(visitorStruct), 1, tempFile);
	}

	closeFile(&tempFile);

	remove(VISITOR_FILE);
	rename(VISITOR_TEMP_FILE, VISITOR_FILE);

}

void deleteVisitor(int indexToRemove) {
	visitorStruct* temp = malloc((visitorCount - 1) * sizeof(visitorStruct));
	if (indexToRemove != 0) {
		memcpy(temp, visitors, indexToRemove * sizeof(visitorStruct));
	}
	if (indexToRemove != visitorCount - 1) {
		memcpy(temp + indexToRemove, visitors + indexToRemove + 1, (visitorCount - indexToRemove - 1) * sizeof(visitorStruct));
	}

	free(visitors);
	visitors = temp;
	--visitorCount;

	saveToFile();
}

void removeVisitors() {
	if (visitorCount > 0) {
		printf("You have %d visitors! Give me the number you want to delete: ",
				visitorCount);
		int read = -1;
		int visitorNumber;
		while (read < 0) {
			read = scanf("%d", &visitorNumber);
			if (read < 0 || visitorNumber < 1 || visitorNumber > visitorCount) {
				printf("Your number is not valid");
			} else {
				deleteVisitor(visitorNumber - 1);
			}
		}
	} else {
		printf("You don't have any visitors!");
	}
}

int menu() {
	int choice = 1;
	do {
		printf("\nChoose an action (0-2): \n");
		printf("1. Add visitor\n");
		printf("2. List visitor\n");
		printf("3. Remove visitor\n");
		printf("4. Empty visitors\n");
		printf("0. Exit\n");
		fflush(stdin);
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			addVisitor();
			break;
		case 2:
			listVisitors();
			break;
		case 3:
			removeVisitors();
			break;
		case 4:
			emptyVisitors();
			break;

		}

	} while (choice > 0);

	return 0;
}
