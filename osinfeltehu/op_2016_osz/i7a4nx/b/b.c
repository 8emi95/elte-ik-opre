//https://www.codingunit.com/c-tutorial-binary-file-io
//https://www.tutorialspoint.com/cprogramming/c_file_io.htm
//http://stackoverflow.com/questions/22839032/parsing-command-line-using-argv-and-argc-in-c
//http://stackoverflow.com/questions/2537681/how-to-implement-a-set
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>

struct visitorType {
   char  	name[35];
   char  	email[50];
   int		registeredEventIds[50];	
   //eventCounter?
} visitorType; 

struct eventType {
   char  	eventName[35];
   int		event_id;	
   //eventCounter?
} eventType; 

void getTime()
{
    time_t current_time;
    char* c_time_string;

    /* Obtain current time. */
    current_time = time(NULL);

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);
}


 /*nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát*/
 /* jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! */
 /*vendég jelentkezéshez !?*/
 /*VENDÉGEK ÉS RENDEZVÉNYEK ADATAI KÜLÖN FÁJLBA?*/
 int signup(char* name, char* email, int event_id){
	int order;
	return order;
	}
 
 /* a jelentkezettek listázásához*/
 int listGuests(int event_id){
	int err;
	return err;
	}
	
 /*egy vendég adatainak módosításához*/ 
 /*bemenő pareméterek?*/
 int modifyGuestData(char* name){
	 int err;
	 return err;
	}

 /*vendég törlése*/ 
 /*bemenő pareméterek?*/
 int deleteGuest(char* name){
	 int err;
	 return err;
	}

 /*új rendezvény indítás*/
 /*VENDÉGEK ÉS RENDEZVÉNYEK ADATAI KÜLÖN FÁJLBA?*/
 int createEvent(char* eventName){
	int err;
	return err;
	}

 /*egy rendezvény összes adatának törlése*/
  int deleteEventData(char* eventName){
	int err;
	return err;
	}
 
 /*A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!*/
 

int main(int argc,char** argv){
 
 struct visitorType visitor;
 struct eventType event;
 
 char* name;
 char* email;
 int event_id;
 
 FILE* ptr_visitorsFile;
 FILE* ptr_eventsFile;
 
 ptr_visitorsFile = fopen( "visitorsData" , "a+b" );
 ptr_eventsFile = fopen( "eventsData" , "a+b" );
 
 
 name="bela";
 email="bela@gmail.com";
 event_id=123456;
 
 strcpy(visitor.name,name);
 strcpy(visitor.email,email);
 
 visitor.registeredEventIds[0]=event_id;
 visitor.registeredEventIds[1]=event_id;
 visitor.registeredEventIds[2]=3;
 visitor.registeredEventIds[3]=9;
 
 int* ptr;
 ptr = visitor.registeredEventIds;
 int i;
 for(i=0;i<4;++i){
	printf("%i\n", *(ptr+i));
 }
 
  
 fclose(ptr_visitorsFile);
 fclose(ptr_eventsFile);
   
return 0;
}
