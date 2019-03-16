/*
Vári Csaba Géza
F7RMYO
elso beadando
*/

/*
Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", 
elektronikus látogatói nyilvántartást készített. 
A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, 
a rendezvény azonosító számát! 
Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg 
a regisztráció! 

Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, 
egy vendég adatainak módosításához, törléséhez. 
Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! 
A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) 
is(ezt nem kell bekérni)! 
*/

/*
guest data:
name, email, event ID, registration time
*/

/*
event data:
ID, name, event date?
*/

#include <stdio.h>
#include <stdlib.h>

//display menu
int printMenu()
{
	printf("=====================\n");
	printf("***King of Stands***\n");
	printf("=====================\n");
	
	printf("1: New guest\n");
	printf("2: List guests\n");
	printf("3: Update guest data\n");
	printf("4: Delete guest\n");
	
	printf("-----------------------\n");
	
	printf("5: New event\n");
	printf("6: Delete event data\n");
	printf("-----------------------\n");
	printf("0: Exit\n");
    printf("\n");
	
	printf("Choose an option: \n");
	
	return;
}

int main(int argc,char ** argv) 

{	
 //menu
 
 while () 
 {
	char c;
    do {
		printMenu();
		
		c = getchar();
		
		switch(c) {
			case '0' :
				exit(0);
			break;
			case '1':
				//adding a guest
				
			break;
			case '2':
				//list guests
				
			break;
			case '3':
				//change guest data
				
			break;
			case '4':
				//delete guest
			
			break;
			case '5':
				//add event
				
			break;
			case '6':
				//delete event
				
			break;
			default:
				printf("Bad input. Please choose from 0 - 6.\n");			
		}

		
    } while (isspace(c));

	
	 
	 
 }	 
 
 return 0;
}