#include <stdlib.h>
#include <stdio.h>

struct Person{
	char* name;
	char* email;
	int* eventID;
};

void addPerson(char* name, char* email, int* id){
	struct Person p;
	p.name = name;
	p.email = email;
	p.eventID = id;
	printf("person added with %s %s %s\n", p.name, p.email, p.eventID);
}

void modifyPerson(){
	
}

void deletePerson(){
	
}

void listGuests(int arr[100]){
	int i;
	printf("Listing guests:\n");
	for (i = 0; i<3; ++i){
		printf("%i\n",arr[i]);
	}	
}

void startNewEvent(char c[100]){
	printf("Event with %s. number created!\n", c);
	
}

void deleteEventData(int c[100], int data){
	int i;
	for (i = 0; i<3; ++i){
		if (c[i] == data){
			printf("VAN ILYEN\n");
		}
	}
	
}


int main(){
	
	int array[100];
	
	char event[100];
	
	int data = 234;
	array[0] = 123;
	array[1] = 234;
	array[2] = 345;
	/*int i;
	for (i = 0; i<3; ++i){
		printf("%i\n",array[i]);
	}*/
	
	deleteEventData(array, data);
	listGuests(array);
	
//	printf("Enter event number: \n");
	
//	gets(event);
	
	char* nev;// = "AB";
	char* mail;// = "SS";
	int idd;// = 11;
	
	printf("name:\n");
	gets(nev);
	printf("mail:\n");
	gets(mail);
	printf("id:\n");
	gets(idd);
	
	addPerson(nev, mail, idd);
	
	startNewEvent(event);
	
//	printf("%c\n", c);
	/*while (c){
		printf("Enter number: \n");
		
		if (c == '1'){
			//printf("IF 1\n");
			printf("Add new Event ID:\n");
			c = getchar();
			startNewEvent(c);
		} else if (c == '2') {
			printf("IF 2\n");
		} else if (c == '3') {
			printf("IF 3\n");
		} 
		c = getchar();
		
	}*/
	return 0;
}